#ifndef _LIBC_
#define _LIBC_

//
// Obligatory null system call: use very short time
// for default since SuSe implements this "syscall" in userland
//
getpid	"getpid"	-B 2m
# if defined(MULTITHREADED)
getpid	"getpidT4"	-B 2m	-T 4
# endif
# if defined(MULTIPROCESS)
getpid	"getpidP4"	-B 2m	-P 4
# endif
# if defined(MULTITHREADED) && defined(MULTIPROCESS)
getpid	"getpidT4P4"	-B 2m	-T 4	-P 4
# endif

getenv	"getenv"	-B 90000	-s 100
# if defined(MULTITHREADED)
getenv	"getenvT2"	-B 90000	-T 2	-s 100
# endif

gettimeofday	"gettimeofday"	-B 400000

nanosleep	"nanosleep"	-B 1500
nanosleep	"nanosleep_r"	-B 1500	-r
# if defined(MULTITHREADED)
nanosleep	"nanosleepT4"	-B 1500	-T 4
nanosleep	"nanosleepT4_r"	-B 1500	-T 4	-r
# endif
nanosleep	"nanosleep_d10"	-B 1500	-d 10
nanosleep	"nanosleep_d100"	-B 1500	-d 100
nanosleep	"nanosleep_d1000"	-B 1500	-d 1000
# if defined(EXTENDED)
nanosleep	"nanosleep_d10000"	-B 1000	-d 10000
nanosleep	"nanosleep_d100000"	-B 500	-d 100000
# endif

usleep	"usleep"	-B 1500
# if defined(MULTITHREADED)
usleep	"usleepT4"	-B 1500	-T 4
# endif

usleep	"usleep_d10"	-B 1500	-d 10
usleep	"usleep_d100"	-B 500	-d 100

lrand48	"lrand48"	-B 8000000

memset	"memset_10"	-s 10	-B 10m
memset	"memset_256"	-s 256	-B 5m
memset	"memset_256_u"	-s 256	-a 1	-B 5m
memset	"memset_1k"	-s 1k	-B 2m
memset	"memset_4k"	-s 4k	-B 700k
memset	"memset_4k_uc"	-s 4k	-u	-B 100k

memset	"memset_10k"	-s 10k
memset	"memset_1m"	-s 1m	-B 200
memset	"memset_10m"	-s 10m	-B 50
# if defined(MULTITHREADED)
memset	"memsetT2_10m"	-s 10m	-B 50	-T 2
#endif
# if defined(MULTIPROCESS)
memset	"memsetP2_10m"	-s 10m	-B 50	-P 2
#endif
# if defined(MULTIPROCESS) && defined(MULTITHREADED)
memset	"memsetP2T2_10m"	-s 10m	-B 50	-P 2	-T 2
#endif

#include "memcpy.cpp"

#include "memmove.cpp"

isatty	"isatty_yes"
isatty	"isatty_no_badfd"	-b
isatty	"isatty_no"	-f $IFILE

malloc	"malloc_10"	-s 10	-g 10	-B 10k
malloc	"malloc_100"	-s 100	-g 10	-B 10k
malloc	"malloc_1k"	-s 1k	-g 10	-B 10k
malloc	"malloc_10k"	-s 10k	-g 10	-B 10k
malloc	"malloc_100k"	-s 100k	-g 10	-B 1000

# if defined(MULTITHREADED)
malloc	"mallocT2_10"	 	-s 10	-g 10	-T 2	-B 16k
malloc	"mallocT2_100"	 	-s 100 	-g 10	-T 2	-B 16k
malloc	"mallocT2_1k"	 	-s 1k	-g 10	-T 2	-B 16k
malloc	"mallocT2_10k"	 	-s 10k 	-g 10	-T 2	-B 16k
malloc	"mallocT2_100k" 	-s 100k	-g 10	-T 2	-B 1K
# endif

strcpy	"strcpy_10"	-s 10	-B 10m
strcpy	"strcpy_1k"	-s 1k	-B 1m

strlen	"strlen_10"	-s 10	-B 15m
strlen	"strlen_1k"	-s 1k	-B 1m

strchr	"strchr_10"	-s 10	-B 18m
strchr	"strchr_1k"	-s 1k	-B 1m

strcmp	"strcmp_10"	-s 10	-B 14m
strcmp	"strcmp_1k"	-s 1k	-B 1m

strcasecmp	"scasecmp_10"	-s 10	-B 5m
strcasecmp	"scasecmp_1k"	-s 1k

strtol	"strtol"	-B 1m
atoi	"atoi"	-B 1m

getcontext	"getcontext"
setcontext	"setcontext"

times	"times"	-B 400k
time	"time"	-B 2m
localtime_r	"localtime_r"
strftime	"strftime"

mktime	"mktime"	-B 10k	
# if defined(MULTITHREADED)
mktime	"mktimeT2"	-T 2	-B 10k
# endif

longjmp	"longjmp"	-B 5m
siglongjmp	"siglongjmp"	-B 500k

getrusage	"getrusage"

sigaction	"sigaction"
signal	"signal"	-B 50k
sigprocmask	"sigprocmask"

#endif /* _LIBC_ */
