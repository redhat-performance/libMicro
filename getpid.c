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
 * getpid
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
			"note: measures getpid()\n");

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
			(void) syscall(SYS_getpid);
		}
	}
	else {
		for (i = 0; i < lm_optB; i++) {
			(void) getpid();
		}
	}

	res->re_count = i;

	return 0;
}

char *benchmark_result(void)
{
	static char	result[256];

	(void) snprintf(result, sizeof(result), "%8s", (opts ? "syscall" : "libc"));

	return result;
}
