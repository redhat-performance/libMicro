#ifndef _FILESYSTEM_
#define _FILESYSTEM_

close	"close_bad"	-B 700k	-b
close	"close_tmp"	-B 640	-f $TFILE
close	"close_usr"	-B 640	-f $VFILE
close	"close_zero"	-B 640	-f /dev/zero

cascade_lockf	"c_lockf_1"	-B 60k
# if defined(MULTIPROCESS)
cascade_lockf	"c_lockf_10"	-P 10	-B 500
#  if defined(EXTENDED)
cascade_lockf	"c_lockf_200"	-P 200	-B 50
#  endif
# endif

cascade_flock	"c_flock"	-B 40k
# if defined(MULTIPROCESS)
cascade_flock	"c_flock_10"	-P 10	-B 1k
#  if defined(EXTENDED)
cascade_flock	"c_flock_200"	-P 200	-B 100
#  endif
# endif

file_lock	"file_lock"	-B 70k

chdir	"chdir_tmp"	-B 10k	$TDIR1 $TDIR2
chdir	"chdir_usr"	-B 10k	$VDIR1 $VDIR2

chdir	"chgetwd_tmp"	-B 10k	-g $TDIR1 $TDIR2
chdir	"chgetwd_usr"	-B 10k	-g $VDIR1 $VDIR2

realpath	"realpath_tmp"	-f $TDIR1	-B 4k
realpath	"realpath_usr"	-f $VDIR1	-B 4k

stat	"stat_tmp"	-f $TFILE	-B 50k
stat	"stat_usr"	-f $VFILE	-B 50k

fcntl	"fcntl_tmp"	-f $TFILE
fcntl	"fcntl_usr"	-f $VFILE

lseek	"lseek_t8k"	-s 8k	-f $TFILE
lseek	"lseek_u8k"	-s 8k	-f $VFILE

open	"open_tmp"	-B 25000	-f $TFILE
open	"open_usr"	-B 25000	-f $VFILE
open	"open_zero"	-B 25000	-f /dev/zero

dup	"dup"	-B 200k

#include "fifo.cpp"

#include "readwrite.cpp"

#endif /* _FILESYSTEM_ */
