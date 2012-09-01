#ifndef _SYSCALLS_
#define _SYSCALLS_

# ifndef _THREADING_
nanosleep	"nanosleep_s"	-B 1500	-s
nanosleep	"nanosleep_rs"	-B 1500	-s	-r
#  ifdef MULTITHREADED
nanosleep	"nanosleepT4_s"	-B 1500	-T 4	-s
nanosleep	"nanosleepT4_rs"	-B 1500	-T 4	-s	-r
#  endif

getpid	"getpid_s"	-B 96k	-s
#  ifdef MULTITHREADED
getpid	"getpidT4_s"	-B 96k	-T 4	-s
#  endif
#  ifdef MULTIPROCESS
getpid	"getpidP4_s"	-B 96k	-P 4	-s
#  endif
#  if defined(MULTITHREADED) && defined(MULTIPROCESS)
getpid	"getpidT4P4_s"	-B 96k	-T 4	-P 4	-s
#  endif
# endif

sched_yield	"sched_yield_s"	-B 32000	-s
# ifdef MULTITHREADED
sched_yield	"sched_yieldT4_s"	-B 32000	-T 4	-s
# endif

#endif /* _SYSCALLS_ */
