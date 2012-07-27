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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "libmicro.h"

typedef struct {
	int			ts_once;
	pid_t		ts_child;
	pthread_t	ts_thread;
	int			ts_in;
	int			ts_out;
	int			ts_in2;
	int			ts_out2;
	int			ts_lsn;
	struct sockaddr_in	ts_add;
} tsd_t;

#define	FIRSTPORT		12345

static char			*modes[] = {"st", "mt", "mp", NULL};
#define	MD_SINGLE		0
#define	MD_MULTITHREAD	1
#define	MD_MULTIPROCESS	2

static char			*xports[] = {"pipe", "fifo", "sock", "tcp", NULL};
#define	XP_PIPES		0
#define	XP_FIFOS		1
#define	XP_SOCKETPAIR	2
#define	XP_LOCALTCP		3

#define	DEFM			MD_SINGLE
#define	DEFS			1024
#define	DEFX			XP_PIPES

static int			optm = DEFM;
static size_t		opts = DEFS;
static int			optx = DEFX;
static void		   *rbuf = NULL;
static void		   *wbuf = NULL;

static int readall(int, void *, size_t);
static void *loopback(void *);
static int prepare_pipes(tsd_t *);
static int prepare_fifos(tsd_t *);
static int cleanup_fifos(void);
static int prepare_socketpair(tsd_t *);
static int prepare_localtcp(tsd_t *);
static int prepare_localtcp_once(tsd_t *);
static char *lookupa(int, char *[]);
static int lookup(char *, char *[]);

int
benchmark_init(void)
{
	lm_tsdsize = sizeof (tsd_t);

	(void) snprintf(lm_optstr, sizeof(lm_optstr), "m:s:x:");

	(void) snprintf(lm_usage, sizeof(lm_usage),
		"\t[-m mode (st|mt|mp, default %s)]\n"
		"\t[-s buffer-size (default %d)]\n"
		"\t[-x transport (pipe|fifo|sock|tcp, default %s)]\n"
		"notes: measures write()/read() across various transports\n",
		lookupa(DEFM, modes), DEFS, lookupa(DEFX, xports));

	(void) snprintf(lm_header, sizeof(lm_header), "%2s %4s", "md", "xprt");

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	int			x;

	switch (opt) {
	case 'm':
		x = lookup(optarg, modes);
		if (x == -1)
			return -1;
		optm = x;
		break;
	case 's':
		opts = sizetoll(optarg);
		break;
	case 'x':
		x = lookup(optarg, xports);
		if (x == -1)
			return -1;
		optx = x;
		break;
	default:
		return -1;
	}
	return 0;
}

int
benchmark_initrun(void)
{
	setfdlimit(4 * lm_optT + 10);

	rbuf = malloc(opts);
	if (rbuf == NULL) {
		perror("malloc");
		return -1;
	}
	wbuf = malloc(opts);
	if (wbuf == NULL) {
		perror("malloc");
		return -1;
	}

	return 0;
}

int
benchmark_initbatch(void *tsd)
{
	tsd_t  *ts = (tsd_t *)tsd;
	int		result;
	pid_t	pid;
	int		i;

	switch (optx) {
	case XP_SOCKETPAIR:
		result = prepare_socketpair(ts);
		break;
	case XP_LOCALTCP:
		result = prepare_localtcp(ts);
		break;
	case XP_FIFOS:
		result = prepare_fifos(ts);
		break;
	case XP_PIPES:
	default:
		result = prepare_pipes(ts);
		break;
	}
	if (result == -1) {
		return 1;
	}

	switch (optm) {
	case MD_MULTITHREAD:
		result = pthread_create(&ts->ts_thread, NULL, loopback, tsd);
		if (result != 0) {
			return 1;
		}
		break;
	case MD_MULTIPROCESS:
		pid = fork();
		switch (pid) {
		case 0:
			(void) loopback(tsd);
			exit(0);
			break;
		case -1:
			perror("fork");
			return 1;
		default:
			ts->ts_child = pid;
			break;
		}
		break;
	case MD_SINGLE:
	default:
		break;
	}

	/* Prime the loopback */
	if (write(ts->ts_out, wbuf, opts) != opts) {
		return 1;
	}
	if (readall(ts->ts_in, rbuf, opts) != opts) {
		return 1;
	}

	return 0;
}

int
benchmark(void *tsd, result_t *res)
{
	tsd_t  *ts = (tsd_t *)tsd;
	int		i, n, ret;

	for (i = 0; i < lm_optB; i++) {
		ret = write(ts->ts_out, wbuf, opts);
		if (ret != opts) {
			if (ret < 0) {
				perror("write");
			}
			res->re_errors++;
			continue;
		}

		n = readall(ts->ts_in, rbuf, opts);
		if (n == -1) {
			res->re_errors++;
			continue;
		}
	}
	res->re_count = i;

	return 0;
}

int
benchmark_finibatch(void *tsd)
{
	tsd_t  *ts = (tsd_t *)tsd;

	/* Terminate the loopback */
	(void) write(ts->ts_out, wbuf, opts);
	(void) readall(ts->ts_in, rbuf, opts);

	switch (optm) {
	case MD_MULTITHREAD:
		(void) close(ts->ts_in2);
		(void) close(ts->ts_out2);
		(void) pthread_join(ts->ts_thread, NULL);
		break;
	case MD_MULTIPROCESS:
		(void) close(ts->ts_in2);
		(void) close(ts->ts_out2);
		(void) waitpid(ts->ts_child, NULL, 0);
		break;
	case MD_SINGLE:
	default:
		break;
	}

	(void) close(ts->ts_in);
	(void) close(ts->ts_out);

	if (optx == XP_FIFOS) {
		(void) cleanup_fifos();
	}

	return 0;
}

char *
benchmark_result(void)
{
	static char		result[256];

	(void) snprintf(result, sizeof(result), "%2s %4s",
		lookupa(optm, modes), lookupa(optx, xports));

	return result;
}

static int
readall(int s, void *buf, size_t len)
{
	size_t	n;
	size_t	total = 0;

	for (;;) {
		n = read(s, (void *)((long)buf + total), len - total);
		if (n < 1) {
			if (n < 0) {
				perror("read");
			}
			return -1;
		}
		total += n;
		if (total >= len) {
			return total;
		}
	}
}

static void *
loopback(void *arg)
{
	tsd_t  *ts = (tsd_t *)arg;
	int		i, n, m;

	/* Include priming and termination */
	m = lm_optB + 2;

	for (i = 0; i < m; i++) {
		n = readall(ts->ts_in2, rbuf, opts);
		if (n == -1) {
			break;
		}
		if (write(ts->ts_out2, wbuf, opts) != opts) {
			break;
		}
	}

	return NULL;
}

static int
prepare_localtcp_once(tsd_t *ts)
{
	int				j;
	int				opt = 1;
	struct hostent *host;

	j = FIRSTPORT;

	ts->ts_lsn = socket(AF_INET, SOCK_STREAM, 0);
	if (ts->ts_lsn == -1) {
		perror("socket");
		return -1;
	}

	if (setsockopt(ts->ts_lsn, SOL_SOCKET, SO_REUSEADDR,
			&opt, sizeof (int)) == -1) {
		perror("setsockopt");
		return -1;
	}

	if ((host = gethostbyname("localhost")) == NULL) {
		perror("gethostbyname");
		return -1;
	}

	for (;;) {
		(void) memset(&ts->ts_add, 0,
				sizeof (struct sockaddr_in));
		ts->ts_add.sin_family = AF_INET;
		ts->ts_add.sin_port = htons(j++);
		(void) memcpy(&ts->ts_add.sin_addr.s_addr,
				host->h_addr_list[0], sizeof (struct in_addr));

		if (bind(ts->ts_lsn,
				(struct sockaddr *)&ts->ts_add,
				sizeof (struct sockaddr_in)) == 0) {
			break;
		}

		if (errno != EADDRINUSE) {
			perror("bind");
			return -1;
		}
	}

	if (listen(ts->ts_lsn, 5) == -1) {
		perror("listen");
		return -1;
	}

	return 0;
}

static int
prepare_localtcp(tsd_t *ts)
{
	int					result;
	struct sockaddr_in	addr;
	int					opt = 1;
	socklen_t			size;

	if (ts->ts_once++ == 0) {
		if (prepare_localtcp_once(ts) == -1) {
			return -1;
		}
	}

	ts->ts_out = socket(AF_INET, SOCK_STREAM, 0);
	if (ts->ts_out == -1) {
		perror("socket");
		return -1;
	}

	if (fcntl(ts->ts_out, F_SETFL, O_NDELAY) == -1) {
		perror("fcntl");
		return -1;
	}

	result = connect(ts->ts_out, (struct sockaddr *)&ts->ts_add,
			sizeof (struct sockaddr_in));
	if ((result == -1) && (errno != EINPROGRESS)) {
		perror("connect");
		return -1;
	}

	if (fcntl(ts->ts_out, F_SETFL, 0) == -1) {
		perror("fcntl");
		return -1;
	}

	size = sizeof (struct sockaddr);
	result = accept(ts->ts_lsn, (struct sockaddr *)&addr, &size);
	if (result == -1) {
		perror("accept");
		return -1;
	}
	ts->ts_out2 = result;

	if (setsockopt(ts->ts_out, IPPROTO_TCP, TCP_NODELAY,
			&opt, sizeof (int)) == -1) {
		perror("setsockopt");
		return -1;
	}

	if (setsockopt(ts->ts_out2, IPPROTO_TCP, TCP_NODELAY,
			&opt, sizeof (int)) == -1) {
		perror("setsockopt");
		return -1;
	}

	if (optm == MD_SINGLE) {
		ts->ts_in = ts->ts_out2;
	} else {
		ts->ts_in = ts->ts_out;
		ts->ts_in2 = ts->ts_out2;
	}

	return 0;
}

static int
prepare_socketpair(tsd_t *ts)
{
	int			s[2];

	if (socketpair(PF_UNIX, SOCK_STREAM, 0, s) == -1) {
		perror("socketpair");
		return -1;
	}

	if (optm == MD_SINGLE) {
		ts->ts_in = s[0];
		ts->ts_out = s[1];
	} else {
		ts->ts_in = s[0];
		ts->ts_out = s[0];
		ts->ts_in2 = s[1];
		ts->ts_out2 = s[1];
	}

	return 0;
}

static int
prepare_fifos(tsd_t *ts)
{
	char	pathA[64];
	char	pathB[64], *pathBp = NULL;

	(void) snprintf(pathA, sizeof(pathA), "/tmp/pipe_%ld_0x%lx_A",
			getpid(), pthread_self());
	if (mkfifo(pathA, 0600) == -1) {
		perror("mkfifo");
		return -1;
	}

	ts->ts_in = open(pathA, O_RDONLY|O_NONBLOCK);
	if (ts->ts_in == -1) {
		perror("prepare_fifos(): open");
		goto error;
	}

	int out = open(pathA, O_WRONLY|O_NONBLOCK);
	if (out == -1) {
		perror("prepare_fifos(): open");
		goto error;
	}

	if (optm == MD_SINGLE) {
		ts->ts_out = out;
	} else {
		ts->ts_out2 = out;

		(void) snprintf(pathB, sizeof(pathB), "/tmp/pipe_%ld_0x%lx_B",
				getpid(), pthread_self());
		pathBp = pathB;
		if (mkfifo(pathBp, 0600) == -1) {
			perror("mkfifo");
			goto error;
		}

		ts->ts_in2 = open(pathBp, O_RDONLY|O_NONBLOCK);
		if (ts->ts_in2 == -1) {
			perror("prepare_fifos(): open");
			goto error;
		}
		ts->ts_out = open(pathBp, O_WRONLY|O_NONBLOCK);
		if (ts->ts_in2 == -1) {
			perror("prepare_fifos(): open");
			goto error;
		}
	}

	if (fcntl(ts->ts_out2, F_SETFL, 0) == -1) {
		perror("fcntl");
		goto error;
	}
	if (fcntl(ts->ts_in2, F_SETFL, 0) == -1) {
		perror("fcntl");
		goto error;
	}
	if (fcntl(ts->ts_out, F_SETFL, 0) == -1) {
		perror("fcntl");
		goto error;
	}
	if (fcntl(ts->ts_in, F_SETFL, 0) == -1) {
		perror("fcntl");
		goto error;
	}

	return 0;

error:
	if (ts->ts_out2 > 0) (void) close(ts->ts_out2), ts->ts_out2 = 0;
	if (ts->ts_in2 > 0) (void) close(ts->ts_in2), ts->ts_in2 = 0;
	if (ts->ts_out > 0) (void) close(ts->ts_out), ts->ts_out = 0;
	if (ts->ts_in > 0) (void) close(ts->ts_in), ts->ts_in = 0;
	if (pathBp) (void) unlink(pathBp);
	(void) unlink(pathA);
	return -1;
}

static int
cleanup_fifos(void)
{
	char	path[64];

	(void) snprintf(path, sizeof(path), "/tmp/pipe_%ld_0x%lx_A", getpid(), pthread_self());
	(void) unlink(path);
	(void) snprintf(path, sizeof(path), "/tmp/pipe_%ld_0x%lx_B", getpid(), pthread_self());
	(void) unlink(path);

	return 0;
}

static int
prepare_pipes(tsd_t *ts)
{
	int		p[2];

	if (optm == MD_SINGLE) {
		if (pipe(p) == -1) {
			perror("pipe");
			return -1;
		}
		ts->ts_in = p[0];
		ts->ts_out = p[1];

	} else {
		if (pipe(p) == -1) {
			perror("pipe");
			return -1;
		}
		ts->ts_in = p[0];
		ts->ts_out2 = p[1];

		if (pipe(p) == -1) {
			perror("pipe");
			return -1;
		}
		ts->ts_in2 = p[0];
		ts->ts_out = p[1];
	}

	return 0;
}

static char *
lookupa(int x, char *names[])
{
	int		i = 0;

	while (names[i] != NULL) {
		if (x == i) {
			return names[i];
		}
		i++;
	}
	return NULL;
}

static int
lookup(char *x, char *names[])
{
	int		i = 0;

	while (names[i] != NULL) {
		if (strcmp(names[i], x) == 0) {
			return i;
		}
		i++;
	}
	return -1;
}
