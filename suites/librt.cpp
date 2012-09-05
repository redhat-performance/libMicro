#ifndef _LIBRT_
#define _LIBRT_

clock_gettime	"clock_gettime"

clock_nanosleep	"clock_nanosleep"
clock_nanosleep	"clock_nanosleep_s"	-s
clock_nanosleep	"clock_nanosleep_r"	-r
clock_nanosleep	"clock_nanosleep_sr"	-s	-r

clock_nanosleep	"clock_nanosleep_m"	-m
clock_nanosleep	"clock_nanosleep_ms"	-m	-s
clock_nanosleep	"clock_nanosleep_mr"	-m	-r
clock_nanosleep	"clock_nanosleep_msr"	-m	-s	-r

# if defined(MULTITHREADED)
clock_nanosleep	"clock_nanosleepT4"	-T 4
clock_nanosleep	"clock_nanosleepT4_s"	-T 4	-s
clock_nanosleep	"clock_nanosleepT4_r"	-T 4	-r
clock_nanosleep	"clock_nanosleepT4_sr"	-T 4	-s	-r
# endif

# if defined(MULTITHREADED)
clock_nanosleep	"clock_nanosleepT4_m"	-T 4	-m
clock_nanosleep	"clock_nanosleepT4_ms"	-T 4	-m	-s
clock_nanosleep	"clock_nanosleepT4_mr"	-T 4	-m	-r
clock_nanosleep	"clock_nanosleepT4_msr"	-T 4	-m	-s	-r
# endif

clock_nanosleep	"clock_nanosleep_d10"	-d 10
clock_nanosleep	"clock_nanosleep_d100"	-d 100
clock_nanosleep	"clock_nanosleep_d1000"	-d 1000
clock_nanosleep	"clock_nanosleep_d10000"	-d 10000
# if defined(EXTENDED)
clock_nanosleep	"clock_nanosleep_d100000"	-d 100000
# endif

#endif /* _LIBRT_ */
