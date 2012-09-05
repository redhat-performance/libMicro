#ifndef _LIBC_
#define _LIBC_

//
// Obligatory null system call: use very short time
// for default since SuSe implements this "syscall" in userland
//
getpid	"getpid"
# if defined(MULTITHREADED)
getpid	"getpidT4"	-T 4
# endif
# if defined(MULTIPROCESS)
getpid	"getpidP4"	-P 4
# endif
# if defined(MULTITHREADED) && defined(MULTIPROCESS)
getpid	"getpidT4P4"	-T 4	-P 4
# endif

getenv	"getenv"	-s 100
# if defined(MULTITHREADED)
getenv	"getenvT2"	-T 2	-s 100
# endif

gettimeofday	"gettimeofday"

nanosleep	"nanosleep"
nanosleep	"nanosleep_r"	-r
# if defined(MULTITHREADED)
nanosleep	"nanosleepT4"	-T 4
nanosleep	"nanosleepT4_r"	-T 4	-r
# endif
nanosleep	"nanosleep_d10"	-d 10
nanosleep	"nanosleep_d100"	-d 100
nanosleep	"nanosleep_d1000"	-d 1000
# if defined(EXTENDED)
nanosleep	"nanosleep_d10000"	-d 10000
nanosleep	"nanosleep_d100000"	-d 100000
# endif

usleep	"usleep"
# if defined(MULTITHREADED)
usleep	"usleepT4"	-T 4
# endif

usleep	"usleep_d10"	-d 10
usleep	"usleep_d100"	-d 100

lrand48	"lrand48"

memset	"memset_10"	-s 10
memset	"memset_256"	-s 256
memset	"memset_256_u"	-s 256	-a 1
memset	"memset_1k"	-s 1k
memset	"memset_4k"	-s 4k
memset	"memset_4k_uc"	-s 4k	-u

memset	"memset_10k"	-s 10k
memset	"memset_1m"	-s 1m
memset	"memset_10m"	-s 10m
# if defined(MULTITHREADED)
memset	"memsetT2_10m"	-s 10m	-T 2
#endif
# if defined(MULTIPROCESS)
memset	"memsetP2_10m"	-s 10m	-P 2
#endif
# if defined(MULTIPROCESS) && defined(MULTITHREADED)
memset	"memsetP2T2_10m"	-s 10m	-P 2	-T 2
#endif

#include "memcpy.cpp"

#include "memmove.cpp"

isatty	"isatty_yes"
isatty	"isatty_no_badfd"	-b
isatty	"isatty_no"	-f $IFILE

malloc	"malloc_10"	-s 10	-g 10
malloc	"malloc_100"	-s 100	-g 10
malloc	"malloc_1k"	-s 1k	-g 10
malloc	"malloc_10k"	-s 10k	-g 10
malloc	"malloc_100k"	-s 100k	-g 10

# if defined(MULTITHREADED)
malloc	"mallocT2_10"	 	-s 10	-g 10	-T 2
malloc	"mallocT2_100"	 	-s 100 	-g 10	-T 2
malloc	"mallocT2_1k"	 	-s 1k	-g 10	-T 2
malloc	"mallocT2_10k"	 	-s 10k 	-g 10	-T 2
malloc	"mallocT2_100k" 	-s 100k	-g 10	-T 2
# endif

strcpy	"strcpy_10"	-s 10
strcpy	"strcpy_1k"	-s 1k

strlen	"strlen_10"	-s 10
strlen	"strlen_1k"	-s 1k

strchr	"strchr_10"	-s 10
strchr	"strchr_1k"	-s 1k

strcmp	"strcmp_10"	-s 10
strcmp	"strcmp_1k"	-s 1k

strcasecmp	"scasecmp_10"	-s 10
strcasecmp	"scasecmp_1k"	-s 1k

strtol	"strtol"
atoi	"atoi"

getcontext	"getcontext"
setcontext	"setcontext"

times	"times"
time	"time"
localtime_r	"localtime_r"
strftime	"strftime"

mktime	"mktime"
# if defined(MULTITHREADED)
mktime	"mktimeT2"	-T 2
# endif

longjmp	"longjmp"
siglongjmp	"siglongjmp"

getrusage	"getrusage"

sigaction	"sigaction"
signal	"signal"
sigprocmask	"sigprocmask"

#endif /* _LIBC_ */
