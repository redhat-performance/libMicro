#ifndef _SYSCALLS_
#define _SYSCALLS_

# ifndef _THREADING_
nanosleep	"nanosleep_s"	-s
nanosleep	"nanosleep_rs"	-s	-r
#  ifdef MULTITHREADED
nanosleep	"nanosleepT4_s"	-T 4	-s
nanosleep	"nanosleepT4_rs"	-T 4	-s	-r
#  endif

getpid	"getpid_s"	-s
#  ifdef MULTITHREADED
getpid	"getpidT4_s"	-T 4	-s
#  endif
#  ifdef MULTIPROCESS
getpid	"getpidP4_s"	-P 4	-s
#  endif
#  if defined(MULTITHREADED) && defined(MULTIPROCESS)
getpid	"getpidT4P4_s"	-T 4	-P 4	-s
#  endif
# endif

sched_yield	"sched_yield_s"	-s
# ifdef MULTITHREADED
sched_yield	"sched_yieldT4_s"	-T 4	-s
# endif

#endif /* _SYSCALLS_ */
