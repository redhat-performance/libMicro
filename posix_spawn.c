/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the "License").	 You may not use this file except
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
 * usr/src/OPENSOLARIS.LICENSE.	 If applicable,
 * add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your
 * own identifying information: Portions Copyright [yyyy]
 * [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2012 Red Hat, Inc.	 All rights reserved.
 * Use is subject to license terms.
 */

/*
 * posix_spawn benchmark
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <spawn.h>

#include "libmicro.h"

#define DEFN	0
#define DEFP	2

static char exec_path[1024];
static char *argv[3];

static int	optn = DEFN;
static int	optp = DEFP;
static int	optt = 0;

static int	mmaps = 0;

static int	pagesize = 0;

int
benchmark_init(void)
{
	lm_defB = 128;
	lm_tsdsize = 0;
	pagesize = sysconf(_SC_PAGESIZE);

	(void) sprintf(lm_optstr, "n:p:t");

	(void) sprintf(lm_usage,
		"\t[-n number of mmaps (default %d)]\n"
		"\t[-p size of mmaps in pages (default %d)]\n"
		"\t[-t touch all mmap'd pages\n"
		"notes: measures posix_spawn/waitpid time of simple process()\n",
		DEFN, DEFP);

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 'n':
		optn = atoi(optarg);
		if (optn < 0)
			return -1;
		break;
	case 'p':
		optp = atoi(optarg);
		if (optp <= 0)
			return -1;
		break;
	case 't':
		optt = 1;
		break;
	default:
		return -1;
	}
	return 0;
}

/*ARGSUSED*/
int
benchmark_initbatch(void *tsd)
{
	(void) strcpy(exec_path, lm_procpath);
	(void) strcat(exec_path, "/exec_bin");

	argv[0] = exec_path;
	argv[1] = "1";
	argv[2] = NULL;

	if (mmaps == 0) {
		mmaps = 1;
		int size = (pagesize * optp);
		int i;
		for (i = 0; i < optn; i++) {
			char *val = (char *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0L);
			if (val == MAP_FAILED) {
				fprintf(stderr, "errno = %d, %s\n", errno, strerror(errno));
				return 1;
			}
			if (optt) {
				int j;
				for (j = 0; j < optp; j++) {
					*(val + (pagesize * j)) = '1';
				}
			}
		}
	}

	return 0;
}

/*ARGSUSED*/
int
benchmark(void *tsd, result_t *res)
{
	int i, ret, status;
	pid_t		pid;
	extern char **environ;

	for (i = 0; i < lm_optB && !res->re_errors; i++) {
		ret = posix_spawn(&pid, exec_path, NULL, NULL, argv, environ);
		if (ret != 0) {
			res->re_errors++;
			break;
		}

		if (waitpid(pid, &status, 0) < 0) {
			res->re_errors++;
			break;
		}

		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			res->re_errors++;
			break;
		}
	}

	res->re_count = i;

	return 0;
}
