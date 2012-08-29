/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Modifications by Red Hat, Inc.
 */

#ifndef LIBMICRO_H
#define	LIBMICRO_H

#include <pthread.h>

#define	LIBMICRO_VERSION	"0.4.1-rh.25"

#define	STRSIZE	1024

typedef struct {
	long long	re_count;
	long long	re_errors;
	long long	re_t0;
	long long	re_t1;
} result_t;

typedef struct {
	long long	sum;
	long long	count;
} histo_t;

/*
 * stats we compute on data sets
 */

typedef struct stats {
	long long	st_min;
	long long	st_max;
	double		st_mean;
	long long	st_median;
	double		st_stddev;
	double		st_stderr;
	double		st_99confidence;
	double		st_skew;
	double		st_kurtosis;
	double		st_timecorr;	/* correlation with respect to time */
} stats_t;

#define KILLED_INT	1		/* Killed by INT, TERM, QUIT, etc. */
#define KILLED_LONG	2		/* Ran too long */

/*
 * Barrier stuff
 */

typedef struct {
	unsigned int	ba_size;		/* Size of allocated structure */
	int				ba_hwm;			/* barrier setpoint	*/
	int				ba_phase;		/* number of time used, benchmark while positive */
	int				ba_waiters;		/* how many are waiting	*/

#ifdef USE_SEMOP
	int				ba_semid;
#else
	pthread_mutex_t	ba_lock;
	pthread_cond_t	ba_cv;
#endif

	/* ^^^ Barrier data fields ^^^ */

	/* vvv Benchmarking data fields vvv */

	long long		ba_deadline;	/* when to hard stop */
	long long		ba_minruntime;	/* when to stop once sample count was
									 * reached */
	int				ba_killed;		/* why was run killed */

	long long		ba_count;		/* how many ops */
	long long		ba_errors;		/* how many errors */

	int				ba_quant;		/* how many quant errors */
	int				ba_batches;		/* how many samples collected, may be
									 * larger than ba_datasize below, since
									 * value can wrap. */
	int				ba_batches_final;	/* how many samples that fit into the
										 * data set size (see ba_datasize
										 * below) and after removing outliers. */

	double			ba_starttime;	/* test time start */
	double			ba_endtime;		/* test time end */

	stats_t			ba_raw;			/* raw stats */
	stats_t			ba_corrected;	/* corrected stats */

	int				ba_outliers;	/* outlier count, # of raw stats tossed */

	int				ba_datasize;	/* possible #items data	*/
	long long		ba_data[1];		/* start of data array	*/
} barrier_t;


/*
 * Barrier interfaces
 */

barrier_t *barrier_create(int, int);
int barrier_destroy(barrier_t *);
int barrier_queue(barrier_t *, result_t *);

/*
 * Functions that can be provided by the user
 */

int	benchmark(void *, result_t *);
int	benchmark_init(void);
int	benchmark_fini(void);
int	benchmark_initrun(void);
int	benchmark_finirun(void);
int	benchmark_initworker(void *);
int	benchmark_finiworker(void *);
int	benchmark_initbatch(void *);
int	benchmark_finibatch(void *);
int	benchmark_optswitch(int, char *);
char	*benchmark_result(void);


/*
 * Globals exported to the user
 */

extern int			lm_argc;
extern char		  **lm_argv;

extern int			lm_optB;
extern int			lm_optG;
extern int			lm_optP;
extern int			lm_optT;

extern int			lm_defB;
extern char		   *lm_defN;

extern int			lm_nsecs_per_op;

extern char		   *lm_procpath;
extern char			lm_procname[STRSIZE];
extern char			lm_usage[STRSIZE];
extern char			lm_optstr[STRSIZE];
extern char			lm_header[STRSIZE];
extern size_t		lm_tsdsize;


/*
 * Utility functions
 */

int				gettindex(void);
long long		getusecs(void);
long long		getnsecs(void);
void			setfdlimit(int);
long long		sizetoll(const char *);
int				sizetoint(const char *);
unsigned int	get_nsecs_resolution(void);
unsigned int	get_nsecs_overhead(void);

#endif /* LIBMICRO_H */
