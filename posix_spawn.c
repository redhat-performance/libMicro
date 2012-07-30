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
 * posix_spawn() / waitpid()
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
#define DEFR	0

static char exec_path[1024];
static char *argv[3];

static int	optn = DEFN;
static int	optp = DEFP;
static int	optr = DEFR;
static int	optt = 0;

static int	mmaps = 0;
static int	threads = 0;

static int	pagesize = 0;

int
benchmark_init(void)
{
	lm_defB = 128;
	lm_tsdsize = 0;
	pagesize = sysconf(_SC_PAGESIZE);

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "n:p:r:t");

	(void) snprintf(lm_usage, sizeof(lm_optstr),
		"\t[-n number of mmaps (default %d)]\n"
		"\t[-p size of mmaps in pages (default %d)]\n"
		"\t[-r number of background sleepr threads (default %d)]\n"
		"\t[-t touch all mmap'd pages\n"
		"notes: measures posix_spawn/waitpid time of simple process()\n",
        DEFN, DEFP, DEFR);

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 'n':
		optn = atoi(optarg);
		if (optn < 0) {
            optn = DEFN;
			return -1;
        }
		break;
	case 'p':
		optp = atoi(optarg);
		if (optp <= 0) {
            optp = DEFP;
			return -1;
        }
		break;
	case 'r':
		optr = atoi(optarg);
		if (optr <= 0) {
            optr = DEFR;
			return -1;
        }
		break;
	case 't':
		optt = 1;
		break;
	default:
		return -1;
	}
	return 0;
}

void *sleeper_thread(void *arg) {
    for (;;) {
        sleep(1);
    }
    return NULL;
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
		char *addr = NULL;
		for (i = 0; i < optn; i++) {
			char *val = (char *)mmap(addr, size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0L);
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
			addr = (val - (pagesize * (optp + 1)));
		}
	}

    if (threads == 0) {
        threads = 1;
        int i, ret;
        for (i = 0; i < optr; i++) {
            pthread_t	tid;
            ret = pthread_create(&tid, NULL, sleeper_thread, NULL);
            if (ret != 0) {
				fprintf(stderr, "errno = %d, %s\n", ret, strerror(ret));
				return 1;
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
