memrand	"memrand_1k"	-s 1k	-B 32m
memrand	"memrand_2k"	-s 2k	-B 32m
memrand	"memrand_4k"	-s 4k	-B 32m
memrand	"memrand_8k"	-s 8k	-B 32m
memrand	"memrand_16k"	-s 16k	-B 32m
memrand	"memrand_32k"	-s 32k	-B 32m
memrand	"memrand_64k"	-s 64k	-B 24m
memrand	"memrand_128k"	-s 128k	-B 24m
memrand	"memrand_256k"	-s 256k	-B 24m
memrand	"memrand_512k"	-s 512k	-B 24m
memrand	"memrand_1m"	-s 1m	-B 24m
memrand	"memrand_2m"	-s 2m	-B 24m
memrand	"memrand_4m"	-s 4m	-B 24m
memrand	"memrand_8m"	-s 8m	-B 24m
memrand	"memrand_16m"	-s 16m	-B 24m
memrand	"memrand_32m"	-s 32m	-B 24m
memrand	"memrand_64m"	-s 64m	-B 16m
memrand	"memrand"	-s 128m	-B 32m
memrand	"memrand_256m"	-s 256m	-B 64m
memrand	"memrand_512m"	-s 512m	-B 128m
memrand	"memrand_1g"	-s 1g	-B 256m
memrand	"memrand_2g"	-s 2g	-B 512m

recurse	"recurse"	-B 200k
recurse	"recurse_1k"	-d 1k	-B 10k
recurse	"recurse_10k"	-d 10k	-B 1k

//
// Obligatory null system call: use very short time
// for default since SuSe implements this "syscall" in userland
//
getpid	"getpid"	-B 640000
getpid	"getpid_s"	-B 32000	-s
getpid	"getpidT4"	-B 640000	-T 4
getpid	"getpidT4_s"	-B 32000	-T 4	-s

sched_yield	"sched_yield"	-B 32000
sched_yield	"sched_yield_s"	-B 32000	-s
sched_yield	"sched_yieldT4"	-B 32000	-T 4
sched_yield	"sched_yieldT4_s"	-B 32000	-T 4	-s

getenv	"getenv"	-B 90000	-s 100
getenv	"getenvT2"	-B 90000	-T 2	-s 100

gettimeofday	"gettimeofday"	-B 400000

clock_gettime	"clock_gettime"	-B 400000

nanosleep	"nanosleep"	-B 1500
nanosleep	"nanosleep_s"	-B 1500	-s
nanosleep	"nanosleep_r"	-B 1500	-r
nanosleep	"nanosleep_rs"	-B 1500	-s	-r

nanosleep	"nanosleepT4"	-B 1500	-T 4
nanosleep	"nanosleepT4_s"	-B 1500	-T 4	-s
nanosleep	"nanosleepT4_r"	-B 1500	-T 4	-r
nanosleep	"nanosleepT4_rs"	-B 1500	-T 4	-s	-r

nanosleep	"nanosleep_d10"	-B 1500	-d 10
nanosleep	"nanosleep_d100"	-B 1500	-d 100
nanosleep	"nanosleep_d1000"	-B 1500	-d 1000
nanosleep	"nanosleep_d10000"	-B 1000	-d 10000
nanosleep	"nanosleep_d100000"	-B 1000	-d 100000

clock_nanosleep	"clock_nanosleep"	-B 1500
clock_nanosleep	"clock_nanosleep_s"	-B 1500	-s
clock_nanosleep	"clock_nanosleep_r"	-B 1500	-r
clock_nanosleep	"clock_nanosleep_rs"	-B 1500	-s	-r

clock_nanosleep	"clock_nanosleep"	-B 1500	-m
clock_nanosleep	"clock_nanosleep_s"	-B 1500	-m	-s
clock_nanosleep	"clock_nanosleep_r"	-B 1500	-m	-r
clock_nanosleep	"clock_nanosleep_rs"	-B 1500	-m	-s	-r

clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-s
clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4	-r
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-s	-r

clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4	-m
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-m	-s
clock_nanosleep	"clock_nanosleepT4"	-B 1500	-T 4	-m	-r
clock_nanosleep	"clock_nanosleepT4_s"	-B 1500	-T 4	-m	-s	-r

clock_nanosleep	"clock_nanosleep_d10"	-B 1500	-d 10
clock_nanosleep	"clock_nanosleep_d100"	-B 1500	-d 100
clock_nanosleep	"clock_nanosleep_d1000"	-B 1500	-d 1000
clock_nanosleep	"clock_nanosleep_d10000"	-B 1000	-d 10000
clock_nanosleep	"clock_nanosleep_d100000"	-B 1000	-d 100000

log	"log"	-B 2000000
exp	"exp"	-B 2000000

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

memcpy	"memcpy_10"	-s 10
memcpy	"memcpy_1k"	-s 1k
memcpy	"memcpy_10k"	-s 10k
memcpy	"memcpy_1m"	-s 1m	-B 100
memcpy	"memcpy_10m"	-s 10m	-B 50

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

close	"close_bad"	-B 1m	-b
close	"close_tmp"	-B 640	-f $TFILE
close	"close_usr"	-B 640	-f $VFILE
close	"close_zero"	-B 640	-f /dev/zero

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

mutex	"mutex_st"	-B 5m
mutex	"mutex_mt"	-t	-B 5m
mutex	"mutex_T2"	-T 2	-B 1m

longjmp	"longjmp"	-B 5m
siglongjmp	"siglongjmp"	-B 800k

getrusage	"getrusage"

times	"times"	-B 800k
time	"time"	-B 2m
localtime_r	"localtime_r"
strftime	"strftime"

mktime	"mktime"	-B 10k	
mktime	"mktimeT2"	-T 2	-B 10k

cascade_mutex	"c_mutex_1"
cascade_mutex	"c_mutex_10"	-T 10	-B 100
cascade_mutex	"c_mutex_200"	-T 200	-B 100

cascade_cond	"c_cond_1"
cascade_cond	"c_cond_10"	-T 10	-B 100
cascade_cond	"c_cond_200"	-T 200	-B 100

cascade_lockf	"c_lockf_1"
cascade_lockf	"c_lockf_10"	-P 10	-B 100
cascade_lockf	"c_lockf_200"	-P 200	-B 100

cascade_flock	"c_flock"
cascade_flock	"c_flock_10"	-P 10	-B 100
cascade_flock	"c_flock_200"	-P 200	-B 100

cascade_fcntl	"c_fcntl_1"	
cascade_fcntl	"c_fcntl_10"	-P 10	-B 100
cascade_fcntl	"c_fcntl_200"	-P 200	-B 10

file_lock	"file_lock"

getsockname	"getsockname"
getpeername	"getpeername"

chdir	"chdir_tmp"	-B 10k	$TDIR1 $TDIR2
chdir	"chdir_usr"	-B 10k	$VDIR1 $VDIR2

chdir	"chgetwd_tmp"	-B 10k	-g $TDIR1 $TDIR2
chdir	"chgetwd_usr"	-B 10k	-g $VDIR1 $VDIR2

realpath	"realpath_tmp"	-f $TDIR1	-B 7000
realpath	"realpath_usr"	-f $VDIR1	-B 7000

stat	"stat_tmp"	-f $TFILE
stat	"stat_usr"	-f $VFILE

fcntl	"fcntl_tmp"	-f $TFILE
fcntl	"fcntl_usr"	-f $VFILE
fcntl_ndelay	"fcntl_ndelay"

lseek	"lseek_t8k"	-s 8k	-f $TFILE
lseek	"lseek_u8k"	-s 8k	-f $VFILE

open	"open_tmp"	-B 25000	-f $TFILE
open	"open_usr"	-B 25000	-f $VFILE
open	"open_zero"	-B 25000	-f /dev/zero

dup	"dup"	-B 400k

socket	"socket_u"	-B 10k
socket	"socket_i"	-B 10k	-f PF_INET

socketpair	"socketpair"	-B 25000

setsockopt	"setsockopt"	

bind	"bind"	-B 200

listen	"listen"	-B 50k

connection	"connection"	-B 512
connection	"conn_connect"	-B 512	-c
connection	"conn_accept"	-B 512	-a

close_tcp	"close_tcp"	-B 1k

poll	"poll_10"	-n 10
poll	"poll_100"	-n 100	-B 10k
poll	"poll_1000"	-n 1000	-B 1k

poll	"poll_w10"	-n 10	-w 1
poll	"poll_w100"	-n 100	-w 10	-B 10k
poll	"poll_w1000"	-n 1000	-w 100	-B 1k

select	"select_10"	-n 10
select	"select_100"	-n 100	-B 10k
select	"select_1000"	-n 1000	-B 1k

select	"select_w10"	-n 10	-w 1
select	"select_w100"	-n 100	-w 10	-B 10k
select	"select_w1000"	-n 1000	-w 100	-B 1k

semop	"semop"

sigaction	"sigaction"
signal	"signal"
sigprocmask	"sigprocmask"

pthread_create	"pthread_32"	-B 32
pthread_create	"pthread_64"	-B 64
pthread_create	"pthread_128"	-B 128
pthread_create	"pthread_256"	-B 256
pthread_create	"pthread_512"	-B 512

fork	"fork_100"	-B 100
fork	"fork_1000"	-B 1000

exit	"exit_100"	-B 100
exit	"exit_100_nolibc"	-e	-B 100
exit	"exit_1000"	-B 1000
exit	"exit_1000_nolibc"	-e	-B 1000

exec	"exec"	-B 100

execw	"execw"	-B 250

execw	"execw_1r"	-B 250	-r 1
execw	"execw_10r"	-B 250	-r 10
execw	"execw_100r"	-B 125	-r 100
execw	"execw_1000r"	-B 25	-r 1000

execw	"execw_32n"	-B 250	-n 32	-p 8192
execw	"execw_64n"	-B 250	-n 64	-p 4096
execw	"execw_128n"	-B 250	-n 128	-p 2048
execw	"execw_256n"	-B 250	-n 256	-p 1024
execw	"execw_512n"	-B 250	-n 512	-p 512
execw	"execw_1024n"	-B 250	-n 1024	-p 256
execw	"execw_2048n"	-B 250	-n 2048	-p 128
execw	"execw_4096n"	-B 250	-n 4096	-p 64
execw	"execw_8192n"	-B 250	-n 8192	-p 32

execw	"execw_8192n1000r"	-B 25	-n 8192	-p 32	-r 1000

execw	"execw_32nt"	-B 100	-n 32	-p 8192	-t
execw	"execw_64nt"	-B 50	-n 64	-p 4096	-t
execw	"execw_128nt"	-B 25	-n 128	-p 2048	-t

execw	"execw_32ntv"	-B 500	-n 32	-p 8192	-t	-v
execw	"execw_64ntv"	-B 500	-n 64	-p 4096	-t	-v
execw	"execw_128ntv"	-B 500	-n 128	-p 2048	-t	-v
execw	"execw_256ntv"	-B 500	-n 256	-p 1024	-t	-v
execw	"execw_512ntv"	-B 500	-n 512	-p 512	-t	-v
execw	"execw_1024ntv"	-B 500	-n 1024	-p 256	-t	-v
execw	"execw_2048ntv"	-B 500	-n 2048	-p 128	-t	-v
execw	"execw_4096ntv"	-B 500	-n 4096	-p 64	-t	-v
execw	"execw_8192ntv"	-B 500	-n 8192	-p 32	-t	-v

execw	"execw_8192ntv1000r"	-B 500	-n 8192	-p 32	-t	-v	-r 1000

posix_spawn	"posix_spawn"	-B 500

posix_spawn	"posix_spawn_1r"	-B 500	-r 1
posix_spawn	"posix_spawn_10r"	-B 500	-r 10
posix_spawn	"posix_spawn_100r"	-B 500	-r 100
posix_spawn	"posix_spawn_1000r"	-B 500	-r 1000

posix_spawn	"posix_spawn_32n"	-B 500	-n 32	-p 8192
posix_spawn	"posix_spawn_64n"	-B 500	-n 64	-p 4096
posix_spawn	"posix_spawn_128n"	-B 500	-n 128	-p 2048
posix_spawn	"posix_spawn_256n"	-B 500	-n 256	-p 1024
posix_spawn	"posix_spawn_512n"	-B 500	-n 512	-p 512
posix_spawn	"posix_spawn_1024n"	-B 500	-n 1024	-p 256
posix_spawn	"posix_spawn_2048n"	-B 500	-n 2048	-p 128
posix_spawn	"posix_spawn_4096n"	-B 500	-n 4096	-p 64
posix_spawn	"posix_spawn_8192n"	-B 500	-n 8192	-p 32

posix_spawn	"posix_spawn_8192n1000r"	-B 500	-n 8192	-p 32	-r 1000

posix_spawn	"posix_spawn_32nt"	-B 500	-n 32	-p 8192	-t
posix_spawn	"posix_spawn_64nt"	-B 500	-n 64	-p 4096	-t
posix_spawn	"posix_spawn_128nt"	-B 500	-n 128	-p 2048	-t
posix_spawn	"posix_spawn_256nt"	-B 500	-n 256	-p 1024	-t
posix_spawn	"posix_spawn_512nt"	-B 500	-n 512	-p 512	-t
posix_spawn	"posix_spawn_1024nt"	-B 500	-n 1024	-p 256	-t
posix_spawn	"posix_spawn_2048nt"	-B 500	-n 2048	-p 128	-t
posix_spawn	"posix_spawn_4096nt"	-B 500	-n 4096	-p 64	-t
posix_spawn	"posix_spawn_8192nt"	-B 500	-n 8192	-p 32	-t

posix_spawn	"posix_spawn_8192nt1000r"	-B 500	-n 8192	-p 32	-t	-r 1000

system	"system"	-B 100

read	"read_t1k"	-s 1k	-f $TFILE	-B 80k
read	"read_t10k"	-s 10k	-f $TFILE	-B 40k
read	"read_t100k"	-s 100k	-f $TFILE	-B 5k

read	"read_u1k"	-s 1k	-f $VFILE	-B 150k
read	"read_u10k"	-s 10k	-f $VFILE	-B 40k
read	"read_u100k"	-s 100k	-f $VFILE	-B 5k

read	"read_z1k"	-s 1k	-f /dev/zero	-B 300k
read	"read_z10k"	-s 10k	-f /dev/zero	-B 70k	
read	"read_z100k"	-s 100k	-f /dev/zero	-B 8k	
read	"read_zw100k"	-s 100k	-w	-f /dev/zero	-B 8k	

write	"write_t1k"	-s 1k	-f $TFILE	-B 80k	
write	"write_t10k"	-s 10k	-f $TFILE	-B 20k	
write	"write_t100k"	-s 100k	-f $TFILE	-B 1k	

write	"write_u1k"	-s 1k	-f $VFILE	-B 80k	
write	"write_u10k"	-s 10k	-f $VFILE	-B 20k	
write	"write_u100k"	-s 100k	-f $VFILE	-B 1k	

write	"write_n1k"	-s 1k	-f /dev/null	-B 500k	
write	"write_n10k"	-s 10k	-f /dev/null	-B 500k	
write	"write_n100k"	-s 100k	-f /dev/null	-B 500k	

writev	"writev_t1k"	-s 1k	-f $TFILE	-B 20k	
writev	"writev_t10k"	-s 10k	-f $TFILE	-B 1k	
writev	"writev_t100k"	-s 100k	-f $TFILE	-B 200	

writev	"writev_u1k"	-s 1k	-f $VFILE	-B 20k	
writev	"writev_u10k"	-s 10k	-f $VFILE	-B 1k	
writev	"writev_u100k"	-s 100k	-f $VFILE	-B 200	
	
writev	"writev_n1k"	-s 1k	-f /dev/null	-B 200k	
writev	"writev_n10k"	-s 10k	-f /dev/null	-B 200k	
writev	"writev_n100k"	-s 100k	-f /dev/null	-B 200k	

pread	"pread_t1k"	-s 1k	-f $TFILE
pread	"pread_t10k"	-s 10k	-f $TFILE
pread	"pread_t100k"	-s 100k	-f $TFILE	-B 10k

pread	"pread_u1k"	-s 1k	-f $VFILE
pread	"pread_u10k"	-s 10k	-f $VFILE
pread	"pread_u100k"	-s 100k	-f $VFILE	-B 10k

pread	"pread_z1k"	-s 1k	-f /dev/zero
pread	"pread_z10k"	-s 10k	-f /dev/zero
pread	"pread_z100k"	-s 100k	-f /dev/zero	-B 10k
pread	"pread_zw100k"	-s 100k	-w	-f /dev/zero	-B 10k

pwrite	"pwrite_t1k"	-s 1k	-f $TFILE
pwrite	"pwrite_t10k"	-s 10k	-f $TFILE	-B 10k
pwrite	"pwrite_t100k"	-s 100k	-f $TFILE	-B 8k

pwrite	"pwrite_u1k"	-s 1k	-f $VFILE
pwrite	"pwrite_u10k"	-s 10k	-f $VFILE	-B 10k
pwrite	"pwrite_u100k"	-s 100k	-f $VFILE	-B 8k

pwrite	"pwrite_n1k"	-s 1k	-f /dev/null	-B 500k
pwrite	"pwrite_n10k"	-s 10k	-f /dev/null	-B 500k
pwrite	"pwrite_n100k"	-s 100k	-f /dev/null	-B 500k

mmap	"mmap_z8k"	-l 8k	-f /dev/zero	-B 10k
mmap	"mmap_z128k"	-l 128k	-f /dev/zero	-B 5k
mmap	"mmap_t8k"	-l 8k	-f $TFILE	-B 10k
mmap	"mmap_t128k"	-l 128k	-f $TFILE	-B 5k
mmap	"mmap_u8k"	-l 8k	-f $VFILE	-B 10k
mmap	"mmap_u128k"	-l 128k	-f $VFILE	-B 5k
mmap	"mmap_a8k"	-l 8k	-f MAP_ANON	-B 10k
mmap	"mmap_a128k"	-l 128k	-f MAP_ANON	-B 5k

mmap	"mmap_rz8k"	-l 8k	-r	-f /dev/zero	-B 10k
mmap	"mmap_rz128k"	-l 128k	-r	-f /dev/zero	-B 5k
mmap	"mmap_rt8k"	-l 8k	-r	-f $TFILE	-B 10k
mmap	"mmap_rt128k"	-l 128k	-r	-f $TFILE	-B 5k
mmap	"mmap_ru8k"	-l 8k	-r	-f $VFILE	-B 10k
mmap	"mmap_ru128k"	-l 128k	-r	-f $VFILE	-B 5k
mmap	"mmap_ra8k"	-l 8k	-r	-f MAP_ANON	-B 10k
mmap	"mmap_ra128k"	-l 128k	-r	-f MAP_ANON	-B 5k

mmap	"mmap_wz8k"	-l 8k	-w	-f /dev/zero	-B 10k
mmap	"mmap_wz128k"	-l 128k	-w	-f /dev/zero	-B 1k
mmap	"mmap_wt8k"	-l 8k	-w	-f $TFILE	-B 10k
mmap	"mmap_wt128k"	-l 128k	-w	-f $TFILE	-B 1k
mmap	"mmap_wu8k"	-l 8k	-w	-f $VFILE	-B 10k
mmap	"mmap_wu128k"	-l 128k	-w	-f $VFILE	-B 1k
mmap	"mmap_wa8k"	-l 8k	-w	-f MAP_ANON	-B 10k
mmap	"mmap_wa128k"	-l 128k	-w	-f MAP_ANON	-B 1k

mmap_frag	"mmap_frag"	-B 10000	-C 1000

munmap	"unmap_z8k"	-l 8k	-f /dev/zero	-B 10k
munmap	"unmap_z128k"	-l 128k	-f /dev/zero	-B 5k
munmap	"unmap_t8k"	-l 8k	-f $TFILE	-B 10k
munmap	"unmap_t128k"	-l 128k	-f $TFILE	-B 5k
munmap	"unmap_u8k"	-l 8k	-f $VFILE	-B 10k
munmap	"unmap_u128k"	-l 128k	-f $VFILE	-B 5k
munmap	"unmap_a8k"	-l 8k	-f MAP_ANON	-B 10k
munmap	"unmap_a128k"	-l 128k	-f MAP_ANON	-B 5k

munmap	"unmap_rz8k"	-l 8k	-r	-f /dev/zero	-B 10k
munmap	"unmap_rz128k"	-l 128k	-r	-f /dev/zero	-B 15k
munmap	"unmap_rt8k"	-l 8k	-r	-f $TFILE	-B 10k
munmap	"unmap_rt128k"	-l 128k	-r	-f $TFILE	-B 5k
munmap	"unmap_ru8k"	-l 8k	-r	-f $VFILE	-B 10k
munmap	"unmap_ru128k"	-l 128k	-r	-f $VFILE	-B 5k
munmap	"unmap_ra8k"	-l 8k	-r	-f MAP_ANON	-B 10k
munmap	"unmap_ra128k"	-l 128k	-r	-f MAP_ANON	-B 5k

munmap	"unmap_wz8k"	-l 8k	-w	-f /dev/zero	-B 10k
munmap	"unmap_wz128k"	-l 128k	-w	-f /dev/zero	-B 1k
munmap	"unmap_wt8k"	-l 8k	-w	-f $TFILE	-B 10k
munmap	"unmap_wt128k"	-l 128k	-w	-f $TFILE	-B 1k
munmap	"unmap_wu8k"	-l 8k	-w	-f $VFILE	-B 10k
munmap	"unmap_wu128k"	-l 128k	-w	-f $VFILE	-B 1k
munmap	"unmap_wa8k"	-l 8k	-w	-f MAP_ANON	-B 10k
munmap	"unmap_wa128k"	-l 128k	-w	-f MAP_ANON	-B 1k

munmap_frag	"unmap_frag"	-B 2500	-C 1000

mprotect	"mprot_z8k"	-l 8k	-f /dev/zero	-B 10k
mprotect	"mprot_z128k"	-l 128k	-f /dev/zero	-B 5k
mprotect	"mprot_wz8k"	-l 8k	-w	-f /dev/zero	-B 10k
mprotect	"mprot_wz128k"	-l 128k	-w	-f /dev/zero	-B 5k
mprotect	"mprot_twz8k"	-l 8k	-w	-t	-f /dev/zero	-B 10k
mprotect	"mprot_tw128k"	-l 128k	-w	-t	-f /dev/zero	-B 5k
mprotect	"mprot_tw4m"	-l 4m	-w	-t	-f /dev/zero	-B 500

msync	"msync_t8k"	-l 8k	-f $TFILE	-B 10k
msync	"msync_t128k"	-l 128k	-f $TFILE	-B 5k
msync	"msync_u8k"	-l 8k	-f $VFILE	-B 10k
msync	"msync_u128k"	-l 128k	-f $VFILE	-B 5k
msync	"msync_ta8k"	-l 8k	-f $TFILE	-a	-B 10k
msync	"msync_ta128k"	-l 128k	-f $TFILE	-a	-B 5k
msync	"msync_ua8k"	-l 8k	-f $VFILE	-a	-B 10k
msync	"msync_ua128k"	-l 128k	-f $VFILE	-a	-B 5k
msync	"msync_ti8k"	-l 8k	-f $TFILE	-i	-B 10k
msync	"msync_ti128k"	-l 128k	-f $TFILE	-i	-B 5k
msync	"msync_ui8k"	-l 8k	-f $VFILE	-i	-B 10k
msync	"msync_ui128k"	-l 128k	-f $VFILE	-i	-B 5k
msync	"msync_tai8k"	-l 8k	-f $TFILE	-a	-i	-B 10k
msync	"msync_tai128k"	-l 128k	-f $TFILE	-a	-i	-B 5k
msync	"msync_uai8k"	-l 8k	-f $VFILE	-a	-i	-B 10k
msync	"msync_uai128k"	-l 128k	-f $VFILE	-a	-i	-B 5k

msync	"msync_rt8k"	-l 8k	-r	-f $TFILE	-B 10k
msync	"msync_rt128k"	-l 128k	-r	-f $TFILE	-B 5k
msync	"msync_ru8k"	-l 8k	-r	-f $VFILE	-B 10k
msync	"msync_ru128k"	-l 128k	-r	-f $VFILE	-B 5k
msync	"msync_rat8k"	-l 8k	-r	-f $TFILE	-a	-B 10k
msync	"msync_rat128k"	-l 128k	-r	-f $TFILE	-a	-B 5k
msync	"msync_rau8k"	-l 8k	-r	-f $VFILE	-a	-B 10k
msync	"msync_rau128k"	-l 128k	-r	-f $VFILE	-a	-B 5k
msync	"msync_rit8k"	-l 8k	-r	-f $TFILE	-i	-B 10k
msync	"msync_rit128k"	-l 128k	-r	-f $TFILE	-i	-B 5k
msync	"msync_riu8k"	-l 8k	-r	-f $VFILE	-i	-B 10k
msync	"msync_riu128k"	-l 128k	-r	-f $VFILE	-i	-B 5k
msync	"msync_rait8k"	-l 8k	-r	-f $TFILE	-a	-i	-B 10k
msync	"msync_rait128k"	-l 128k	-r	-f $TFILE	-a	-i	-B 5k
msync	"msync_raiu8k"	-l 8k	-r	-f $VFILE	-a	-i	-B 10k
msync	"msync_raiu128k"	-l 128k	-r	-f $VFILE	-a	-i	-B 5k

msync	"msync_wt8k"	-l 8k	-w	-f $TFILE	-B 10k
msync	"msync_wt128k"	-l 128k	-w	-f $TFILE	-B 1k
msync	"msync_wu8k"	-l 8k	-w	-f $VFILE	-B 10k
msync	"msync_wu128k"	-l 128k	-w	-f $VFILE	-B 1k
msync	"msync_wat8k"	-l 8k	-w	-f $TFILE	-a	-B 10k
msync	"msync_wat128k"	-l 128k	-w	-f $TFILE	-a	-B 1k
msync	"msync_wau8k"	-l 8k	-w	-f $VFILE	-a	-B 10k
msync	"msync_wau128k"	-l 128k	-w	-f $VFILE	-a	-B 1k
msync	"msync_wit8k"	-l 8k	-w	-f $TFILE	-i	-B 10k
msync	"msync_wit128k"	-l 128k	-w	-f $TFILE	-i	-B 1k
msync	"msync_wiu8k"	-l 8k	-w	-f $VFILE	-i	-B 10k
msync	"msync_wiu128k"	-l 128k	-w	-f $VFILE	-i	-B 1k
msync	"msync_wait8k"	-l 8k	-w	-f $TFILE	-a	-i	-B 10k
msync	"msync_wait128k"	-l 128k	-w	-f $TFILE	-a	-i	-B 1k
msync	"msync_waiu8k"	-l 8k	-w	-f $VFILE	-a	-i	-B 10k
msync	"msync_waiu128k"	-l 128k	-w	-f $VFILE	-a	-i	-B 1k

pipe	"pipe_pst1"	-s 1	-x pipe	-m st
pipe	"pipe_pmt1"	-s 1	-x pipe	-m mt	-B 10k
pipe	"pipe_pmp1"	-s 1	-x pipe	-m mp	-B 10k
pipe	"pipe_pst4k"	-s 4k	-x pipe	-m st
pipe	"pipe_pmt4k"	-s 4k	-x pipe	-m mt	-B 10k
pipe	"pipe_pmp4k"	-s 4k	-x pipe	-m mp	-B 10k

pipe	"pipe_sst1"	-s 1	-x sock	-m st
pipe	"pipe_smt1"	-s 1	-x sock	-m mt	-B 10k
pipe	"pipe_smp1"	-s 1	-x sock	-m mp	-B 10k
pipe	"pipe_sst4k"	-s 4k	-x sock	-m st
pipe	"pipe_smt4k"	-s 4k	-x sock	-m mt	-B 10k
pipe	"pipe_smp4k"	-s 4k	-x sock	-m mp	-B 10k

pipe	"pipe_tst1"	-s 1	-x tcp	-m st	-B 10k
pipe	"pipe_tmt1"	-s 1	-x tcp	-m mt	-B 5k
pipe	"pipe_tmp1"	-s 1	-x tcp	-m mp	-B 5k
pipe	"pipe_tst4k"	-s 4k	-x tcp	-m st	-B 10k
pipe	"pipe_tmt4k"	-s 4k	-x tcp	-m mt	-B 5k
pipe	"pipe_tmp4k"	-s 4k	-x tcp	-m mp	-B 5k
