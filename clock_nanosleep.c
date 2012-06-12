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
 * clock_nanosleep
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/syscall.h>

#include "libmicro.h"

#define DEFDELTA	1

static int	optr = 0;
static int	opts = 0;

static int			delta = DEFDELTA;
static clockid_t	bclock = CLOCK_REALTIME;
static int			flags = 0;

int
benchmark_init(void)
{
	lm_tsdsize = 0;

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "d:a:mrs");

	(void) snprintf(lm_usage, sizeof(lm_usage),
			"\t-d delta to wait in nanoseconds (defaults to 1ns)\n"
			"\t-a use absolute time (implies additional calls to clock_gettime)\n"
			"\t-m use MONOTONIC clock (defaults to REALTIME)\n"
			"\t-r provide remainder argument\n"
			"\t-s skip libc making system call directly\n"
			"note: measures clock_nanosleep()\n");

	(void) snprintf(lm_header, sizeof(lm_header), "%8s %8s %8s %9s %8s", "delta", "abs/rel", "clock", "remainder", "method");

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 'a':
		flags = TIMER_ABSTIME;
	case 'd':
		delta = atoi(optarg);
		if (delta <= 0) {
			fprintf(stderr, "Invalid delta for %s, %d <= 0\n", (flags == TIMER_ABSTIME ? "-a" : "-d"), delta);
			exit(1);
		}
		break;
	case 'm':
		bclock = CLOCK_MONOTONIC;
		break;
	case 'r':
		optr = 1;
		break;
	case 's':
		opts = 1;
		break;
	default:
		return -1;
	}
	return 0;
}

static void new_abstime(struct timespec *req)
{
    int64_t curtim;
    (void) clock_gettime(bclock, req);
    curtim = (req->tv_sec * 1000000000) + req->tv_nsec + delta;
    req->tv_sec  = (time_t)(curtim / 1000000000);
    req->tv_nsec =   (long)(curtim % 1000000000);
}

/*ARGSUSED*/
int
benchmark(void *tsd, result_t *res)
{
	int				i;
    int             ret;
    int				errors = 0;
	struct timespec req = { 0, delta };
	struct timespec rem;
	struct timespec *remp = NULL;

	if (optr) remp = &rem;

	if (opts) {
        if (flags == TIMER_ABSTIME) {
			for (i = 0; i < lm_optB; i++) {
                new_abstime(&req);
				ret = syscall(SYS_clock_nanosleep, bclock, flags, &req, remp);
                if (ret < 0) errors++;
			}
        }
        else {
			for (i = 0; i < lm_optB; i++) {
				ret = syscall(SYS_clock_nanosleep, bclock, flags, &req, remp);
                if (ret < 0) errors++;
			}
        }
	}
	else {
        if (flags == TIMER_ABSTIME) {
			for (i = 0; i < lm_optB; i++) {
                new_abstime(&req);
				ret = clock_nanosleep(bclock, flags, &req, remp);
                if (ret < 0) errors++;
			}
        }
        else {
			for (i = 0; i < lm_optB; i++) {
				ret = clock_nanosleep(bclock, flags, &req, remp);
                if (ret < 0) errors++;
			}
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

	(void) snprintf(result, sizeof(result), "%8d %8s %8s %9s %8s", delta, (flags == TIMER_ABSTIME ? "abs" : "rel"), (bclock == CLOCK_MONOTONIC ? "mono" : "real"), (optr ? "yes" : "no"), (opts ? "syscall" : "libc"));

	return result;
}
