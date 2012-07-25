#ifndef _LIBC_
#define _LIBC_

//
// Obligatory null system call: use very short time
// for default since SuSe implements this "syscall" in userland
//
getpid	"getpid"	-B 640000
getpid	"getpidT4"	-B 640000	-T 4

getenv	"getenv"	-B 90000	-s 100
getenv	"getenvT2"	-B 90000	-T 2	-s 100

gettimeofday	"gettimeofday"	-B 400000

nanosleep	"nanosleep"	-B 1500
nanosleep	"nanosleep_r"	-B 1500	-r

nanosleep	"nanosleepT4"	-B 1500	-T 4
nanosleep	"nanosleepT4_r"	-B 1500	-T 4	-r

nanosleep	"nanosleep_d10"	-B 1500	-d 10
nanosleep	"nanosleep_d100"	-B 1500	-d 100
nanosleep	"nanosleep_d1000"	-B 1500	-d 1000
nanosleep	"nanosleep_d10000"	-B 1000	-d 10000
nanosleep	"nanosleep_d100000"	-B 1000	-d 100000

lrand48	"lrand48"	-B 8000000

memset	"memset_10"	-s 10	-B 10m
memset	"memset_256"	-s 256	-B 5m
memset	"memset_256_u"	-s 256	-a 1	-B 5m
memset	"memset_1k"	-s 1k	-B 2m
memset	"memset_4k"	-s 4k	-B 700k
memset	"memset_4k_uc"	-s 4k	-u	-B 100k

memset	"memset_10k"	-s 10k
memset	"memset_1m"	-s 1m	-B 200
memset	"memset_10m"	-s 10m	-B 100
memset	"memsetP2_10m"	-s 10m	-B 100	-P 2

#include "memcpy.cpp"

memmove	"memmove_10"	-s 10
memmove	"memmove_1k"	-s 1k
memmove	"memmove_10k"	-s 10k
memmove	"memmove_1m"	-s 1m	-B 100
memmove	"memmove_10m"	-s 10m	-B 50

isatty	"isatty_yes"
isatty	"isatty_no" 	-f $IFILE

malloc	"malloc_10"	-s 10	-g 10	-B 1000
malloc	"malloc_100"	-s 100	-g 10	-B 1000
malloc	"malloc_1k"	-s 1k	-g 10	-B 1000
malloc	"malloc_10k"	-s 10k	-g 10	-B 1000
malloc	"malloc_100k"	-s 100k	-g 10	-B 1000

malloc	"mallocT2_10"	 	-s 10	 	-g 10	-T 2	-B 1000
malloc	"mallocT2_100"	 	-s 100 	-g 10	-T 2	-B 1000
malloc	"mallocT2_1k"	 	-s 1k	 	-g 10	-T 2	-B 1000
malloc	"mallocT2_10k"	 	-s 10k 	-g 10	-T 2	-B 1000
malloc	"mallocT2_100k" 	-s 100k	-g 10	-T 2	-B 1000

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

times	"times"	-B 800k
time	"time"	-B 2m
localtime_r	"localtime_r"
strftime	"strftime"

mktime	"mktime"	-B 10k	
mktime	"mktimeT2"	-T 2	-B 10k

longjmp	"longjmp"	-B 5m
siglongjmp	"siglongjmp"	-B 800k

getrusage	"getrusage"

sigaction	"sigaction"
signal	"signal"
sigprocmask	"sigprocmask"

#endif /* _LIBC_ */
