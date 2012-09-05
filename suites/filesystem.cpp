#ifndef _FILESYSTEM_
#define _FILESYSTEM_

close	"close_bad"	-B 7500	-b
close	"close_tmp"	-B 2600	-f $TFILE
# if defined(EXTENDED)
close	"close_usr"	-B 2600	-f $VFILE
# endif
close	"close_zero"	-B 2600	-f /dev/zero

cascade_lockf	"c_lockf_1"
# if defined(MULTIPROCESS)
cascade_lockf	"c_lockf_10"	-P 10
#  if defined(EXTENDED)
cascade_lockf	"c_lockf_200"	-P 200
#  endif
# endif

cascade_flock	"c_flock"
# if defined(MULTIPROCESS)
cascade_flock	"c_flock_10"	-P 10
#  if defined(EXTENDED)
cascade_flock	"c_flock_200"	-P 200
#  endif
# endif

file_lock	"file_lock"

chdir	"chdir_tmp"	$TDIR1 $TDIR2
chdir	"chdir_usr"	$VDIR1 $VDIR2

chdir	"chgetwd_tmp"	-g $TDIR1 $TDIR2
chdir	"chgetwd_usr"	-g $VDIR1 $VDIR2

realpath	"realpath_tmp"	-f $TDIR1
realpath	"realpath_usr"	-f $VDIR1

stat	"stat_tmp"	-f $TFILE
#  if defined(EXTENDED)
stat	"stat_usr"	-f $VFILE
# endif

fcntl	"fcntl_tmp"	-f $TFILE
#  if defined(EXTENDED)
fcntl	"fcntl_usr"	-f $VFILE
# endif

lseek	"lseek_t8k"	-s 8k	-f $TFILE
#  if defined(EXTENDED)
lseek	"lseek_u8k"	-s 8k	-f $VFILE
# endif

open	"open_tmp"	-f $TFILE
#  if defined(EXTENDED)
open	"open_usr"	-f $VFILE
# endif
open	"open_zero"	-f /dev/zero

dup	"dup"	-B 5500

#include "fifo.cpp"

#include "readwrite.cpp"

#endif /* _FILESYSTEM_ */
