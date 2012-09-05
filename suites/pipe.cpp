#ifndef _PIPE_
#define _PIPE_

pipe	"pipe_pst1"	-s 1	-x pipe	-m st
pipe	"pipe_pmt1"	-s 1	-x pipe	-m mt
pipe	"pipe_pmp1"	-s 1	-x pipe	-m mp
pipe	"pipe_pst4k"	-s 4k	-x pipe	-m st
pipe	"pipe_pmt4k"	-s 4k	-x pipe	-m mt
pipe	"pipe_pmp4k"	-s 4k	-x pipe	-m mp

pipe	"pipe_sst1"	-s 1	-x sock	-m st
pipe	"pipe_smt1"	-s 1	-x sock	-m mt
pipe	"pipe_smp1"	-s 1	-x sock	-m mp
pipe	"pipe_sst4k"	-s 4k	-x sock	-m st
pipe	"pipe_smt4k"	-s 4k	-x sock	-m mt
pipe	"pipe_smp4k"	-s 4k	-x sock	-m mp

#include "localtcp.cpp"

#include "fifo.cpp"

#endif /* _PIPE_ */
