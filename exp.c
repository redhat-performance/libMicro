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

/*
 * exp[2|10][f|l]()
 */

/*
 * TODO:
 *   - should add range check
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern double exp10(double);
extern float exp10f(float);
extern long double exp10l(long double);

#include "libmicro.h"

static char	*optf = "exp";

int
benchmark_init(void)
{
	(void) snprintf(lm_optstr, sizeof(lm_optstr), "f:");

	(void) snprintf(lm_usage, sizeof(lm_usage),
            "\t[-f <exp func> (exp[fl],exp2[fl],exp10[fl], default %s)]\n"
			"note: measures exp[2|10][fl]() family of functions", optf);

	(void) snprintf(lm_header, sizeof(lm_header), "%8s", "func");

	lm_nsecs_per_op = 25;
	lm_tsdsize = 0;

	return 0;
}

int
benchmark_optswitch(int opt, char *optarg)
{
	switch (opt) {
	case 'f':
        if (0 == strcmp(optarg, "exp")
                || 0 == strcmp(optarg, "exp2")
                || 0 == strcmp(optarg, "exp10")
        		|| 0 == strcmp(optarg, "expf")
                || 0 == strcmp(optarg, "exp2f")
                || 0 == strcmp(optarg, "exp10f")
        		|| 0 == strcmp(optarg, "expl")
                || 0 == strcmp(optarg, "exp2l")
                || 0 == strcmp(optarg, "exp10l")) {
            optf = strdup(optarg);
            break;
        }
        // Else return bad argument
	default:
		return -1;
	}
	return 0;
}

#define UNROLL10(__func__, __type__, __i__) \
    __type__ value = 1.0 / (__i__ + 0.01); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value); \
	(void) __func__(value)

/*ARGSUSED*/
int
benchmark(void *tsd, result_t *res)
{
	int	i;

    if ((optf[3] == '\0') || (optf[3] == 'f') || (optf[3] == 'l')) {
        if (optf[3] == 'f') {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(expf, float, i);
    		}
        }
        else if (optf[3] == 'l') {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(expl, long double, i);
    		}
        }
        else {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(exp, double, i);
    		}
        }
    }
    else if (optf[3] == '2') {
        if (optf[4] == 'f') {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(exp2f, float, i);
    		}
        }
        else if (optf[4] == 'l') {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(exp2l, long double, i);
    		}
        }
        else {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(exp2, double, i);
    		}
        }
    }
    else {
        if (optf[5] == 'f') {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(exp10f, float, i);
    		}
        }
        else if (optf[5] == 'l') {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(exp10l, long double, i);
    		}
        }
        else {
    		for (i = 0; i < lm_optB; i += 10) {
    			UNROLL10(exp10, double, i);
    		}
        }
    }

	res->re_count = i;

	return 0;
}

char *
benchmark_result(void)
{
	static char	result[16];
	(void) snprintf(result, sizeof(result), "%8s", optf);

	return result;
}
