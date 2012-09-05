#ifndef _READWRITE_
#define _READWRITE_

read	"read_t1k"	-s 1k	-f $TFILE
read	"read_t10k"	-s 10k	-f $TFILE
read	"read_t100k"	-s 100k	-f $TFILE

# ifdef EXTENDED
read	"read_u1k"	-s 1k	-f $VFILE
read	"read_u10k"	-s 10k	-f $VFILE
read	"read_u100k"	-s 100k	-f $VFILE
# endif

read	"read_z1k"	-s 1k	-f /dev/zero
read	"read_z10k"	-s 10k	-f /dev/zero
read	"read_z100k"	-s 100k	-f /dev/zero
read	"read_zw100k"	-s 100k	-w	-f /dev/zero

write	"write_t1k"	-s 1k	-f $TFILE
write	"write_t10k"	-s 10k	-f $TFILE
write	"write_t100k"	-s 100k	-f $TFILE

# ifdef EXTENDED
write	"write_u1k"	-s 1k	-f $VFILE
write	"write_u10k"	-s 10k	-f $VFILE
write	"write_u100k"	-s 100k	-f $VFILE
# endif

write	"write_n1k"	-s 1k	-f /dev/null
write	"write_n10k"	-s 10k	-f /dev/null
write	"write_n100k"	-s 100k	-f /dev/null

writev	"writev_t1k"	-s 1k	-f $TFILE
writev	"writev_t10k"	-s 10k	-f $TFILE
writev	"writev_t100k"	-s 100k	-f $TFILE

# ifdef EXTENDED
writev	"writev_u1k"	-s 1k	-f $VFILE
writev	"writev_u10k"	-s 10k	-f $VFILE
writev	"writev_u100k"	-s 100k	-f $VFILE
# endif

writev	"writev_n1k"	-s 1k	-f /dev/null
writev	"writev_n10k"	-s 10k	-f /dev/null
writev	"writev_n100k"	-s 100k	-f /dev/null

pread	"pread_t1k"	-s 1k	-f $TFILE
pread	"pread_t10k"	-s 10k	-f $TFILE
pread	"pread_t100k"	-s 100k	-f $TFILE

# ifdef EXTENDED
pread	"pread_u1k"	-s 1k	-f $VFILE
pread	"pread_u10k"	-s 10k	-f $VFILE
pread	"pread_u100k"	-s 100k	-f $VFILE
# endif

pread	"pread_z1k"	-s 1k	-f /dev/zero
pread	"pread_z10k"	-s 10k	-f /dev/zero
pread	"pread_z100k"	-s 100k	-f /dev/zero
pread	"pread_zw100k"	-s 100k	-w	-f /dev/zero

pwrite	"pwrite_t1k"	-s 1k	-f $TFILE
pwrite	"pwrite_t10k"	-s 10k	-f $TFILE
pwrite	"pwrite_t100k"	-s 100k	-f $TFILE

# ifdef EXTENDED
pwrite	"pwrite_u1k"	-s 1k	-f $VFILE
pwrite	"pwrite_u10k"	-s 10k	-f $VFILE
pwrite	"pwrite_u100k"	-s 100k	-f $VFILE
# endif

pwrite	"pwrite_n1k"	-s 1k	-f /dev/null
pwrite	"pwrite_n10k"	-s 10k	-f /dev/null
pwrite	"pwrite_n100k"	-s 100k	-f /dev/null

#endif /* _READWRITE_ */
