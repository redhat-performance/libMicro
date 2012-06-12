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
 * nanosleep
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include "libmicro.h"

#define DEFDELTA	1

static int	optd = DEFDELTA;
static int	opts = 0;
static int	optr = 0;

int
benchmark_init(void)
{
	lm_tsdsize = 0;

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "d:sr");

	(void) snprintf(lm_usage, sizeof(lm_usage),
			"\t-d delta to wait in nanoseconds (defaults to 1ns)\n"
			"\t-r provide remainder argument\n"
			"\t-s skip libc making system call directly\n"
			"note: measures nanosleep()\n");
	(void) snprintf(lm_header, sizeof(lm_header), "%8s %9s %8s", "delta", "remainder", "method");

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	int ret = 0;

	switch (opt) {
	case 'd':
		optd = atoi(optarg);
		if (optd <= 0) {
			fprintf(stderr, "Invalid delta, %d <= 0\n", optd);
			exit(1);
		}
		break;
	case 's':
		opts = 1;
		break;
	case 'r':
		optr = 1;
		break;
	default:
		ret = -1;
	}
	return ret;
}

/*ARGSUSED*/
int
benchmark(void *tsd, result_t *res)
{
	int				i, ret;
	int				errors = 0;
	struct timespec	req = { 0, optd };
	struct timespec	rem;
	struct timespec	*remp = NULL;

	if (optr) remp = &rem;

	if (opts) {
		for (i = 0; i < lm_optB; i++) {
			ret = syscall(SYS_nanosleep, &req, remp);
			if (ret < 0) errors++;
		}
	}
	else {
		for (i = 0; i < lm_optB; i++) {
			ret = nanosleep(&req, remp);
			if (ret < 0) errors++;
		}
	}

	res->re_count = i;
	res->re_errors = errors;

	return 0;
}

char *
benchmark_result(void)
{
	static char	result[256];

	(void) snprintf(result, sizeof(result), "%8d %9s %8s", optd, (optr ? "yes" : "no"), (opts ? "syscall" : "libc"));

	return result;
}
