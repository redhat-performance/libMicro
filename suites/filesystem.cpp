#ifndef _FILESYSTEM_
#define _FILESYSTEM_

close	"close_bad"	-B 1m	-b
close	"close_tmp"	-B 640	-f $TFILE
close	"close_usr"	-B 640	-f $VFILE
close	"close_zero"	-B 640	-f /dev/zero

cascade_lockf	"c_lockf_1"	-B 10k
cascade_lockf	"c_lockf_10"	-P 10	-B 1k
cascade_lockf	"c_lockf_200"	-P 200	-B 100

cascade_flock	"c_flock"
cascade_flock	"c_flock_10"	-P 10	-B 1k
cascade_flock	"c_flock_200"	-P 200	-B 100

file_lock	"file_lock"

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

lseek	"lseek_t8k"	-s 8k	-f $TFILE
lseek	"lseek_u8k"	-s 8k	-f $VFILE

open	"open_tmp"	-B 25000	-f $TFILE
open	"open_usr"	-B 25000	-f $VFILE
open	"open_zero"	-B 25000	-f /dev/zero

dup	"dup"	-B 400k

#include "fifo.cpp"

#include "readwrite.cpp"

#endif /* _FILESYSTEM_ */
