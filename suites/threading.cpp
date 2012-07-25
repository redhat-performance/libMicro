#ifndef _THREADING_
#define _THREADING_

sched_yield	"sched_yield"	-B 32000
sched_yield	"sched_yieldT4"	-B 32000	-T 4

#include "syscalls.cpp"

mutex	"mutex_st"	-B 5m
mutex	"mutex_mt"	-t	-B 5m
mutex	"mutex_T2"	-T 2	-B 1m

cascade_mutex	"c_mutex_1"
cascade_mutex	"c_mutex_10"	-T 10	-B 100
cascade_mutex	"c_mutex_200"	-T 200	-B 100

cascade_cond	"c_cond_1"
cascade_cond	"c_cond_10"	-T 10	-B 100
cascade_cond	"c_cond_200"	-T 200	-B 100

cascade_fcntl	"c_fcntl_1"	
cascade_fcntl	"c_fcntl_10"	-P 10	-B 100
cascade_fcntl	"c_fcntl_200"	-P 200	-B 10

pthread_create	"pthread_32"	-B 32
pthread_create	"pthread_64"	-B 64
pthread_create	"pthread_128"	-B 128
pthread_create	"pthread_256"	-B 256
pthread_create	"pthread_512"	-B 512

semop	"semop"

#endif /* _THREADING_ */
