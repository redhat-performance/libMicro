#!/bin/sh
#
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms
# of the Common Development and Distribution License
# (the "License").  You may not use this file except
# in compliance with the License.
#
# You can obtain a copy of the license at
# src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing
# permissions and limitations under the License.
#
# When distributing Covered Code, include this CDDL
# HEADER in each file and include the License file at
# usr/src/OPENSOLARIS.LICENSE.  If applicable,
# add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your
# own identifying information: Portions Copyright [yyyy]
# [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

DIRNAME=$(dirname $0)

bench_version=0.4.1-rh.17
libmicro_version=`$DIRNAME/bin/tattle -V`

case $libmicro_version in
$bench_version)
	;;
*)
	echo "ERROR: libMicro version doesn't match 'bench' script version"
	exit 1
esac

TMPROOT=/tmp/libmicro.$$
VARROOT=/var/tmp/libmicro.$$
mkdir -p $TMPROOT
mkdir -p $VARROOT
trap "rm -rf $TMPROOT $VARROOT" 0 2

TFILE=$TMPROOT/data
IFILE=$TMPROOT/ifile
TDIR1=$TMPROOT/0/1/2/3/4/5/6/7/8/9
TDIR2=$TMPROOT/1/2/3/4/5/6/7/8/9/0
VFILE=$VARROOT/data
VDIR1=$VARROOT/0/1/2/3/4/5/6/7/8/9
VDIR2=$VARROOT/1/2/3/4/5/6/7/8/9/0

dd if=/dev/zero of=$TFILE bs=1M count=1K 2>/dev/null
dd if=/dev/zero of=$VFILE bs=1M count=1K 2>/dev/null
mkdir -p $TDIR1 $TDIR2
mkdir -p $VDIR1 $VDIR2

touch $IFILE

OPTS="-C 100 -D 10000 -E -L -S -W"

ROOT_UID=0   # Only users with $UID 0 have root privileges.

if [ "$UID" -eq "$ROOT_UID" ]
then
	# Make an attempt to keep the system from going into
	# energy savings mode (Intel systems only?).
	$DIRNAME/bin/pm_qos > /dev/null 2>&1 < /dev/null &
    PM_QOS_PID=$!
fi

ARCH=`uname -m`

# produce benchmark header for easier comparisons

hostname=`uname -n`

if [ -f /usr/sbin/psrinfo ]; then
	p_type=`psrinfo -vp 2>/dev/null | awk '{if (NR == 3) {print $0; exit}}'`
fi

if [ -f /proc/cpuinfo ]; then
	p_type=`awk -F: '/model name/{print $2; exit}' /proc/cpuinfo`
fi

printf "!Libmicro_#:          %45s\n" $libmicro_version
printf "!Options:             %45s\n" "$OPTS"
printf "!Machine_name:        %45s\n" $hostname
printf "!OS_name:             %45s\n" `uname -s`
printf "!OS_release:          %45s\n" `uname -r`
printf "!OS_build:            %45s\n" "`uname -v`"
printf "!IPV4_address:        %45s\n" `getent ahostsv4 $hostname | grep STREAM | awk '{print $1}'`
printf "!IPV6_address:        %45s\n" `getent ahostsv6 $hostname | grep STREAM | awk '{print $1}'`
printf "!Run_by:              %45s\n" $LOGNAME
printf "!Date:                %45s\n" "`date '+%D %R'`"
printf "!Compiler:            %45s\n" `$DIRNAME/bin/tattle -c`
printf "!Compiler Ver.:       %45s\n" "`$DIRNAME/bin/tattle -v`"
printf "!Libc Ver.:           %45s\n" "`$DIRNAME/bin/tattle -l`"
printf "!Libpthread Ver.:     %45s\n" "`$DIRNAME/bin/tattle -p`"
printf "!sizeof(long):        %45s\n" `$DIRNAME/bin/tattle -s`
printf "!extra_CFLAGS:        %45s\n" "`$DIRNAME/bin/tattle -f`"
printf "!TimerRes:            %45s\n" "`$DIRNAME/bin/tattle -r`"

printf "!CPU_NAME:            %45s\n" "$p_type"

lscpu | sed 's/(s)/s/g' | sed -r 's/: +/:/g' | sed 's/, /,/g' | sed 's/ /_/g' | sed 's/:/ /g' | sed 's/Architecture/Processor/g' | sed -r 's/^CPUs/#CPUs/g' | sed 's/CPU_sockets/Sockets/g' | awk '{name=$1; val=$2; printf("!%-20s %45s\n", name ":", val);}'

dmidecode --type 17 2> /dev/null | awk -f $DIRNAME/mem.awk | awk '{printf("!%-40s %25s\n", "Memory:", $1 "," $2 "," $3)}'

sysctl -A 2> /dev/null | grep sched | grep -v sched_domain | awk '{printf("!%-40s %25s\n", $1 ":", $3)}'

numactl --hardware | awk -f $DIRNAME/numactl.awk

mkdir -p $TMPROOT/bin
cp $DIRNAME/bin-$ARCH/exec_bin $TMPROOT/bin/

while read A B
do
	# If we encounter a problem where the "trap" runs above, we will have lost
	# the TMPROOT directory. So just exit the loop when we detect that it no
	# longer exists.
	if [ ! -e $TMPROOT ]
	then
		break
	fi

	# $A contains the command, $B contains the arguments we echo blank lines
	# and comments we ship anything which fails to match *$1* (useful if we
	# only want to test one case, but a nasty hack)

	case $A in
	"#"*)
		# Add an extra "pound" at the beginning so that these comments will
		# not be added to the html page constructed by multiview
		echo "#$A $B"
		continue
		;;
	"")
		continue
		;;
	*$1*)
		;;
	*)
		continue
	esac

	if [ ! -f $TMPROOT/bin/$A ]
	then
		cp $DIRNAME/bin-$ARCH/$A $TMPROOT/bin/$A
	fi

	if [ "$UID" -eq "$ROOT_UID" ]
	then
		# Clear the system caches before starting the run
		sync; sync
		echo 3 > /proc/sys/vm/drop_caches
	fi

	(cd $TMPROOT && eval "bin/$A $B")
done <<.
#
# Obligatory null system call: use very short time
# for default since SuSe implements this "syscall" in userland
#
getpid	$OPTS	-N "getpid"	-B 640000
getpid	$OPTS	-N "getpid_s"	-B 32000	-s
getpid	$OPTS	-N "getpidT4"	-B 640000	-T 4
getpid	$OPTS	-N "getpidT4_s"	-B 32000	-T 4	-s

sched_yield	$OPTS	-N "sched_yield"	-B 32000
sched_yield	$OPTS	-N "sched_yield_s"	-B 32000	-s
sched_yield	$OPTS	-N "sched_yieldT4"	-B 32000	-T 4
sched_yield	$OPTS	-N "sched_yieldT4_s"	-B 32000	-T 4	-s

getenv	$OPTS	-N "getenv"	-B 90000	-s 100
getenv	$OPTS	-N "getenvT2"	-B 90000	-T 2	-s 100

gettimeofday	$OPTS	-N "gettimeofday"	-B 400000

clock_gettime	$OPTS	-N "clock_gettime"	-B 400000

nanosleep	$OPTS	-N "nanosleep"	-B 1500
nanosleep	$OPTS	-N "nanosleep_s"	-B 1500	-s
nanosleep	$OPTS	-N "nanosleep_r"	-B 1500	-r
nanosleep	$OPTS	-N "nanosleep_rs"	-B 1500	-s	-r

nanosleep	$OPTS	-N "nanosleepT4"	-B 1500	-T 4
nanosleep	$OPTS	-N "nanosleepT4_s"	-B 1500	-T 4	-s
nanosleep	$OPTS	-N "nanosleepT4_r"	-B 1500	-T 4	-r
nanosleep	$OPTS	-N "nanosleepT4_rs"	-B 1500	-T 4	-s	-r

nanosleep	$OPTS	-N "nanosleep_d10"	-B 1500	-d 10
nanosleep	$OPTS	-N "nanosleep_d100"	-B 1500	-d 100
nanosleep	$OPTS	-N "nanosleep_d1000"	-B 1500	-d 1000
nanosleep	$OPTS	-N "nanosleep_d10000"	-B 1000	-d 10000
nanosleep	$OPTS	-N "nanosleep_d100000"	-B 1000	-d 100000

clock_nanosleep	$OPTS	-N "clock_nanosleep"	-B 1500
clock_nanosleep	$OPTS	-N "clock_nanosleep_s"	-B 1500	-s
clock_nanosleep	$OPTS	-N "clock_nanosleep_r"	-B 1500	-r
clock_nanosleep	$OPTS	-N "clock_nanosleep_rs"	-B 1500	-s	-r

clock_nanosleep	$OPTS	-N "clock_nanosleep"	-B 1500	-m
clock_nanosleep	$OPTS	-N "clock_nanosleep_s"	-B 1500	-m	-s
clock_nanosleep	$OPTS	-N "clock_nanosleep_r"	-B 1500	-m	-r
clock_nanosleep	$OPTS	-N "clock_nanosleep_rs"	-B 1500	-m	-s	-r

clock_nanosleep	$OPTS	-N "clock_nanosleepT4"	-B 1500	-T 4
clock_nanosleep	$OPTS	-N "clock_nanosleepT4_s"	-B 1500	-T 4	-s
clock_nanosleep	$OPTS	-N "clock_nanosleepT4"	-B 1500	-T 4	-r
clock_nanosleep	$OPTS	-N "clock_nanosleepT4_s"	-B 1500	-T 4	-s	-r

clock_nanosleep	$OPTS	-N "clock_nanosleepT4"	-B 1500	-T 4	-m
clock_nanosleep	$OPTS	-N "clock_nanosleepT4_s"	-B 1500	-T 4	-m	-s
clock_nanosleep	$OPTS	-N "clock_nanosleepT4"	-B 1500	-T 4	-m	-r
clock_nanosleep	$OPTS	-N "clock_nanosleepT4_s"	-B 1500	-T 4	-m	-s	-r

clock_nanosleep	$OPTS	-N "clock_nanosleep_d10"	-B 1500	-d 10
clock_nanosleep	$OPTS	-N "clock_nanosleep_d100"	-B 1500	-d 100
clock_nanosleep	$OPTS	-N "clock_nanosleep_d1000"	-B 1500	-d 1000
clock_nanosleep	$OPTS	-N "clock_nanosleep_d10000"	-B 1000	-d 10000
clock_nanosleep	$OPTS	-N "clock_nanosleep_d100000"	-B 1000	-d 100000

log	$OPTS	-N "log"	-B 2000000
exp	$OPTS	-N "exp"	-B 2000000

lrand48	$OPTS	-N "lrand48"	-B 8000000

memset	$OPTS	-N "memset_10"	-s 10	-B 10m
memset	$OPTS	-N "memset_256"	-s 256	-B 5m
memset	$OPTS	-N "memset_256_u"	-s 256	-a 1	-B 5m
memset	$OPTS	-N "memset_1k"	-s 1k	-B 2m
memset	$OPTS	-N "memset_4k"	-s 4k	-B 700k
memset	$OPTS	-N "memset_4k_uc"	-s 4k	-u	-B 100k

memset	$OPTS	-N "memset_10k"	-s 10k
memset	$OPTS	-N "memset_1m"	-s 1m	-B 200
memset	$OPTS	-N "memset_10m"	-s 10m	-B 100
memset	$OPTS	-N "memsetP2_10m"	-s 10m	-B 100	-P 2

memrand	$OPTS	-N "memrand"	-s 128m	-B 10000
cachetocache	$OPTS	-N "cachetocache"	-s 100k	-T 2

isatty	$OPTS	-N "isatty_yes"
isatty	$OPTS	-N "isatty_no" 	-f $IFILE

malloc	$OPTS	-N "malloc_10"	-s 10	-g 10	-B 1000
malloc	$OPTS	-N "malloc_100"	-s 100	-g 10	-B 1000
malloc	$OPTS	-N "malloc_1k"	-s 1k	-g 10	-B 1000
malloc	$OPTS	-N "malloc_10k"	-s 10k	-g 10	-B 1000
malloc	$OPTS	-N "malloc_100k"	-s 100k	-g 10	-B 1000

malloc	$OPTS	-N "mallocT2_10"	 	-s 10	 	-g 10	-T 2	-B 1000
malloc	$OPTS	-N "mallocT2_100"	 	-s 100 	-g 10	-T 2	-B 1000
malloc	$OPTS	-N "mallocT2_1k"	 	-s 1k	 	-g 10	-T 2	-B 1000
malloc	$OPTS	-N "mallocT2_10k"	 	-s 10k 	-g 10	-T 2	-B 1000
malloc	$OPTS	-N "mallocT2_100k" 	-s 100k	-g 10	-T 2	-B 1000

close	$OPTS	-N "close_bad"	-B 1m	-b
close	$OPTS	-N "close_tmp"	-B 640	-f $TFILE
close	$OPTS	-N "close_usr"	-B 640	-f $VFILE
close	$OPTS	-N "close_zero"	-B 640	-f /dev/zero

memcpy	$OPTS	-N "memcpy_10"	-s 10
memcpy	$OPTS	-N "memcpy_1k"	-s 1k
memcpy	$OPTS	-N "memcpy_10k"	-s 10k
memcpy	$OPTS	-N "memcpy_1m"	-s 1m	-B 100
memcpy	$OPTS	-N "memcpy_10m"	-s 10m	-B 50

strcpy	$OPTS	-N "strcpy_10"	-s 10	-B 10m
strcpy	$OPTS	-N "strcpy_1k"	-s 1k	-B 1m

strlen	$OPTS	-N "strlen_10"	-s 10	-B 15m
strlen	$OPTS	-N "strlen_1k"	-s 1k	-B 1m

strchr	$OPTS	-N "strchr_10"	-s 10	-B 18m
strchr	$OPTS	-N "strchr_1k"	-s 1k	-B 1m

strcmp	$OPTS	-N "strcmp_10"	-s 10	-B 14m
strcmp	$OPTS	-N "strcmp_1k"	-s 1k	-B 1m

strcasecmp	$OPTS	-N "scasecmp_10"	-s 10	-B 5m
strcasecmp	$OPTS	-N "scasecmp_1k"	-s 1k

strtol	$OPTS	-N "strtol"	-B 1m
atoi	$OPTS	-N "atoi"	-B 1m

getcontext	$OPTS	-N "getcontext"
setcontext	$OPTS	-N "setcontext"

mutex	$OPTS	-N "mutex_st"	-B 5m
mutex	$OPTS	-N "mutex_mt"	-t	-B 5m
mutex	$OPTS	-N "mutex_T2"	-T 2	-B 1m

longjmp	$OPTS	-N "longjmp"	-B 5m
siglongjmp	$OPTS	-N "siglongjmp"	-B 800k

getrusage	$OPTS	-N "getrusage"

times	$OPTS	-N "times"	-B 800k
time	$OPTS	-N "time"	-B 2m
localtime_r	$OPTS	-N "localtime_r"
strftime	$OPTS	-N "strftime"

mktime	$OPTS	-N "mktime"	-B 10k	
mktime	$OPTS	-N "mktimeT2"	-T 2	-B 10k

cascade_mutex	$OPTS	-N "c_mutex_1"
cascade_mutex	$OPTS	-N "c_mutex_10"	-T 10	-B 100
cascade_mutex	$OPTS	-N "c_mutex_200"	-T 200	-B 100

cascade_cond	$OPTS	-N "c_cond_1"
cascade_cond	$OPTS	-N "c_cond_10"	-T 10	-B 100
cascade_cond	$OPTS	-N "c_cond_200"	-T 200	-B 100

cascade_lockf	$OPTS	-N "c_lockf_1"
cascade_lockf	$OPTS	-N "c_lockf_10"	-P 10	-B 100
cascade_lockf	$OPTS	-N "c_lockf_200"	-P 200	-B 100

cascade_flock	$OPTS	-N "c_flock"
cascade_flock	$OPTS	-N "c_flock_10"	-P 10	-B 100
cascade_flock	$OPTS	-N "c_flock_200"	-P 200	-B 100

cascade_fcntl	$OPTS	-N "c_fcntl_1"	
cascade_fcntl	$OPTS	-N "c_fcntl_10"	-P 10	-B 100
cascade_fcntl	$OPTS	-N "c_fcntl_200"	-P 200	-B 10

file_lock	$OPTS	-N "file_lock"

getsockname	$OPTS	-N "getsockname"
getpeername	$OPTS	-N "getpeername"

chdir	$OPTS	-N "chdir_tmp"	-B 10k	$TDIR1 $TDIR2
chdir	$OPTS	-N "chdir_usr"	-B 10k	$VDIR1 $VDIR2

chdir	$OPTS	-N "chgetwd_tmp"	-B 10k	-g $TDIR1 $TDIR2
chdir	$OPTS	-N "chgetwd_usr"	-B 10k	-g $VDIR1 $VDIR2

realpath	$OPTS	-N "realpath_tmp"	-f $TDIR1	-B 7000
realpath	$OPTS	-N "realpath_usr"	-f $VDIR1	-B 7000

stat	$OPTS	-N "stat_tmp"	-f $TFILE
stat	$OPTS	-N "stat_usr"	-f $VFILE

fcntl	$OPTS	-N "fcntl_tmp"	-f $TFILE
fcntl	$OPTS	-N "fcntl_usr"	-f $VFILE
fcntl_ndelay	$OPTS	-N "fcntl_ndelay"

lseek	$OPTS	-N "lseek_t8k"	-s 8k	-f $TFILE
lseek	$OPTS	-N "lseek_u8k"	-s 8k	-f $VFILE

open	$OPTS	-N "open_tmp"	-B 25000	-f $TFILE
open	$OPTS	-N "open_usr"	-B 25000	-f $VFILE
open	$OPTS	-N "open_zero"	-B 25000	-f /dev/zero

dup	$OPTS	-N "dup"	-B 400k

socket	$OPTS	-N "socket_u"	-B 10k
socket	$OPTS	-N "socket_i"	-B 10k	-f PF_INET

socketpair	$OPTS	-N "socketpair"	-B 25000

setsockopt	$OPTS	-N "setsockopt"	

bind	$OPTS	-N "bind"	-B 200

listen	$OPTS	-N "listen"	-B 50k

connection	$OPTS	-N "connection"	-B 512
connection	$OPTS	-N "conn_connect"	-B 512	-c
connection	$OPTS	-N "conn_accept"	-B 512	-a

close_tcp	$OPTS	-N "close_tcp"	-B 1k

poll	$OPTS	-N "poll_10"	-n 10
poll	$OPTS	-N "poll_100"	-n 100	-B 10k
poll	$OPTS	-N "poll_1000"	-n 1000	-B 1k

poll	$OPTS	-N "poll_w10"	-n 10	-w 1
poll	$OPTS	-N "poll_w100"	-n 100	-w 10	-B 10k
poll	$OPTS	-N "poll_w1000"	-n 1000	-w 100	-B 1k

select	$OPTS	-N "select_10"	-n 10
select	$OPTS	-N "select_100"	-n 100	-B 10k
select	$OPTS	-N "select_1000"	-n 1000	-B 1k

select	$OPTS	-N "select_w10"	-n 10	-w 1
select	$OPTS	-N "select_w100"	-n 100	-w 10	-B 10k
select	$OPTS	-N "select_w1000"	-n 1000	-w 100	-B 1k

semop	$OPTS	-N "semop"

sigaction	$OPTS	-N "sigaction"
signal	$OPTS	-N "signal"
sigprocmask	$OPTS	-N "sigprocmask"

pthread_create	$OPTS	-N "pthread_32"	-B 32
pthread_create	$OPTS	-N "pthread_64"	-B 64
pthread_create	$OPTS	-N "pthread_128"	-B 128
pthread_create	$OPTS	-N "pthread_256"	-B 256
pthread_create	$OPTS	-N "pthread_512"	-B 512

fork	$OPTS	-N "fork_100"	-B 100
fork	$OPTS	-N "fork_1000"	-B 1000

exit	$OPTS	-N "exit_100"	-B 100
exit	$OPTS	-N "exit_100_nolibc"	-e	-B 100
exit	$OPTS	-N "exit_1000"	-B 1000
exit	$OPTS	-N "exit_1000_nolibc"	-e	-B 1000

exec	$OPTS	-N "exec"	-B 100

execw	$OPTS	-N "execw"	-B 250

execw	$OPTS	-N "execw_1r"	-B 250	-r 1
execw	$OPTS	-N "execw_10r"	-B 250	-r 10
execw	$OPTS	-N "execw_100r"	-B 125	-r 100
execw	$OPTS	-N "execw_1000r"	-B 25	-r 1000

execw	$OPTS	-N "execw_32n"	-B 250	-n 32	-p 8192
execw	$OPTS	-N "execw_64n"	-B 250	-n 64	-p 4096
execw	$OPTS	-N "execw_128n"	-B 250	-n 128	-p 2048
execw	$OPTS	-N "execw_256n"	-B 250	-n 256	-p 1024
execw	$OPTS	-N "execw_512n"	-B 250	-n 512	-p 512
execw	$OPTS	-N "execw_1024n"	-B 250	-n 1024	-p 256
execw	$OPTS	-N "execw_2048n"	-B 250	-n 2048	-p 128
execw	$OPTS	-N "execw_4096n"	-B 250	-n 4096	-p 64
execw	$OPTS	-N "execw_8192n"	-B 250	-n 8192	-p 32

execw	$OPTS	-N "execw_8192n1000r"	-B 25	-n 8192	-p 32	-r 1000

execw	$OPTS	-N "execw_32nt"	-B 100	-n 32	-p 8192	-t
execw	$OPTS	-N "execw_64nt"	-B 50	-n 64	-p 4096	-t
execw	$OPTS	-N "execw_128nt"	-B 25	-n 128	-p 2048	-t

execw	$OPTS	-N "execw_32ntv"	-B 500	-n 32	-p 8192	-t	-v
execw	$OPTS	-N "execw_64ntv"	-B 500	-n 64	-p 4096	-t	-v
execw	$OPTS	-N "execw_128ntv"	-B 500	-n 128	-p 2048	-t	-v
execw	$OPTS	-N "execw_256ntv"	-B 500	-n 256	-p 1024	-t	-v
execw	$OPTS	-N "execw_512ntv"	-B 500	-n 512	-p 512	-t	-v
execw	$OPTS	-N "execw_1024ntv"	-B 500	-n 1024	-p 256	-t	-v
execw	$OPTS	-N "execw_2048ntv"	-B 500	-n 2048	-p 128	-t	-v
execw	$OPTS	-N "execw_4096ntv"	-B 500	-n 4096	-p 64	-t	-v
execw	$OPTS	-N "execw_8192ntv"	-B 500	-n 8192	-p 32	-t	-v

execw	$OPTS	-N "execw_8192ntv1000r"	-B 500	-n 8192	-p 32	-t	-v	-r 1000

posix_spawn	$OPTS	-N "posix_spawn"	-B 500

posix_spawn	$OPTS	-N "posix_spawn_1r"	-B 500	-r 1
posix_spawn	$OPTS	-N "posix_spawn_10r"	-B 500	-r 10
posix_spawn	$OPTS	-N "posix_spawn_100r"	-B 500	-r 100
posix_spawn	$OPTS	-N "posix_spawn_1000r"	-B 500	-r 1000

posix_spawn	$OPTS	-N "posix_spawn_32n"	-B 500	-n 32	-p 8192
posix_spawn	$OPTS	-N "posix_spawn_64n"	-B 500	-n 64	-p 4096
posix_spawn	$OPTS	-N "posix_spawn_128n"	-B 500	-n 128	-p 2048
posix_spawn	$OPTS	-N "posix_spawn_256n"	-B 500	-n 256	-p 1024
posix_spawn	$OPTS	-N "posix_spawn_512n"	-B 500	-n 512	-p 512
posix_spawn	$OPTS	-N "posix_spawn_1024n"	-B 500	-n 1024	-p 256
posix_spawn	$OPTS	-N "posix_spawn_2048n"	-B 500	-n 2048	-p 128
posix_spawn	$OPTS	-N "posix_spawn_4096n"	-B 500	-n 4096	-p 64
posix_spawn	$OPTS	-N "posix_spawn_8192n"	-B 500	-n 8192	-p 32

posix_spawn	$OPTS	-N "posix_spawn_8192n1000r"	-B 500	-n 8192	-p 32	-r 1000

posix_spawn	$OPTS	-N "posix_spawn_32nt"	-B 500	-n 32	-p 8192	-t
posix_spawn	$OPTS	-N "posix_spawn_64nt"	-B 500	-n 64	-p 4096	-t
posix_spawn	$OPTS	-N "posix_spawn_128nt"	-B 500	-n 128	-p 2048	-t
posix_spawn	$OPTS	-N "posix_spawn_256nt"	-B 500	-n 256	-p 1024	-t
posix_spawn	$OPTS	-N "posix_spawn_512nt"	-B 500	-n 512	-p 512	-t
posix_spawn	$OPTS	-N "posix_spawn_1024nt"	-B 500	-n 1024	-p 256	-t
posix_spawn	$OPTS	-N "posix_spawn_2048nt"	-B 500	-n 2048	-p 128	-t
posix_spawn	$OPTS	-N "posix_spawn_4096nt"	-B 500	-n 4096	-p 64	-t
posix_spawn	$OPTS	-N "posix_spawn_8192nt"	-B 500	-n 8192	-p 32	-t

posix_spawn	$OPTS	-N "posix_spawn_8192nt1000r"	-B 500	-n 8192	-p 32	-t	-r 1000

system	$OPTS	-N "system"	-B 100

recurse	$OPTS	-N "recurse"	-B 200k

read	$OPTS	-N "read_t1k"	-s 1k	-f $TFILE	-B 80k
read	$OPTS	-N "read_t10k"	-s 10k	-f $TFILE	-B 40k
read	$OPTS	-N "read_t100k"	-s 100k	-f $TFILE	-B 5k

read	$OPTS	-N "read_u1k"	-s 1k	-f $VFILE	-B 150k
read	$OPTS	-N "read_u10k"	-s 10k	-f $VFILE	-B 40k
read	$OPTS	-N "read_u100k"	-s 100k	-f $VFILE	-B 5k

read	$OPTS	-N "read_z1k"	-s 1k	-f /dev/zero	-B 300k
read	$OPTS	-N "read_z10k"	-s 10k	-f /dev/zero	-B 70k	
read	$OPTS	-N "read_z100k"	-s 100k	-f /dev/zero	-B 8k	
read	$OPTS	-N "read_zw100k"	-s 100k	-w	-f /dev/zero	-B 8k	

write	$OPTS	-N "write_t1k"	-s 1k	-f $TFILE	-B 80k	
write	$OPTS	-N "write_t10k"	-s 10k	-f $TFILE	-B 20k	
write	$OPTS	-N "write_t100k"	-s 100k	-f $TFILE	-B 1k	

write	$OPTS	-N "write_u1k"	-s 1k	-f $VFILE	-B 80k	
write	$OPTS	-N "write_u10k"	-s 10k	-f $VFILE	-B 20k	
write	$OPTS	-N "write_u100k"	-s 100k	-f $VFILE	-B 1k	

write	$OPTS	-N "write_n1k"	-s 1k	-f /dev/null	-B 500k	
write	$OPTS	-N "write_n10k"	-s 10k	-f /dev/null	-B 500k	
write	$OPTS	-N "write_n100k"	-s 100k	-f /dev/null	-B 500k	

writev	$OPTS	-N "writev_t1k"	-s 1k	-f $TFILE	-B 20k	
writev	$OPTS	-N "writev_t10k"	-s 10k	-f $TFILE	-B 1k	
writev	$OPTS	-N "writev_t100k"	-s 100k	-f $TFILE	-B 200	

writev	$OPTS	-N "writev_u1k"	-s 1k	-f $VFILE	-B 20k	
writev	$OPTS	-N "writev_u10k"	-s 10k	-f $VFILE	-B 1k	
writev	$OPTS	-N "writev_u100k"	-s 100k	-f $VFILE	-B 200	
	
writev	$OPTS	-N "writev_n1k"	-s 1k	-f /dev/null	-B 200k	
writev	$OPTS	-N "writev_n10k"	-s 10k	-f /dev/null	-B 200k	
writev	$OPTS	-N "writev_n100k"	-s 100k	-f /dev/null	-B 200k	

pread	$OPTS	-N "pread_t1k"	-s 1k	-f $TFILE
pread	$OPTS	-N "pread_t10k"	-s 10k	-f $TFILE
pread	$OPTS	-N "pread_t100k"	-s 100k	-f $TFILE	-B 10k

pread	$OPTS	-N "pread_u1k"	-s 1k	-f $VFILE
pread	$OPTS	-N "pread_u10k"	-s 10k	-f $VFILE
pread	$OPTS	-N "pread_u100k"	-s 100k	-f $VFILE	-B 10k

pread	$OPTS	-N "pread_z1k"	-s 1k	-f /dev/zero
pread	$OPTS	-N "pread_z10k"	-s 10k	-f /dev/zero
pread	$OPTS	-N "pread_z100k"	-s 100k	-f /dev/zero	-B 10k
pread	$OPTS	-N "pread_zw100k"	-s 100k	-w	-f /dev/zero	-B 10k

pwrite	$OPTS	-N "pwrite_t1k"	-s 1k	-f $TFILE
pwrite	$OPTS	-N "pwrite_t10k"	-s 10k	-f $TFILE	-B 10k
pwrite	$OPTS	-N "pwrite_t100k"	-s 100k	-f $TFILE	-B 8k

pwrite	$OPTS	-N "pwrite_u1k"	-s 1k	-f $VFILE
pwrite	$OPTS	-N "pwrite_u10k"	-s 10k	-f $VFILE	-B 10k
pwrite	$OPTS	-N "pwrite_u100k"	-s 100k	-f $VFILE	-B 8k

pwrite	$OPTS	-N "pwrite_n1k"	-s 1k	-f /dev/null	-B 500k
pwrite	$OPTS	-N "pwrite_n10k"	-s 10k	-f /dev/null	-B 500k
pwrite	$OPTS	-N "pwrite_n100k"	-s 100k	-f /dev/null	-B 500k

mmap	$OPTS	-N "mmap_z8k"	-l 8k	-f /dev/zero	-B 10k
mmap	$OPTS	-N "mmap_z128k"	-l 128k	-f /dev/zero	-B 5k
mmap	$OPTS	-N "mmap_t8k"	-l 8k	-f $TFILE	-B 10k
mmap	$OPTS	-N "mmap_t128k"	-l 128k	-f $TFILE	-B 5k
mmap	$OPTS	-N "mmap_u8k"	-l 8k	-f $VFILE	-B 10k
mmap	$OPTS	-N "mmap_u128k"	-l 128k	-f $VFILE	-B 5k
mmap	$OPTS	-N "mmap_a8k"	-l 8k	-f MAP_ANON	-B 10k
mmap	$OPTS	-N "mmap_a128k"	-l 128k	-f MAP_ANON	-B 5k

mmap	$OPTS	-N "mmap_rz8k"	-l 8k	-r	-f /dev/zero	-B 10k
mmap	$OPTS	-N "mmap_rz128k"	-l 128k	-r	-f /dev/zero	-B 5k
mmap	$OPTS	-N "mmap_rt8k"	-l 8k	-r	-f $TFILE	-B 10k
mmap	$OPTS	-N "mmap_rt128k"	-l 128k	-r	-f $TFILE	-B 5k
mmap	$OPTS	-N "mmap_ru8k"	-l 8k	-r	-f $VFILE	-B 10k
mmap	$OPTS	-N "mmap_ru128k"	-l 128k	-r	-f $VFILE	-B 5k
mmap	$OPTS	-N "mmap_ra8k"	-l 8k	-r	-f MAP_ANON	-B 10k
mmap	$OPTS	-N "mmap_ra128k"	-l 128k	-r	-f MAP_ANON	-B 5k

mmap	$OPTS	-N "mmap_wz8k"	-l 8k	-w	-f /dev/zero	-B 10k
mmap	$OPTS	-N "mmap_wz128k"	-l 128k	-w	-f /dev/zero	-B 1k
mmap	$OPTS	-N "mmap_wt8k"	-l 8k	-w	-f $TFILE	-B 10k
mmap	$OPTS	-N "mmap_wt128k"	-l 128k	-w	-f $TFILE	-B 1k
mmap	$OPTS	-N "mmap_wu8k"	-l 8k	-w	-f $VFILE	-B 10k
mmap	$OPTS	-N "mmap_wu128k"	-l 128k	-w	-f $VFILE	-B 1k
mmap	$OPTS	-N "mmap_wa8k"	-l 8k	-w	-f MAP_ANON	-B 10k
mmap	$OPTS	-N "mmap_wa128k"	-l 128k	-w	-f MAP_ANON	-B 1k

mmap_frag	$OPTS	-N "mmap_frag"	-B 10000	-C 1000

munmap	$OPTS	-N "unmap_z8k"	-l 8k	-f /dev/zero	-B 10k
munmap	$OPTS	-N "unmap_z128k"	-l 128k	-f /dev/zero	-B 5k
munmap	$OPTS	-N "unmap_t8k"	-l 8k	-f $TFILE	-B 10k
munmap	$OPTS	-N "unmap_t128k"	-l 128k	-f $TFILE	-B 5k
munmap	$OPTS	-N "unmap_u8k"	-l 8k	-f $VFILE	-B 10k
munmap	$OPTS	-N "unmap_u128k"	-l 128k	-f $VFILE	-B 5k
munmap	$OPTS	-N "unmap_a8k"	-l 8k	-f MAP_ANON	-B 10k
munmap	$OPTS	-N "unmap_a128k"	-l 128k	-f MAP_ANON	-B 5k

munmap	$OPTS	-N "unmap_rz8k"	-l 8k	-r	-f /dev/zero	-B 10k
munmap	$OPTS	-N "unmap_rz128k"	-l 128k	-r	-f /dev/zero	-B 15k
munmap	$OPTS	-N "unmap_rt8k"	-l 8k	-r	-f $TFILE	-B 10k
munmap	$OPTS	-N "unmap_rt128k"	-l 128k	-r	-f $TFILE	-B 5k
munmap	$OPTS	-N "unmap_ru8k"	-l 8k	-r	-f $VFILE	-B 10k
munmap	$OPTS	-N "unmap_ru128k"	-l 128k	-r	-f $VFILE	-B 5k
munmap	$OPTS	-N "unmap_ra8k"	-l 8k	-r	-f MAP_ANON	-B 10k
munmap	$OPTS	-N "unmap_ra128k"	-l 128k	-r	-f MAP_ANON	-B 5k

munmap	$OPTS	-N "unmap_wz8k"	-l 8k	-w	-f /dev/zero	-B 10k
munmap	$OPTS	-N "unmap_wz128k"	-l 128k	-w	-f /dev/zero	-B 1k
munmap	$OPTS	-N "unmap_wt8k"	-l 8k	-w	-f $TFILE	-B 10k
munmap	$OPTS	-N "unmap_wt128k"	-l 128k	-w	-f $TFILE	-B 1k
munmap	$OPTS	-N "unmap_wu8k"	-l 8k	-w	-f $VFILE	-B 10k
munmap	$OPTS	-N "unmap_wu128k"	-l 128k	-w	-f $VFILE	-B 1k
munmap	$OPTS	-N "unmap_wa8k"	-l 8k	-w	-f MAP_ANON	-B 10k
munmap	$OPTS	-N "unmap_wa128k"	-l 128k	-w	-f MAP_ANON	-B 1k

munmap_frag	$OPTS	-N "munmap_frag"	-B 2500	-C 1000

mprotect	$OPTS	-N "mprot_z8k"	-l 8k	-f /dev/zero	-B 10k
mprotect	$OPTS	-N "mprot_z128k"	-l 128k	-f /dev/zero	-B 5k
mprotect	$OPTS	-N "mprot_wz8k"	-l 8k	-w	-f /dev/zero	-B 10k
mprotect	$OPTS	-N "mprot_wz128k"	-l 128k	-w	-f /dev/zero	-B 5k
mprotect	$OPTS	-N "mprot_twz8k"	-l 8k	-w	-t	-f /dev/zero	-B 10k
mprotect	$OPTS	-N "mprot_tw128k"	-l 128k	-w	-t	-f /dev/zero	-B 5k
mprotect	$OPTS	-N "mprot_tw4m"	-l 4m	-w	-t	-f /dev/zero	-B 500

pipe	$OPTS	-N "pipe_pst1"	-s 1	-x pipe	-m st
pipe	$OPTS	-N "pipe_pmt1"	-s 1	-x pipe	-m mt	-B 10k
pipe	$OPTS	-N "pipe_pmp1"	-s 1	-x pipe	-m mp	-B 10k
pipe	$OPTS	-N "pipe_pst4k"	-s 4k	-x pipe	-m st
pipe	$OPTS	-N "pipe_pmt4k"	-s 4k	-x pipe	-m mt	-B 10k
pipe	$OPTS	-N "pipe_pmp4k"	-s 4k	-x pipe	-m mp	-B 10k

pipe	$OPTS	-N "pipe_sst1"	-s 1	-x sock	-m st
pipe	$OPTS	-N "pipe_smt1"	-s 1	-x sock	-m mt	-B 10k
pipe	$OPTS	-N "pipe_smp1"	-s 1	-x sock	-m mp	-B 10k
pipe	$OPTS	-N "pipe_sst4k"	-s 4k	-x sock	-m st
pipe	$OPTS	-N "pipe_smt4k"	-s 4k	-x sock	-m mt	-B 10k
pipe	$OPTS	-N "pipe_smp4k"	-s 4k	-x sock	-m mp	-B 10k

pipe	$OPTS	-N "pipe_tst1"	-s 1	-x tcp	-m st	-B 10k
pipe	$OPTS	-N "pipe_tmt1"	-s 1	-x tcp	-m mt	-B 5k
pipe	$OPTS	-N "pipe_tmp1"	-s 1	-x tcp	-m mp	-B 5k
pipe	$OPTS	-N "pipe_tst4k"	-s 4k	-x tcp	-m st	-B 10k
pipe	$OPTS	-N "pipe_tmt4k"	-s 4k	-x tcp	-m mt	-B 5k
pipe	$OPTS	-N "pipe_tmp4k"	-s 4k	-x tcp	-m mp	-B 5k
.

# Clean up background processes
if [ -n "$PM_QOS_PID" ]; then
	kill -INT $PM_QOS_PID
fi

exit 0
