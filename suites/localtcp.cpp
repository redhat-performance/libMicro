#ifndef _LOCALTCP_
#define _LOCALTCP_

pipe	"pipe_tst1"	-s 1	-x tcp	-m st
pipe	"pipe_tmt1"	-s 1	-x tcp	-m mt
pipe	"pipe_tmp1"	-s 1	-x tcp	-m mp
pipe	"pipe_tst4k"	-s 4k	-x tcp	-m st
pipe	"pipe_tmt4k"	-s 4k	-x tcp	-m mt
pipe	"pipe_tmp4k"	-s 4k	-x tcp	-m mp

#endif /* _LOCALTCP_ */
