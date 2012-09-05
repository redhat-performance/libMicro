#ifndef _FIFO_
#define _FIFO_

pipe	"pipe_fst1"	-s 1	-x fifo	-m st
pipe	"pipe_fmt1"	-s 1	-x fifo	-m mt
pipe	"pipe_fmp1"	-s 1	-x fifo	-m mp
pipe	"pipe_fst4k"	-s 4k	-x fifo	-m st
pipe	"pipe_fmt4k"	-s 4k	-x fifo	-m mt
pipe	"pipe_fmp4k"	-s 4k	-x fifo	-m mp

#endif /* _FIFO_ */
