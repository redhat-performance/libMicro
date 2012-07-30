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

#
# Modifications by Red Hat, Inc.
#

DIRNAME=$(dirname $0)

bench_version=0.4.1-rh.19
libmicro_version=`$DIRNAME/bin/tattle -V`

case $libmicro_version in
$bench_version)
	;;
*)
	echo "ERROR: libMicro version doesn't match 'bench' script version"
	exit 1
esac

case $1 in
	-s) suite=$2; shift 2;;
	*) suite="full";;
esac

if [ ! -e "$DIRNAME/suites/$suite.txt" ]; then
	echo "ERROR: $DIRNAME/suites/$suite.txt does not exist"
	exit 1
fi

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

OPTS="-C 100 -D 10000 -X 600000 -E -L -S -W"

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

mkdir -p $TMPROOT/suites
cp $DIRNAME/suites/$suite.txt $TMPROOT/suites/

while read A B C
do
	# If we encounter a problem where the "trap" runs above, we will have lost
	# the TMPROOT directory. So just exit the loop when we detect that it no
	# longer exists.
	if [ ! -e $TMPROOT ]
	then
		break
	fi

	# $A contains the command, $B contains the test name, $C contains the test
	# specific arguments; we echo blank lines and comments we ship anything
	# which fails to match *$1* (useful if we only want to test one case, but
	# a nasty hack)

	case $A in
	"#"*)
		# Add an extra "pound" at the beginning so that these comments will
		# not be added to the html page constructed by multiview
		echo "#$A $B $C"
		continue
		;;
	"")
		continue
		;;
	esac

	case $B in
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

	(cd $TMPROOT && eval "bin/$A -N $B $OPTS $C")
done < $TMPROOT/suites/$suite.txt

# Clean up background processes
if [ -n "$PM_QOS_PID" ]; then
	kill -INT $PM_QOS_PID
fi

exit 0
