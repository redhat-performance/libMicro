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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Modifications by Red Hat, Inc.
 */

/*
 * open()
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "libmicro.h"

typedef struct {
	int		ts_size;
	int		ts_count;
	int	   *ts_fds;
} tsd_t;

#define	DEFF	"/dev/null"

static char	   *optf = DEFF;

int
benchmark_init(void)
{
	lm_tsdsize = sizeof (tsd_t);

	(void) snprintf(lm_usage, sizeof(lm_usage),
			"\t[-f file-to-open (default %s)]\n"
			"notes: measures open()\n",
			DEFF);

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "f:");

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 'f':
		optf = optarg;
		break;
	default:
		return -1;
	}
	return 0;
}

int
benchmark_initrun(void)
{
	setfdlimit(lm_optB * lm_optT + 10);

	return 0;
}

int
benchmark_initworker(void *tsd)
{
	tsd_t  *ts = (tsd_t *)tsd;
	int		errors = 0;

	ts->ts_count = lm_optB;
	ts->ts_size = lm_optB * sizeof(int);
	ts->ts_fds = (int *)malloc(ts->ts_size);
	if (NULL == ts->ts_fds) {
		errors++;
	}

	return errors;
}

int
benchmark_initbatch(void *tsd)
{
	tsd_t  *ts = (tsd_t *)tsd;

    if (lm_optB > ts->ts_count) {
        /*
         * It got bigger, expand the array of FDs, but only allocate a new
         * array if the size of the currently allocated array is insufficient.
         */
        int newsize = (lm_optB * sizeof(int));
		if (ts->ts_size < newsize) {
            if (pthread_self() == lm_default_thread) {
                /*
                 * Only the default thread handles checking the new upper
                 * limit.
                 */
                setfdlimit(lm_optB * lm_optT + 10);
            }

            ts->ts_size = 0;
            ts->ts_count = 0;
            free(ts->ts_fds);
            ts->ts_fds = malloc(newsize);
            if (NULL == ts->ts_fds) {
                return 1;
            }
            ts->ts_size = newsize;
        }
        ts->ts_count = lm_optB;
    }
    else if (lm_optB < ts->ts_count) {
        // It got smaller, just reduce the count we are working with.
        ts->ts_count = lm_optB;
    }
    // Else, it has not changed, so just go with it.

    int i;
    for (i = 0; i < ts->ts_count; i++) {
        ts->ts_fds[i] = -1;
    }
	return 0;
}

int
benchmark(void *tsd, result_t *res)
{
	tsd_t  *ts = (tsd_t *)tsd;
	int		i;

	for (i = 0; i < ts->ts_count; i++) {
		ts->ts_fds[i] = open(optf, O_RDONLY);
		if (ts->ts_fds[i] < 0) {
			res->re_errors++;
		}
	}
	res->re_count = i;

	return 0;
}

int
benchmark_finibatch(void *tsd)
{
	tsd_t  *ts = (tsd_t *)tsd;
	int		i;

	for (i = 0; i < ts->ts_count; i++) {
		(void) close(ts->ts_fds[i]);
	}

	return 0;
}
