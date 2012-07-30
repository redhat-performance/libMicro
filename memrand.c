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
 * memory access time check
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

#include "libmicro.h"

static long long	opts = 1024*1024;
static long long	len = 0;
static long long	loopend = 0;

typedef struct {
	long long  **ts_data;
	long long	ts_result;
} tsd_t;

int
benchmark_init(void)
{
	lm_tsdsize = sizeof(tsd_t);

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "s:");

	(void) snprintf(lm_usage, sizeof(lm_usage),
		"       [-s size] number of bytes to"
		" access (default %ld)\n"
		"notes: measures \"random\" memory access times\n",
		opts);

	(void) snprintf(lm_header, sizeof(lm_header), "%8s", "size");

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 's':
		opts = sizetoll(optarg);
		break;
	default:
		return -1;
	}

	return 0;
}

int
benchmark_initworker(void *tsd)
{
	tsd_t	   *ts = (tsd_t *)tsd;
	long long	i, j;

	ts->ts_data = malloc(opts);
	if (ts->ts_data == NULL) {
		return 1;
	}

	len = opts / sizeof(long long *);
	long long	split = len / 3;

	/*
	 * use lmbench style backwards stride
	 */

	for (i = 0; i < len; i++) {
		j = i - split;
		if (j < 0)
			j = j + len;
		ts->ts_data[i] = (long long *)&(ts->ts_data[j]);
	}

	/* Ensure that we go through the data at least twice */
	loopend = (lm_optB > (2 * len)) ? lm_optB : 2 * len;

	if (loopend > lm_optB) printf("*** using -B of %lld instead of %d\n", loopend, lm_optB);

	return 0;
}

int
benchmark(void *tsd, result_t *res)
{
	tsd_t		   *ts = (tsd_t *)tsd;
	long long	  **ptr = ts->ts_data;
	int				i;

	for (i = 0; i < loopend; i += 10) {
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
		ptr = (long long **)*ptr;
	}

	ts->ts_result = (long long)*ptr;

	res->re_count = i;

	return 0;
}

char *
benchmark_result()
{
	static char	 result[256];

	(void) snprintf(result, sizeof(result), "%8ld ", opts);

	return result;
}
