/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the "License").  You may not use this file except
 * in compliance with the License.
 *
 * You can obtain a copy of the license at
 * src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing
 * permissions and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL
 * HEADER in each file and include the License file at
 * usr/src/OPENSOLARIS.LICENSE.  If applicable,
 * add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your
 * own identifying information: Portions Copyright [yyyy]
 * [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2012 Red Hat, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Adapted from agua_frag_test.c, which was based on mmap.rep.anon.c, Version
 * 4, see BZ https://bugzilla.redhat.com/show_bug.cgi?id=790921.
 *
 * This version benchmarks the munmap() calls that are used to create the
 * holes in the VMAs.
 *
 * Notes:
 *
 *  1. CLR_MMAP_TX from the original is now the lm_optB (-B) parameter value
 *     - See bench.sh which should contain the default value used
 *     - The original test used 2,500 for this value
 *  2. The original program measured the time it took to run the entire
 *     benchmark loop, and reported the statistics that way; here the
 *     benchmark() routine is just calling the first mlist_dealloc(),
 *     with the benchmark_initbatch() performing the original mmap() calls
 *     for setup outside the measurement window, and benchmark_finibatch()
 *     calling mlist_dealloc() when capping the total number of mmaps.
 *     - While the original ran this for 1,000 * KLOOPS, where KLOOPS was
 *       defined to be 10 (so 10,000), that takes about 40+ minutes to run;
 *       see bench.sh for the equivalent value specified by -C
 */

#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

#include "libmicro.h"

typedef struct _mlist_
{
    struct _mlist_ *prev;
    struct _mlist_ *next;
    void           *mmap_ptr;
    size_t          len;
} mlist_t, *mlist_p;

typedef struct {
	int		ts_once;
    mlist_p ml_initial;
    mlist_p ml;
    mlist_p	*mls;
    size_t  mseg_size;
    int     nmap;
    int     unmap_gap;
    int     msize_cnt;
    int     loop_iter;
} tsd_t;

#define INITIAL_GAP              5
#define GAP_INCREMENT            3
#define GAP_MAXIMUM             20

#define MEMSIZE_1              128
#define MEMSIZE_2            25000
#define MEMSIZE_3           100000

#define CLR_MMAP_CNT_DEF     35000  /* override using -c XX */
#define ITER_SIM_TX          10000  /* override using -i XX */
#define MAX_MMAP_CNT_DEF    200000  /* override using -m XX */

static int			optc = CLR_MMAP_CNT_DEF;
static int			opti = ITER_SIM_TX;
static int			optm = MAX_MMAP_CNT_DEF;

static unsigned int	initial_map_count = 0;
static unsigned int max_map_count = 0;
static unsigned int min_map_count = UINT_MAX;

int
benchmark_init(void)
{
	lm_tsdsize = sizeof(tsd_t);

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "c:i:m:");

	(void) snprintf(lm_usage, sizeof(lm_usage),
            "\t[-c] number of maps to unmap to drop total below maximum (see -m) (default %d)]\n"
            "\t[-i] number of maps to create before each run [default %d]\n"
            "\t[-m] maximum number of maps above which we force unmaps to cap memory usage (default %d)]\n"
            "notes: measures mmap() under high fragmentation conditions\n",
            CLR_MMAP_CNT_DEF, ITER_SIM_TX, MAX_MMAP_CNT_DEF);

	(void) snprintf(lm_header, sizeof(lm_header), "%8s %8s %8s", "maxcnt", "clrcnt", "itrcnt");

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 'c':
		optc = atoi(optarg);
        if (optc < 0) {
            fprintf(stderr, "Warning: invalid value for -c, %d, defaulting to %d\n", optc, CLR_MMAP_CNT_DEF);
            optc = CLR_MMAP_CNT_DEF;
        }
		break;
	case 'i':
		opti = atoi(optarg);
        if (opti < 0) {
            fprintf(stderr, "Warning: invalid value for -i, %d, defaulting to %d\n", opti, ITER_SIM_TX);
            opti = ITER_SIM_TX;
        }
		break;
	case 'm':
		optm = atoi(optarg);
        if (optm < 0) {
            fprintf(stderr, "Warning: invalid value for -m, %d, defaulting to %d\n", optm, MAX_MMAP_CNT_DEF);
            optm = MAX_MMAP_CNT_DEF;
        }
		break;
	default:
		return -1;
	}
	return 0;
}

static unsigned int
count_maps(void)
{
    unsigned int map_count = 0;

    FILE *file = fopen("/proc/self/maps", "r");
    if (file != NULL) {
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL) {
            map_count++;
        }
        fclose(file);
    }
    else {
        perror("fopen('/proc/self/maps')");
    }

    return map_count;
}

int
benchmark_initrun(void)
{
    if (lm_optG >= 3) {
        /*
         * Open /proc/self/maps and count the number of lines to check how
         * many maps we start out with initially.
         */
        unsigned int map_count = count_maps();
        if (map_count > 0) {
            fprintf(stderr, "DEBUG3: initial map count: %d\n", map_count);
        }
        initial_map_count = map_count;
    }
}

int
benchmark_initbatch(void *tsd)
{
	tsd_t	*ts = (tsd_t *)tsd;
	int		errors = 0;

	if (ts->ts_once++ == 0) {
        ts->ml = (mlist_p)calloc(1, sizeof(mlist_t));
        if (ts->ml == NULL) {
            fprintf(stderr, "mlist allocation error: (%d) %s\n", errno, strerror(errno));
            exit(1);
        }
        ts->ml_initial = ts->ml;
        ts->nmap = 0;
        ts->unmap_gap = INITIAL_GAP;
        ts->msize_cnt = 0;
        ts->loop_iter = 0;
        /*
         * The benchmark() routine will invoke mmap() opti times, so just
         * preallocate the mlist structures, link them all up, calculate the
         * sizes, etc. so that the benchmark can focus on just making those
         * calls.
         */
        ts->mls = (mlist_p *)calloc(opti, sizeof(mlist_p));
        if (ts->mls == NULL) {
            fprintf(stderr, "mlist array of pointers allocation error: (%d) %s\n", errno, strerror(errno));
            exit(1);
        }
	}

    int i;
    for (i = 0; i < opti; i++) {
        /*
         * Note that we always have one preallocated, so we always store the
         * last one allocated into the current slot.
         */
        assert(ts->mls[i] == NULL);
        ts->mls[i] = ts->ml;
        /*
         * Calculate varying sizes for maps, emphasis on small.
         *
         * Simulate a transaction by round-robin'in through a set of sizes
         * with a given distribution:
         *
         *   Size       Count
         *   ---------  -----
         *   MEMSIZE_1  16 ( 0-15)
         *   MEMSIZE_2   4 (16-19)
         *   MEMSIZE_3   1 (   20)
         */
        ts->mseg_size = (ts->msize_cnt <= 15)
            	? MEMSIZE_1
            	: (ts->msize_cnt <= 19)
            			? MEMSIZE_2
            			: MEMSIZE_3;
        ts->msize_cnt = (ts->msize_cnt >= 20)
				? 0
				: ts->msize_cnt + 1;
        /*
         * Note here that we always start with the initially allocated mlist
         * structure, and we'll end with one unallocated for the next one.
         */
        assert(ts->ml->len == 0);
        ts->ml->len = ts->mseg_size;
        ts->ml->mmap_ptr = NULL; // mmap() call will occur in benchmark()
        void *mmap_ptr = (void *)mmap(0, ts->ml->len, PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
        if (mmap_ptr == MAP_FAILED) {
            fprintf(stderr, "MMAP ERROR - %d:%s\n", errno, strerror(errno));
            exit(1);
        }
        ts->ml->mmap_ptr = mmap_ptr;
        ts->nmap++;
        /*
         * "Preallocate" the next structure
         */
        assert(ts->ml->next == NULL);
        ts->ml->next = calloc(1, sizeof(mlist_t));
        if (ts->ml->next == NULL) {
            fprintf(stderr, "mlist structure allocation error: (%d) %s\n", errno, strerror(errno));
            exit(1);
        }
        ts->ml->next->prev = ts->ml;
        ts->ml = ts->ml->next;
    }

	return errors;
}

static int
mlist_dealloc(
        int         nunmaps,
        int         start_offset_gap,
        mlist_p     ml_initial,
        int         *nmap,
        int         unmap_gap,
        mlist_p     ml)
{
    int     errors      = 0;
    mlist_p	pml         = ml_initial;
    int 	nmap_tx     = nunmaps;
    int 	offset_cntr = start_offset_gap;

    /* move into the list a little before starting */
    while ((offset_cntr > 0) && (pml->next != NULL)) {
        pml = pml->next;
        offset_cntr--;
    }

    if (pml->next == NULL)
        return errors;

    /* Attempt to unmap the number of mappings indicated */
    while ((nmap_tx > 0) && (pml->next != NULL)) {
        pml->next->prev = pml->prev;
        pml->prev->next = pml->next;

        int ret = munmap(pml->mmap_ptr, pml->len);
        if (ret < 0) errors++;

        mlist_p pml_tmp = pml;
        pml = pml->next;
        (void) free(pml_tmp);

        nmap_tx--;
        (*nmap)--;

        /* skip a few each time */
        int unmap_cntr;
        for (unmap_cntr = unmap_gap; (unmap_cntr > 0) && (pml != ml) && (pml->next != NULL); unmap_cntr--) {
            pml = pml->next;
        }
    }

    if (lm_optG >= 5 && nmap_tx > 0) fprintf(stderr, "DEBUG5: Warning: asked to unmap %d, still %d more to unmap\n", nunmaps, nmap_tx);

    return errors;
}

int
benchmark(void *tsd, result_t *res)
{
	tsd_t	*ts = (tsd_t *)tsd;

    int		nmap = ts->nmap;
    if (lm_optG >= 4) fprintf(stderr, "DEBUG4: benchmark: nmap = %d\n", nmap);

    /*
     * Create varying size holes in the address space. The value of lm_optB is
     * equivalent to the value of CLR_MMAP_TX_DEF from the original program.
     */
    res->re_errors = mlist_dealloc(lm_optB, ts->unmap_gap, ts->ml_initial, &ts->nmap, ts->unmap_gap, ts->ml);
    ts->unmap_gap = GAP_INCREMENT + ((ts->unmap_gap > GAP_MAXIMUM) ? 0 : ts->unmap_gap);

    /*
     * Mimics what the original code did since the number of times this
     * benchmark() routine is called is the loop_iter count from the original
     * program. Used to skip an initial set of entries when making holes.
     */
    ts->loop_iter++;

    /*
     * Accounting for the libMicro framework, this is the count of munmap
     * calls. The number of munmap() calls might be less than the requested
     * number when the gap size skips more than the number of existing maps.
     */
    int diff = nmap - ts->nmap;
    assert(diff >= 0);
	res->re_count = nmap - ts->nmap;

	return 0;
}

int
benchmark_finibatch(void *tsd)
{
	tsd_t	*ts = (tsd_t *)tsd;
	int		errors = 0;

    /*
     * Place a cap on the maximum number of maps to limit overall memory
     * usage.
     *
     * NOTE: using loop_iter here as the starting offset gap can lead to a
     * case where mlist_dealloc() can't deallocate anything because we'll skip
     * a huge chunk of allocations.
     */
    if (ts->nmap > optm) {
        errors += mlist_dealloc(optc, ts->loop_iter, ts->ml_initial, &ts->nmap, ts->unmap_gap/2, ts->ml);
    }

    if (lm_optG >= 3) {
        unsigned int map_count = count_maps() - initial_map_count;
        if (map_count > max_map_count) {
            max_map_count = map_count;
        }
        if (map_count < min_map_count) {
            min_map_count = map_count;
        }
    }

    int i;
    for (i = 0; i < opti; i++) {
        ts->mls[i] = NULL;
    }

	return errors;
}

int
benchmark_finirun(void)
{
    if (lm_optG >= 3) {
        fprintf(stderr, "DEBUG3: min map count: %u, max map count: %u\n", min_map_count, max_map_count);
    }
}

char *
benchmark_result(void)
{
	static char		result[256];

	(void) snprintf(result, sizeof(result), "%8d %8d %8d", optm, optc, opti);

	return result;
}
