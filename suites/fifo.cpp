#ifndef _FIFO_
#define _FIFO_

pipe	"pipe_fst1"	-s 1	-x fifo	-m st
pipe	"pipe_fmt1"	-s 1	-x fifo	-m mt	-B 5k
pipe	"pipe_fmp1"	-s 1	-x fifo	-m mp	-B 10k
pipe	"pipe_fst4k"	-s 4k	-x fifo	-m st
pipe	"pipe_fmt4k"	-s 4k	-x fifo	-m mt	-B 5k
pipe	"pipe_fmp4k"	-s 4k	-x fifo	-m mp	-B 5k

#endif /* _FIFO_ */
