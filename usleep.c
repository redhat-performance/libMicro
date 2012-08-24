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
 * usleep()
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include "libmicro.h"

#define DEFDELTA	1

static int	optd = DEFDELTA;

int
benchmark_init(void)
{
	lm_tsdsize = 0;

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "d:");

	(void) snprintf(lm_usage, sizeof(lm_usage),
			"\t-d delta to wait in microseconds (defaults to 1us)\n"
			"note: measures usleep()\n");
	(void) snprintf(lm_header, sizeof(lm_header), "%8s", "delta");

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

	for (i = 0; i < lm_optB; i++) {
		ret = usleep(optd);
		if (ret < 0) errors++;
	}

	res->re_count = i;
	res->re_errors = errors;

	return 0;
}

char *
benchmark_result(void)
{
	static char	result[256];

	(void) snprintf(result, sizeof(result), "%8d", optd);

	return result;
}
