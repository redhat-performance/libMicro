/*
 * Copyright 2012 Red Hat, Inc.
 *
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the "License").  You may not use this file except
 * in compliance with the License.
 *
 * See the License for the specific language governing
 * permissions and limitations under the License.
 */

/*
 * sched_yield()
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include "libmicro.h"

int	opts = 0;

int
benchmark_init(void)
{
	lm_tsdsize = 0;

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "s");

	(void) snprintf(lm_usage, sizeof(lm_usage),
			"\t-s skip libc making system call directly\n"
			"note: measures sched_yield()\n");

	(void) snprintf(lm_header, sizeof(lm_header), "%8s", "method");

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 's':
		opts = 1;
		break;
	default:
		return -1;
	}
	return 0;
}

/*ARGSUSED*/
int
benchmark(void *tsd, result_t *res)
{
	int	i;

	if (opts) {
		for (i = 0; i < lm_optB; i++) {
			(void) syscall(SYS_sched_yield);
		}
	}
	else {
		for (i = 0; i < lm_optB; i++) {
			(void) sched_yield();
		}
	}

	res->re_count = i;

	return 0;
}

char *
benchmark_result(void)
{
	static char	result[256];

	(void) snprintf(result, sizeof(result), "%8s", (opts ? "syscall" : "libc"));

	return result;
}
