getpid	"getpidT4"	-B 640000	-T 4
getpid	"getpidT4_s"	-B 32000	-T 4	-s

sched_yield	"sched_yield"	-B 32000
sched_yield	"sched_yield_s"	-B 32000	-s
sched_yield	"sched_yieldT4"	-B 32000	-T 4
sched_yield	"sched_yieldT4_s"	-B 32000	-T 4	-s

getenv	"getenvT2"	-B 90000	-T 2	-s 100

nanosleep	"nanosleepT4"	-B 1500	-T 4
nanosleep	"nanosleepT4_s"	-B 1500	-T 4	-s
nanosleep	"nanosleepT4_r"	-B 1500	-T 4	-r
nanosleep	"nanosleepT4_rs"	-B 1500	-T 4	-s	-r

clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-s
clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4	-r
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-s	-r

clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4	-m
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-m	-s
clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4	-m	-r
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-m	-s	-r

malloc	"mallocT2_10"	 	-s 10	 	-g 10	-T 2	-B 1000
malloc	"mallocT2_100"	 	-s 100 	-g 10	-T 2	-B 1000
malloc	"mallocT2_1k"	 	-s 1k	 	-g 10	-T 2	-B 1000
malloc	"mallocT2_10k"	 	-s 10k 	-g 10	-T 2	-B 1000
malloc	"mallocT2_100k" 	-s 100k	-g 10	-T 2	-B 1000

getcontext	"getcontext"
setcontext	"setcontext"

mutex	"mutex_st"	-B 5m
mutex	"mutex_mt"	-t	-B 5m
mutex	"mutex_T2"	-T 2	-B 1m

longjmp	"longjmp"	-B 5m
siglongjmp	"siglongjmp"	-B 800k

mktime	"mktimeT2"	-T 2	-B 10k

cascade_mutex	"c_mutex_1"
cascade_mutex	"c_mutex_10"	-T 10	-B 100
cascade_mutex	"c_mutex_200"	-T 200	-B 100

cascade_cond	"c_cond_1"
cascade_cond	"c_cond_10"	-T 10	-B 100
cascade_cond	"c_cond_200"	-T 200	-B 100

pthread_create	"pthread_32"	-B 32
pthread_create	"pthread_64"	-B 64
pthread_create	"pthread_128"	-B 128
pthread_create	"pthread_256"	-B 256
pthread_create	"pthread_512"	-B 512
