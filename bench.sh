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

bench_version=0.4.1-rh.13
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

dd if=/dev/zero of=$TFILE bs=1024k count=10 2>/dev/null
dd if=/dev/zero of=$VFILE bs=1024k count=10 2>/dev/null
mkdir -p $TDIR1 $TDIR2
mkdir -p $VDIR1 $VDIR2

touch $IFILE

OPTS="-E -D 10000 -L -S -W"

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

	# $A contains the command, $B contains the arguments
	# we echo blank lines and comments
	# we ship anything which fails to match *$1* (useful
	# if we only want to test one case, but a nasty hack)

	case $A in
	"#"*)
		# Add an extra "pound" at the beginning so that
		# these comments will not be added to the html
		# page constructed by multiview
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
	(cd $TMPROOT && eval "bin/$A $B")
done <<.
#
# Obligatory null system call: use very short time
# for default since SuSe implements this "syscall" in userland
#
getpid		$OPTS -N "getpid"	-B 32000
getpid		$OPTS -N "getpid_s"	-B 32000	-s
getpid		$OPTS -N "getpidT4"	-B 32000	-T 4
getpid		$OPTS -N "getpidT4_s"	-B 32000	-T 4	-s

getenv		$OPTS -N "getenv"	-s 100 -I 6
getenv		$OPTS -N "getenvT2"	-s 100 -I 50	-T 2

gettimeofday	$OPTS -N "gettimeofday" -B 4000000

clock_gettime	$OPTS -N "clock_gettime" -B 4000000

log		$OPTS -N "log"	-I 3
exp		$OPTS -N "exp"	-I 3
lrand48		$OPTS -N "lrand48"  -B 800

memset		$OPTS -N "memset_10"	-s 10	-B 400
memset		$OPTS -N "memset_256"	-s 256	-B 200
memset		$OPTS -N "memset_256_u"	-s 256	 -a 1 -B 200
memset		$OPTS -N "memset_1k"	-s 1k	 -I 1
memset		$OPTS -N "memset_4k"    -s 4k    -I 6
memset		$OPTS -N "memset_4k_uc" -s 4k    -u -I 6

memset		$OPTS -N "memset_10k"	-s 10k	-I 150
memset		$OPTS -N "memset_1m"	-s 1m	-I 200000
memset		$OPTS -N "memset_10m"	-s 10m	-I 2000000
memset		$OPTS -N "memsetP2_10m"	-s 10m	-P 2	-I 2000000

memrand		$OPTS -N "memrand"	-s 128m -B 10000
cachetocache	$OPTS -N "cachetocache" -s 100k -T 2 -I 200

isatty		$OPTS -N "isatty_yes"
isatty		$OPTS -N "isatty_no"  -f $IFILE

malloc		$OPTS -N "malloc_10"    -s 10    -g 10 -B 1000
malloc		$OPTS -N "malloc_100"   -s 100   -g 10 -B 1000
malloc		$OPTS -N "malloc_1k"    -s 1k    -g 10 -B 1000
malloc		$OPTS -N "malloc_10k"   -s 10k   -g 10 -B 1000
malloc		$OPTS -N "malloc_100k"  -s 100k  -g 10 -B 1000

malloc		$OPTS -N "mallocT2_10"    -s 10   -g 10 -T 2 -B 1000
malloc		$OPTS -N "mallocT2_100"   -s 100  -g 10 -T 2 -B 1000
malloc		$OPTS -N "mallocT2_1k"    -s 1k   -g 10 -T 2 -B 1000
malloc		$OPTS -N "mallocT2_10k"   -s 10k  -g 10 -T 2 -B 1000
malloc		$OPTS -N "mallocT2_100k"  -s 100k -g 10 -T 2 -B 1000

close		$OPTS -N "close_bad"	-B 768	-b
close		$OPTS -N "close_tmp"	-B 640	-f $TFILE
close		$OPTS -N "close_usr"	-B 640	-f $VFILE
close		$OPTS -N "close_zero"	-B 640	-f /dev/zero

memcpy		$OPTS -N "memcpy_10"	-s 10	-B 200
memcpy		$OPTS -N "memcpy_1k"	-s 1k	-B 200
memcpy		$OPTS -N "memcpy_10k"	-s 10k	-I 5
memcpy		$OPTS -N "memcpy_1m"	-s 1m   -I 7000
memcpy		$OPTS -N "memcpy_10m"	-s 10m  -I 1250000

strcpy		$OPTS -N "strcpy_10"	-s 10   -B 400
strcpy		$OPTS -N "strcpy_1k"	-s 1k   -B 200

strlen		$OPTS -N "strlen_10"	-s 10   -B 400
strlen		$OPTS -N "strlen_1k"	-s 1k   -B 200

strchr		$OPTS -N "strchr_10"	-s 10   -B 400
strchr		$OPTS -N "strchr_1k"	-s 1k   -B 200

strcmp		$OPTS -N "strcmp_10"	-s 10   -B 400
strcmp		$OPTS -N "strcmp_1k"	-s 1k   -B 200

strcasecmp	$OPTS -N "scasecmp_10"	-s 10 -B 200
strcasecmp	$OPTS -N "scasecmp_1k"	-s 1k -I 155

strtol		$OPTS -N "strtol"      -B 200

getcontext	$OPTS -N "getcontext"  -I 100
setcontext	$OPTS -N "setcontext"  -I 12

mutex		$OPTS -N "mutex_st"		-B 1000
mutex		$OPTS -N "mutex_mt"	-t	-B 1000
mutex		$OPTS -N "mutex_T2"     -T 2  -B 1000

longjmp		$OPTS -N "longjmp"	-B 200
siglongjmp	$OPTS -N "siglongjmp"	-B 200

getrusage	$OPTS -N "getrusage"	-I 200

times		$OPTS -N "times"	-I 1
time		$OPTS -N "time"		-I 2
localtime_r	$OPTS -N "localtime_r"	-I 100
strftime	$OPTS -N "strftime" -I 250

mktime		$OPTS -N "mktime"       -I 500
mktime		$OPTS -N "mktimeT2" -T 2 -I 1000

cascade_mutex	$OPTS -N "c_mutex_1"	-B 100
cascade_mutex	$OPTS -N "c_mutex_10"	-T 10 -I 5000
cascade_mutex	$OPTS -N "c_mutex_200"	-T 200	-I 2000000

cascade_cond	$OPTS -N "c_cond_1"	-B 100
cascade_cond	$OPTS -N "c_cond_10"	-T 10	-I 3000
cascade_cond	$OPTS -N "c_cond_200"	-T 200	-I 2000000

cascade_lockf	$OPTS -N "c_lockf_1"	-I 1000
cascade_lockf	$OPTS -N "c_lockf_10"	-P 10 -I 50000
cascade_lockf	$OPTS -N "c_lockf_200"	-P 200 -I 5000000

cascade_flock	$OPTS -N "c_flock"	-I 500
cascade_flock	$OPTS -N "c_flock_10"	-P 10   -I 6250
cascade_flock	$OPTS -N "c_flock_200"	-P 200	-I 5000000

cascade_fcntl	$OPTS -N "c_fcntl_1"		-I 1000
cascade_fcntl	$OPTS -N "c_fcntl_10"	-P 10 -I 20000
cascade_fcntl	$OPTS -N "c_fcntl_200"	-P 200	-I 5000000

file_lock	$OPTS -N "file_lock"   -I 50

getsockname	$OPTS -N "getsockname"	-I 5
getpeername	$OPTS -N "getpeername"	-I 5

chdir		$OPTS -N "chdir_tmp"	-I 1000		$TDIR1 $TDIR2
chdir		$OPTS -N "chdir_usr"	-I 1000		$VDIR1 $VDIR2

chdir		$OPTS -N "chgetwd_tmp"	-I 1500	-g $TDIR1 $TDIR2
chdir		$OPTS -N "chgetwd_usr"	-I 1500	-g $VDIR1 $VDIR2

realpath	$OPTS -N "realpath_tmp" -I 3000		-f $TDIR1
realpath	$OPTS -N "realpath_usr"	-I 3000	-f $VDIR1

stat		$OPTS -N "stat_tmp" -I 25		-f $TFILE
stat		$OPTS -N "stat_usr" -I 25		-f $VFILE

fcntl		$OPTS -N "fcntl_tmp"	-I 5	-f $TFILE
fcntl		$OPTS -N "fcntl_usr"	-I 5	-f $VFILE
fcntl_ndelay	$OPTS -N "fcntl_ndelay"	-I 5

lseek		$OPTS -N "lseek_t8k"	-s 8k	-I 5	-f $TFILE
lseek		$OPTS -N "lseek_u8k"	-s 8k	-I 5	-f $VFILE

open		$OPTS -N "open_tmp"		-B 256	-f $TFILE
open		$OPTS -N "open_usr"		-B 256	-f $VFILE
open		$OPTS -N "open_zero"		-B 256	-f /dev/zero

dup		$OPTS -N "dup"			-B 768

socket		$OPTS -N "socket_u"		-B 256
socket		$OPTS -N "socket_i"		-B 256	-f PF_INET

socketpair	$OPTS -N "socketpair"		-B 256

setsockopt	$OPTS -N "setsockopt"		-I 10

bind		$OPTS -N "bind"			-B 200

listen		$OPTS -N "listen"		-B 800

connection	$OPTS -N "connection"	-B 512	-C 100
connection	$OPTS -N "conn_connect"	-B 512			-c
connection	$OPTS -N "conn_accept"	-B 512	-C 100	-a

close_tcp	$OPTS -N "close_tcp"		-B 32

poll		$OPTS -N "poll_10"	-n 10	-I 25
poll		$OPTS -N "poll_100"	-n 100	-I 1000
poll		$OPTS -N "poll_1000"	-n 1000	-I 5000

poll		$OPTS -N "poll_w10"	-n 10	-I 25		-w 1
poll		$OPTS -N "poll_w100"	-n 100	-I 2000		-w 10
poll		$OPTS -N "poll_w1000"	-n 1000	-I 20000	-w 100

select		$OPTS -N "select_10"	-n 10	-I 25
select		$OPTS -N "select_100"	-n 100	-I 1000
select		$OPTS -N "select_1000"	-n 1000	-I 5000

select		$OPTS -N "select_w10"	-n 10	-I 25		-w 1
select		$OPTS -N "select_w100"	-n 100	-I 2000		-w 10
select		$OPTS -N "select_w1000"	-n 1000	-I 20000        -w 100

semop		$OPTS -N "semop" -I 10

sigaction	$OPTS -N "sigaction" -I 5
signal		$OPTS -N "signal" -I 500
sigprocmask	$OPTS -N "sigprocmask" -I 5

pthread_create  $OPTS -N "pthread_16"		-B 16
pthread_create  $OPTS -N "pthread_32"		-B 32
pthread_create  $OPTS -N "pthread_128"		-B 128
pthread_create  $OPTS -N "pthread_512"		-B 512

fork		$OPTS -N "fork_10"		-B 10
fork		$OPTS -N "fork_100"		-B 100
fork		$OPTS -N "fork_1000"	-B 1000

exit		$OPTS -N "exit_10"		-B 10
exit		$OPTS -N "exit_100"		-B 100
exit		$OPTS -N "exit_1000"	-B 1000

exit		$OPTS -N "exit_10_nolibc"	-e -B 10

exec		$OPTS -N "exec" -B 10

execw		$OPTS -N "execw"	-B 500

execw		$OPTS -N "execw_1r"	-B 500	-r 1
execw		$OPTS -N "execw_10r"	-B 500	-r 10
execw		$OPTS -N "execw_100r"	-B 500	-r 100
execw		$OPTS -N "execw_1000r"	-B 500	-r 1000

execw		$OPTS -N "execw_32n"	-B 500	-n 32	-p 8192
execw		$OPTS -N "execw_64n"	-B 500	-n 64	-p 4096
execw		$OPTS -N "execw_128n"	-B 500	-n 128	-p 2048
execw		$OPTS -N "execw_256n"	-B 500	-n 256	-p 1024
execw		$OPTS -N "execw_512n"	-B 500	-n 512	-p 512
execw		$OPTS -N "execw_1024n"	-B 500	-n 1024	-p 256
execw		$OPTS -N "execw_2048n"	-B 500	-n 2048	-p 128
execw		$OPTS -N "execw_4096n"	-B 500	-n 4096	-p 64
execw		$OPTS -N "execw_8192n"	-B 500	-n 8192	-p 32
execw		$OPTS -N "execw_8192n1000r"	-B 500	-n 8192	-p 32	-r 1000

execw		$OPTS -N "execw_32nt"	-B 250	-n 32	-p 8192	-t
execw		$OPTS -N "execw_64nt"	-B 150	-n 64	-p 4096	-t
execw		$OPTS -N "execw_128nt"	-B 100	-n 128	-p 2048	-t

execw		$OPTS -N "execw_32ntv"	-B 500	-n 32	-p 8192	-t	-v
execw		$OPTS -N "execw_64ntv"	-B 500	-n 64	-p 4096	-t	-v
execw		$OPTS -N "execw_128ntv"	-B 500	-n 128	-p 2048	-t	-v
execw		$OPTS -N "execw_256ntv"	-B 500	-n 256	-p 1024	-t	-v
execw		$OPTS -N "execw_512ntv"	-B 500	-n 512	-p 512	-t	-v
execw		$OPTS -N "execw_1024ntv"	-B 500	-n 1024	-p 256	-t	-v
execw		$OPTS -N "execw_2048ntv"	-B 500	-n 2048	-p 128	-t	-v
execw		$OPTS -N "execw_4096ntv"	-B 500	-n 4096	-p 64	-t	-v
execw		$OPTS -N "execw_8192ntv"	-B 500	-n 8192	-p 32	-t	-v
execw		$OPTS -N "execw_8192ntv1000r"	-B 500	-n 8192	-p 32	-t	-v	-r 1000

posix_spawn	$OPTS -N "posix_spawn"	-B 500

posix_spawn	$OPTS -N "posix_spawn_1r"	-B 500	-r 1
posix_spawn	$OPTS -N "posix_spawn_10r"	-B 500	-r 10
posix_spawn	$OPTS -N "posix_spawn_100r"	-B 500	-r 100
posix_spawn	$OPTS -N "posix_spawn_1000r"	-B 500	-r 1000

posix_spawn	$OPTS -N "posix_spawn_32n"	-B 500	-n 32	-p 8192
posix_spawn	$OPTS -N "posix_spawn_64n"	-B 500	-n 64	-p 4096
posix_spawn	$OPTS -N "posix_spawn_128n"	-B 500	-n 128	-p 2048
posix_spawn	$OPTS -N "posix_spawn_256n"	-B 500	-n 256	-p 1024
posix_spawn	$OPTS -N "posix_spawn_512n"	-B 500	-n 512	-p 512
posix_spawn	$OPTS -N "posix_spawn_1024n"	-B 500	-n 1024	-p 256
posix_spawn	$OPTS -N "posix_spawn_2048n"	-B 500	-n 2048	-p 128
posix_spawn	$OPTS -N "posix_spawn_4096n"	-B 500	-n 4096	-p 64
posix_spawn	$OPTS -N "posix_spawn_8192n"	-B 500	-n 8192	-p 32
posix_spawn	$OPTS -N "posix_spawn_8192n1000r"	-B 500	-n 8192	-p 32	-r 1000

posix_spawn	$OPTS -N "posix_spawn_32nt"	-B 500	-n 32	-p 8192	-t
posix_spawn	$OPTS -N "posix_spawn_64nt"	-B 500	-n 64	-p 4096	-t
posix_spawn	$OPTS -N "posix_spawn_128nt"	-B 500	-n 128	-p 2048	-t
posix_spawn	$OPTS -N "posix_spawn_256nt"	-B 500	-n 256	-p 1024	-t
posix_spawn	$OPTS -N "posix_spawn_512nt"	-B 500	-n 512	-p 512	-t
posix_spawn	$OPTS -N "posix_spawn_1024nt"	-B 500	-n 1024	-p 256	-t
posix_spawn	$OPTS -N "posix_spawn_2048nt"	-B 500	-n 2048	-p 128	-t
posix_spawn	$OPTS -N "posix_spawn_4096nt"	-B 500	-n 4096	-p 64	-t
posix_spawn	$OPTS -N "posix_spawn_8192nt"	-B 500	-n 8192	-p 32	-t
posix_spawn	$OPTS -N "posix_spawn_8192nt1000r"	-B 500	-n 8192	-p 32	-t	-r 1000

system		$OPTS -N "system" -I 1000000

recurse		$OPTS -N "recurse"		-B 512

read		$OPTS -N "read_t1k"	-s 1k	-B 800	-f $TFILE
read		$OPTS -N "read_t10k"	-s 10k	-B 200	-f $TFILE
read		$OPTS -N "read_t100k"	-s 100k	-B 20	-f $TFILE

read		$OPTS -N "read_u1k"	-s 1k	-B 500	-f $VFILE
read		$OPTS -N "read_u10k"	-s 10k	-B 200	-f $VFILE
read		$OPTS -N "read_u100k"	-s 100k	-B 20	-f $VFILE

read		$OPTS -N "read_z1k"	-s 1k	-B 600	-f /dev/zero
read		$OPTS -N "read_z10k"	-s 10k	-B 200	-f /dev/zero
read		$OPTS -N "read_z100k"	-s 100k	-B 40	-f /dev/zero
read		$OPTS -N "read_zw100k"	-s 100k	-B 40	-w	-f /dev/zero

write		$OPTS -N "write_t1k"	-s 1k	-B 200	-f $TFILE
write		$OPTS -N "write_t10k"	-s 10k	-B 100	-f $TFILE
write		$OPTS -N "write_t100k"	-s 100k	-B 10	-f $TFILE

write		$OPTS -N "write_u1k"	-s 1k	-B 200	-f $VFILE
write		$OPTS -N "write_u10k"	-s 10k	-B 100	-f $VFILE
write		$OPTS -N "write_u100k"	-s 100k	-B 10	-f $VFILE

write		$OPTS -N "write_n1k"	-s 1k	-B 200	-f /dev/null
write		$OPTS -N "write_n10k"	-s 10k	-B 100	-f /dev/null
write		$OPTS -N "write_n100k"	-s 100k	-B 50	-f /dev/null

writev		$OPTS -N "writev_t1k"	-s 1k	-B 200	-f $TFILE
writev		$OPTS -N "writev_t10k"	-s 10k	-B 10        -f $TFILE
writev		$OPTS -N "writev_t100k"	-s 100k		-f $TFILE

writev		$OPTS -N "writev_u1k"	-s 1k	-B 100	-f $VFILE
writev		$OPTS -N "writev_u10k"	-s 10k	-B 10	-f $VFILE
writev		$OPTS -N "writev_u100k"	-s 100k		-f $VFILE

writev		$OPTS -N "writev_n1k"	-s 1k	-B 200	-f /dev/null
writev		$OPTS -N "writev_n10k"	-s 10k	-B 100	-f /dev/null
writev		$OPTS -N "writev_n100k"	-s 100k	-B 10	-f /dev/null

pread		$OPTS -N "pread_t1k"	-s 1k	-I 15		-f $TFILE
pread		$OPTS -N "pread_t10k"	-s 10k	-I 50		-f $TFILE
pread		$OPTS -N "pread_t100k"	-s 100k	-I 500	-f $TFILE

pread		$OPTS -N "pread_u1k"	-s 1k	-I 15		-f $VFILE
pread		$OPTS -N "pread_u10k"	-s 10k	-I 50		-f $VFILE
pread		$OPTS -N "pread_u100k"	-s 100k	-I 500	-f $VFILE

pread		$OPTS -N "pread_z1k"	-s 1k	-I 15		-f /dev/zero
pread		$OPTS -N "pread_z10k"	-s 10k	-I 50		-f /dev/zero
pread		$OPTS -N "pread_z100k"	-s 100k	-I 200	-f /dev/zero
pread		$OPTS -N "pread_zw100k"	-s 100k	-w -I 250	-f /dev/zero

pwrite		$OPTS -N "pwrite_t1k"	-s 1k	-I 50		-f $TFILE
pwrite		$OPTS -N "pwrite_t10k"	-s 10k	-I 100		-f $TFILE
pwrite		$OPTS -N "pwrite_t100k"	-s 100k	-I 1000	-f $TFILE

pwrite		$OPTS -N "pwrite_u1k"	-s 1k	-I 50		-f $VFILE
pwrite		$OPTS -N "pwrite_u10k"	-s 10k	-I 100		-f $VFILE
pwrite		$OPTS -N "pwrite_u100k"	-s 100k	-I 1000	-f $VFILE

pwrite		$OPTS -N "pwrite_n1k"	-s 1k	-B 200		-f /dev/null
pwrite		$OPTS -N "pwrite_n10k"	-s 10k	-B 100		-f /dev/null
pwrite		$OPTS -N "pwrite_n100k"	-s 100k	-B 50		-f /dev/null

mmap		$OPTS -N "mmap_z8k"	-l 8k   -I 3		-f /dev/zero
mmap		$OPTS -N "mmap_z128k"	-l 128k	-I 30		-f /dev/zero
mmap		$OPTS -N "mmap_t8k"	-l 8k	-I 30		-f $TFILE
mmap		$OPTS -N "mmap_t128k"	-l 128k	-I 30		-f $TFILE
mmap		$OPTS -N "mmap_u8k"	-l 8k	-I 3		-f $VFILE
mmap		$OPTS -N "mmap_u128k"	-l 128k	-I 30		-f $VFILE
mmap		$OPTS -N "mmap_a8k"	-l 8k	-I 10		-f MAP_ANON
mmap		$OPTS -N "mmap_a128k"	-l 128k	-I 2		-f MAP_ANON

mmap		$OPTS -N "mmap_rz8k"	-l 8k	-I 1000 -r	-f /dev/zero
mmap		$OPTS -N "mmap_rz128k"	-l 128k	-I 1000 -r	-f /dev/zero
mmap		$OPTS -N "mmap_rt8k"	-l 8k	-I 1000 -r	-f $TFILE
mmap		$OPTS -N "mmap_rt128k"	-l 128k	-I 10000 -r	-f $TFILE
mmap		$OPTS -N "mmap_ru8k"	-l 8k	-I 1000 -r	-f $VFILE
mmap		$OPTS -N "mmap_ru128k"	-l 128k	-I 10000 -r	-f $VFILE
mmap		$OPTS -N "mmap_ra8k"	-l 8k	-I 500 -r	-f MAP_ANON
mmap		$OPTS -N "mmap_ra128k"	-l 128k	-I 5000 -r	-f MAP_ANON

mmap		$OPTS -N "mmap_wz8k"	-l 8k	-I 1250 -w	-f /dev/zero
mmap		$OPTS -N "mmap_wz128k"	-l 128k	-I 25000 -w	-f /dev/zero
mmap		$OPTS -N "mmap_wt8k"	-l 8k	-I 1250 -w	-f $TFILE
mmap		$OPTS -N "mmap_wt128k"	-l 128k	-I 25000 -w	-f $TFILE
mmap		$OPTS -N "mmap_wu8k"	-l 8k	-I 1250 -w	-f $VFILE
mmap		$OPTS -N "mmap_wu128k"	-l 128k	-I 12500 -w	-f $VFILE
mmap		$OPTS -N "mmap_wa8k"	-l 8k	-I 1500 -w	-f MAP_ANON
mmap		$OPTS -N "mmap_wa128k"	-l 128k	-I 25000 -w	-f MAP_ANON

munmap		$OPTS -N "unmap_z8k"	-l 8k   -I 25		-f /dev/zero
munmap		$OPTS -N "unmap_z128k"	-l 128k	-I 25		-f /dev/zero
munmap		$OPTS -N "unmap_t8k"	-l 8k	-I 25		-f $TFILE
munmap		$OPTS -N "unmap_t128k"	-l 128k	-I 25		-f $TFILE
munmap		$OPTS -N "unmap_u8k"	-l 8k	-I 25		-f $VFILE
munmap		$OPTS -N "unmap_u128k"	-l 128k	-I 25		-f $VFILE
munmap		$OPTS -N "unmap_a8k"	-l 8k	-I 25		-f MAP_ANON
munmap		$OPTS -N "unmap_a128k"	-l 128k	-I 25		-f MAP_ANON

munmap		$OPTS -N "unmap_rz8k"	-l 8k	-I 25	-r	-f /dev/zero
munmap		$OPTS -N "unmap_rz128k"	-l 128k	-I 50   -r	-f /dev/zero
munmap		$OPTS -N "unmap_rt8k"	-l 8k	-I 50	-r	-f $TFILE
munmap		$OPTS -N "unmap_rt128k"	-l 128k	-I 150	-r	-f $TFILE
munmap		$OPTS -N "unmap_ru8k"	-l 8k	-I 50	-r	-f $VFILE
munmap		$OPTS -N "unmap_ru128k"	-l 128k	-I 1500	-r	-f $VFILE
munmap		$OPTS -N "unmap_ra8k"	-l 8k	-I 25	-r	-f MAP_ANON
munmap		$OPTS -N "unmap_ra128k"	-l 128k	-I 50	-r	-f MAP_ANON

munmap		$OPTS -N "unmap_wz8k"	-l 8k	-I 500	-w	-f /dev/zero
munmap		$OPTS -N "unmap_wz128k"	-l 128k	-I 4000	-w	-f /dev/zero
munmap		$OPTS -N "unmap_wt8k"	-l 8k	-I 500	-w	-f $TFILE
munmap		$OPTS -N "unmap_wt128k"	-l 128k	-I 5000	-w	-f $TFILE
munmap		$OPTS -N "unmap_wu8k"	-l 8k	-I 500	-w	-f $VFILE
munmap		$OPTS -N "unmap_wu128k"	-l 128k	-I 5000	-w	-f $VFILE
munmap		$OPTS -N "unmap_wa8k"	-l 8k	-I 500	-w	-f MAP_ANON
munmap		$OPTS -N "unmap_wa128k"	-l 128k	-I 5000	-w	-f MAP_ANON

mprotect	$OPTS -N "mprot_z8k"	-l 8k   -I 30			-f /dev/zero
mprotect	$OPTS -N "mprot_z128k"	-l 128k	-I 50			-f /dev/zero
mprotect	$OPTS -N "mprot_wz8k"	-l 8k	-I 50	-w		-f /dev/zero
mprotect	$OPTS -N "mprot_wz128k"	-l 128k	-I 50	-w		-f /dev/zero
mprotect	$OPTS -N "mprot_twz8k"  -l 8k   -I 50	-w	-t	-f /dev/zero
mprotect	$OPTS -N "mprot_tw128k" -l 128k -I 10	-w	-t	-f /dev/zero
mprotect	$OPTS -N "mprot_tw4m"   -l 4m   -B 10	-w	-t	-f /dev/zero

pipe		$OPTS -N "pipe_pst1"	-s 1	-I 5	-x pipe -m st
pipe		$OPTS -N "pipe_pmt1"	-s 1	-I 2000	-x pipe -m mt
pipe		$OPTS -N "pipe_pmp1"	-s 1	-I 2000	-x pipe -m mp
pipe		$OPTS -N "pipe_pst4k"	-s 4k	-I 50	-x pipe -m st
pipe		$OPTS -N "pipe_pmt4k"	-s 4k	-I 2000	-x pipe -m mt
pipe		$OPTS -N "pipe_pmp4k"	-s 4k	-I 2000	-x pipe -m mp

pipe		$OPTS -N "pipe_sst1"	-s 1	-I 50	-x sock -m st
pipe		$OPTS -N "pipe_smt1"	-s 1	-I 2000	-x sock -m mt
pipe		$OPTS -N "pipe_smp1"	-s 1	-I 2000	-x sock -m mp
pipe		$OPTS -N "pipe_sst4k"	-s 4k	-I 50	-x sock -m st
pipe		$OPTS -N "pipe_smt4k"	-s 4k	-I 2000	-x sock -m mt
pipe		$OPTS -N "pipe_smp4k"	-s 4k	-I 2000	-x sock -m mp

pipe		$OPTS -N "pipe_tst1"	-s 1	-I 500	-x tcp  -m st
pipe		$OPTS -N "pipe_tmt1"	-s 1	-I 2000	-x tcp  -m mt
pipe		$OPTS -N "pipe_tmp1"	-s 1	-I 2000	-x tcp  -m mp
pipe		$OPTS -N "pipe_tst4k"	-s 4k	-I 500	-x tcp  -m st
pipe		$OPTS -N "pipe_tmt4k"	-s 4k	-I 2000	-x tcp  -m mt
pipe		$OPTS -N "pipe_tmp4k"	-s 4k	-I 2000	-x tcp  -m mp
.

# Clean up background processes
if [ -n "$PM_QOS_PID" ]; then
	kill -INT $PM_QOS_PID
fi

exit 0
