#ifndef _MEMORY_
#define _MEMORY_

mmap	"mmap_z8k"	-l 8k	-f /dev/zero
mmap	"mmap_z128k"	-l 128k	-f /dev/zero
mmap	"mmap_t8k"	-l 8k	-f $TFILE
mmap	"mmap_t128k"	-l 128k	-f $TFILE
# ifdef EXTENDED
mmap	"mmap_u8k"	-l 8k	-f $VFILE
mmap	"mmap_u128k"	-l 128k	-f $VFILE
# endif
mmap	"mmap_a8k"	-l 8k	-f MAP_ANON
mmap	"mmap_a128k"	-l 128k	-f MAP_ANON

mmap	"mmap_rz8k"	-l 8k	-r	-f /dev/zero
mmap	"mmap_rz128k"	-l 128k	-r	-f /dev/zero
mmap	"mmap_rt8k"	-l 8k	-r	-f $TFILE
mmap	"mmap_rt128k"	-l 128k	-r	-f $TFILE
# ifdef EXTENDED
mmap	"mmap_ru8k"	-l 8k	-r	-f $VFILE
mmap	"mmap_ru128k"	-l 128k	-r	-f $VFILE
# endif
mmap	"mmap_ra8k"	-l 8k	-r	-f MAP_ANON
mmap	"mmap_ra128k"	-l 128k	-r	-f MAP_ANON

mmap	"mmap_wz8k"	-l 8k	-w	-f /dev/zero
mmap	"mmap_wz128k"	-l 128k	-w	-f /dev/zero
mmap	"mmap_wt8k"	-l 8k	-w	-f $TFILE
mmap	"mmap_wt128k"	-l 128k	-w	-f $TFILE
# ifdef EXTENDED
mmap	"mmap_wu8k"	-l 8k	-w	-f $VFILE
mmap	"mmap_wu128k"	-l 128k	-w	-f $VFILE
# endif
mmap	"mmap_wa8k"	-l 8k	-w	-f MAP_ANON
mmap	"mmap_wa128k"	-l 128k	-w	-f MAP_ANON

# ifdef EXTENDED
mmap_frag	"mmap_frag"	-B 10000	-C 1000
# endif

munmap	"unmap_z8k"	-l 8k	-f /dev/zero
munmap	"unmap_z128k"	-l 128k	-f /dev/zero
munmap	"unmap_t8k"	-l 8k	-f $TFILE
munmap	"unmap_t128k"	-l 128k	-f $TFILE
# ifdef EXTENDED
munmap	"unmap_u8k"	-l 8k	-f $VFILE
munmap	"unmap_u128k"	-l 128k	-f $VFILE
# endif
munmap	"unmap_a8k"	-l 8k	-f MAP_ANON
munmap	"unmap_a128k"	-l 128k	-f MAP_ANON

munmap	"unmap_rz8k"	-l 8k	-r	-f /dev/zero
munmap	"unmap_rz128k"	-l 128k	-r	-f /dev/zero
munmap	"unmap_rt8k"	-l 8k	-r	-f $TFILE
munmap	"unmap_rt128k"	-l 128k	-r	-f $TFILE
# ifdef EXTENDED
munmap	"unmap_ru8k"	-l 8k	-r	-f $VFILE
munmap	"unmap_ru128k"	-l 128k	-r	-f $VFILE
# endif
munmap	"unmap_ra8k"	-l 8k	-r	-f MAP_ANON
munmap	"unmap_ra128k"	-l 128k	-r	-f MAP_ANON

munmap	"unmap_wz8k"	-l 8k	-w	-f /dev/zero
munmap	"unmap_wz128k"	-l 128k	-w	-f /dev/zero
munmap	"unmap_wt8k"	-l 8k	-w	-f $TFILE
munmap	"unmap_wt128k"	-l 128k	-w	-f $TFILE
# ifdef EXTENDED
munmap	"unmap_wu8k"	-l 8k	-w	-f $VFILE
munmap	"unmap_wu128k"	-l 128k	-w	-f $VFILE
# endif
munmap	"unmap_wa8k"	-l 8k	-w	-f MAP_ANON
munmap	"unmap_wa128k"	-l 128k	-w	-f MAP_ANON

# ifdef EXTENDED
munmap_frag	"unmap_frag"	-B 2500	-C 1000
# endif

mprotect	"mprot_z8k"	-l 8k	-f /dev/zero	-B 1k
mprotect	"mprot_z128k"	-l 128k	-f /dev/zero	-B 1k
mprotect	"mprot_wz8k"	-l 8k	-w	-f /dev/zero	-B 1k
mprotect	"mprot_wz128k"	-l 128k	-w	-f /dev/zero	-B 800
mprotect	"mprot_twz8k"	-l 8k	-w	-t	-f /dev/zero	-B 900
mprotect	"mprot_tw128k"	-l 128k	-w	-t	-f /dev/zero	-B 500
mprotect	"mprot_tw4m"	-l 4m	-w	-t	-f /dev/zero	-B 40

msync	"msync_t8k"	-l 8k	-f $TFILE
msync	"msync_t128k"	-l 128k	-f $TFILE
# ifdef EXTENDED
msync	"msync_u8k"	-l 8k	-f $VFILE
msync	"msync_u128k"	-l 128k	-f $VFILE
# endif
msync	"msync_ta8k"	-l 8k	-f $TFILE	-a
msync	"msync_ta128k"	-l 128k	-f $TFILE	-a
# ifdef EXTENDED
msync	"msync_ua8k"	-l 8k	-f $VFILE	-a
msync	"msync_ua128k"	-l 128k	-f $VFILE	-a
# endif
msync	"msync_ti8k"	-l 8k	-f $TFILE	-i
msync	"msync_ti128k"	-l 128k	-f $TFILE	-i
# ifdef EXTENDED
msync	"msync_ui8k"	-l 8k	-f $VFILE	-i
msync	"msync_ui128k"	-l 128k	-f $VFILE	-i
# endif
msync	"msync_tai8k"	-l 8k	-f $TFILE	-a	-i
msync	"msync_tai128k"	-l 128k	-f $TFILE	-a	-i
# ifdef EXTENDED
msync	"msync_uai8k"	-l 8k	-f $VFILE	-a	-i
msync	"msync_uai128k"	-l 128k	-f $VFILE	-a	-i
# endif

msync	"msync_rt8k"	-l 8k	-r	-f $TFILE
msync	"msync_rt128k"	-l 128k	-r	-f $TFILE
# ifdef EXTENDED
msync	"msync_ru8k"	-l 8k	-r	-f $VFILE
msync	"msync_ru128k"	-l 128k	-r	-f $VFILE
# endif
msync	"msync_rat8k"	-l 8k	-r	-f $TFILE	-a
msync	"msync_rat128k"	-l 128k	-r	-f $TFILE	-a
# ifdef EXTENDED
msync	"msync_rau8k"	-l 8k	-r	-f $VFILE	-a
msync	"msync_rau128k"	-l 128k	-r	-f $VFILE	-a
# endif
msync	"msync_rit8k"	-l 8k	-r	-f $TFILE	-i
msync	"msync_rit128k"	-l 128k	-r	-f $TFILE	-i
# ifdef EXTENDED
msync	"msync_riu8k"	-l 8k	-r	-f $VFILE	-i
msync	"msync_riu128k"	-l 128k	-r	-f $VFILE	-i
# endif
msync	"msync_rait8k"	-l 8k	-r	-f $TFILE	-a	-i
msync	"msync_rait128k"	-l 128k	-r	-f $TFILE	-a	-i
# ifdef EXTENDED
msync	"msync_raiu8k"	-l 8k	-r	-f $VFILE	-a	-i
msync	"msync_raiu128k"	-l 128k	-r	-f $VFILE	-a	-i
# endif

msync	"msync_wt8k"	-l 8k	-w	-f $TFILE
msync	"msync_wt128k"	-l 128k	-w	-f $TFILE
# ifdef EXTENDED
msync	"msync_wu8k"	-l 8k	-w	-f $VFILE
msync	"msync_wu128k"	-l 128k	-w	-f $VFILE
# endif
msync	"msync_wat8k"	-l 8k	-w	-f $TFILE	-a
msync	"msync_wat128k"	-l 128k	-w	-f $TFILE	-a
# ifdef EXTENDED
msync	"msync_wau8k"	-l 8k	-w	-f $VFILE	-a
msync	"msync_wau128k"	-l 128k	-w	-f $VFILE	-a
# endif
msync	"msync_wit8k"	-l 8k	-w	-f $TFILE	-i
msync	"msync_wit128k"	-l 128k	-w	-f $TFILE	-i
# ifdef EXTENDED
msync	"msync_wiu8k"	-l 8k	-w	-f $VFILE	-i
msync	"msync_wiu128k"	-l 128k	-w	-f $VFILE	-i
# endif
msync	"msync_wait8k"	-l 8k	-w	-f $TFILE	-a	-i
msync	"msync_wait128k"	-l 128k	-w	-f $TFILE	-a	-i
# ifdef EXTENDED
msync	"msync_waiu8k"	-l 8k	-w	-f $VFILE	-a	-i
msync	"msync_waiu128k"	-l 128k	-w	-f $VFILE	-a	-i
# endif

#endif /* _MEMORY_ */
