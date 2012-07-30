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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/poll.h>

#include "libmicro.h"

#define	FIRSTPORT		12345

typedef struct {
	int			ts_once;
	int			*ts_lsns;
	int			*ts_accs;
	int			*ts_cons;
	struct sockaddr_in	*ts_adds;
} tsd_t;

static int			opta = 0;
static int			optc = 0;
static struct hostent		*host;

static struct linger	linger = { 1, 1 };
static int				reuseaddr = 1;

int
benchmark_init(void)
{
	lm_defB = 256;
	lm_tsdsize = sizeof (tsd_t);

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "ac");

	(void) snprintf(lm_usage, sizeof(lm_usage),
		"\t[-a] (measure accept() only)\n"
		"\t[-c] (measure connect() only)\n"
		"notes: measures connect()/accept()\n");

	return 0;
}

/*ARGSUSED*/
int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 'a':
		opta = 1;
		break;
	case 'c':
		optc = 1;
		break;
	default:
		return -1;
	}

	if (opta && optc) {
		(void) printf("warning: -a overrides -c\n");
		optc = 0;
	}

	return 0;
}

int
benchmark_initrun(void)
{
	setfdlimit(3 * lm_optB * lm_optT + 10);

	if ((host = gethostbyname("localhost")) == NULL) {
		perror("gethostbyname");
		exit(3);
	}

	return 0;
}

int
benchmark_initbatch_once(tsd_t *ts)
{
	int			errors = 0;

	ts->ts_lsns = (int *)malloc(lm_optB * sizeof (int));
	if (ts->ts_lsns == NULL) {
		perror("malloc");
		errors ++;
	}
	ts->ts_accs = (int *)malloc(lm_optB * sizeof (int));
	if (ts->ts_accs == NULL) {
		perror("malloc");
		errors ++;
	}
	ts->ts_cons = (int *)malloc(lm_optB * sizeof (int));
	if (ts->ts_cons == NULL) {
		perror("malloc");
		errors ++;
	}
	ts->ts_adds =
		(struct sockaddr_in *)malloc(lm_optB *
		sizeof (struct sockaddr_in));
	if (ts->ts_adds == NULL) {
		perror("malloc");
		errors ++;
	}

	int				port = FIRSTPORT;
	int				i;
	for (i = 0; i < lm_optB; i++) {
		ts->ts_cons[i] = -2;
		ts->ts_accs[i] = -2;
		ts->ts_lsns[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (ts->ts_lsns[i] < 0) {
			perror("init:socket:lsns");
			errors ++;
		}
		else if (fcntl(ts->ts_lsns[i], F_SETFL, O_NDELAY) == -1) {
			/*
			 * make accept socket non-blocking so in case of errors we don't
			 * hang
			 */
			perror("init:fcntl:lsns");
			errors ++;
			(void) close(ts->ts_lsns[i]);
			ts->ts_lsns[i] = -3;
		}
		else if (setsockopt(ts->ts_lsns[i], SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
			perror("init:setsockopt:lsns-reuseaddr");
			errors ++;
			(void) close(ts->ts_lsns[i]);
			ts->ts_lsns[i] = -3;
		}
		else if (setsockopt(ts->ts_lsns[i], SOL_SOCKET, SO_LINGER, &linger, sizeof(linger)) == -1) {
			perror("init:setsockopt:lsns-linger");
			errors ++;
			(void) close(ts->ts_lsns[i]);
			ts->ts_lsns[i] = -3;
		}
		else {
			int bret;
			do {
				(void) memset(&ts->ts_adds[i], 0,
					sizeof (struct sockaddr_in));
				ts->ts_adds[i].sin_family = AF_INET;
				ts->ts_adds[i].sin_port = htons(port++);
				(void) memcpy(&ts->ts_adds[i].sin_addr.s_addr,
					host->h_addr_list[0], sizeof (struct in_addr));

				bret = bind(ts->ts_lsns[i],
						(struct sockaddr *)&ts->ts_adds[i],
						sizeof (struct sockaddr_in));
				if (bret != 0) {
					if (errno != EADDRINUSE) {
						perror("init:bind");
						errors++;
						(void) close(ts->ts_lsns[i]);
						ts->ts_lsns[i] = -3;
					}
				}
			} while (bret == -1 && errno == EADDRINUSE);

			if (ts->ts_lsns[i] >= 0) {
				if (listen(ts->ts_lsns[i], 5) == -1) {
					perror("init:listen");
					errors++;
					(void) close(ts->ts_lsns[i]);
					ts->ts_lsns[i] = -3;
				}
			}
		}
	}
	return errors;
}

int
benchmark_initbatch(void *tsd)
{
	tsd_t		*ts = (tsd_t *)tsd;
	int			errors = 0;

	if (ts->ts_once++ == 0) {
		if (errors += benchmark_initbatch_once(tsd) != 0) {
			return errors;
		}
	}

	int	i;
	for (i = 0; i < lm_optB; i++) {
		ts->ts_cons[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (ts->ts_cons[i] == -1) {
			perror("init:socket");
			errors ++;
		}
		else if (fcntl(ts->ts_cons[i], F_SETFL, O_NDELAY) == -1) {
			perror("init:fcntl");
			errors ++;
			(void) close(ts->ts_cons[i]);
			ts->ts_cons[i] = -3;
		}
		else if (opta) {
			int	result;
			result = connect(ts->ts_cons[i],
				(struct sockaddr *)&ts->ts_adds[i],
				sizeof (struct sockaddr_in));
			if ((result == -1) && (errno != EINPROGRESS)) {
				perror("init:connect");
				errors ++;
				(void) close(ts->ts_cons[i]);
				ts->ts_cons[i] = -3;
			}
		}
	}

	return errors;
}

int
benchmark(void *tsd, result_t *res)
{
	tsd_t			*ts = (tsd_t *)tsd;
	int			i;
	int			result;
    int			aretries = 0;
    int		    cretries = 0;
    int			apolls = 0;
    int			cpolls = 0;

	for (i = 0; i < lm_optB; i++) {
		if (!opta && (ts->ts_cons[i] >= 0)) {
			int done = 0, error = 0;
			do {
				result = connect(ts->ts_cons[i],
					(struct sockaddr *)&ts->ts_adds[i],
					sizeof (struct sockaddr_in));
				if (result == 0) {
					done = 1;
				}
				else {
					if (errno != EINPROGRESS) {
						res->re_errors ++;
						perror("benchmark:connect");
						error = 1;
					}
					else {
						if (optc) {
							done = 1;
						}
						else {
                            cpolls++;
							struct pollfd pollfd;
							pollfd.fd = ts->ts_cons[i];
							pollfd.events = POLLOUT;
							int pollres = poll(&pollfd, 1, -1);
							if (pollres < 0) {
								res->re_errors ++;
								perror("benchmark:connect:poll");
								error = 1;
							}
							else if (pollres != 1) {
								fprintf(stderr, "benchmark:connect: poll() returned unexpected value: %d\n", pollres);
								res->re_errors ++;
								error = 1;
							}
							else {
								// Retry connect() attempt
                                cretries++;
							}
						}
					}
				}
			} while (!done && !error);

			if (optc || error)
				continue;
		}

		if (!optc && (ts->ts_lsns[i] >= 0)) {
			struct sockaddr_in	addr;
			socklen_t	size = sizeof (struct sockaddr);
			int pollres;
			do {
				pollres = 0;
				result = accept(ts->ts_lsns[i],
					(struct sockaddr *)&addr, &size);
				if (result < 0) {
					if (errno != EAGAIN) {
						res->re_errors ++;
						perror("benchmark:accept");
					}
					else {
						apolls++;
						struct pollfd pollfd;
						pollfd.fd = ts->ts_lsns[i];
						pollfd.events = POLLIN;
						pollres = poll(&pollfd, 1, -1);
                        if (pollres < 0) {
                            res->re_errors ++;
                            perror("benchmark:accept:poll");
                        }
						else if (pollres != 1) {
                            res->re_errors ++;
                            fprintf(stderr, "benchmark:accept: poll() returned unexpected value: %d\n", pollres);
						}
                        else {
                            // Retry accept() attempt
                            aretries++;
                        }
					}
				}
			} while (pollres == 1);

			ts->ts_accs[i] = result;
		}
	}

	res->re_count = i;

	return 0;
}

int
benchmark_finibatch(void *tsd)
{
	tsd_t		*ts = (tsd_t *)tsd;
	int			i;

	for (i = 0; i < lm_optB; i++) {
		if (ts->ts_accs[i] >= 0) {
			(void) close(ts->ts_accs[i]);
		}
		if (ts->ts_cons[i] >= 0) {
			(void) close(ts->ts_cons[i]);
		}
	}

	return 0;
}
