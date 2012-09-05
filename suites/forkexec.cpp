#ifndef _FORKEXEC_
#define _FORKEXEC_

fork	"fork_100"	-B 15
# ifdef EXTENDED
fork	"fork_1000"	-B 10
# endif

exit	"exit_100"	-B 50
exit	"exit_100_nolibc"	-e	-B 60
# ifdef EXTENDED
exit	"exit_1000"	-B 50
exit	"exit_1000_nolibc"	-e	-B 60
# endif

exec	"exec"

execw	"execw"

execw	"execw_1r"	-r 1
execw	"execw_10r"	-r 10
execw	"execw_100r"	-r 100
# ifdef EXTENDED
execw	"execw_1000r"	-r 1000
# endif

execw	"execw_32n"	-n 32	-p 8192
execw	"execw_64n"	-n 64	-p 4096
execw	"execw_128n"	-n 128	-p 2048
execw	"execw_256n"	-n 256	-p 1024
execw	"execw_512n"	-n 512	-p 512
execw	"execw_1024n"	-n 1024	-p 256
# ifdef EXTENDED
execw	"execw_2048n"	-n 2048	-p 128
execw	"execw_4096n"	-n 4096	-p 64
execw	"execw_8192n"	-n 8192	-p 32
# endif

# ifdef EXTENDED
execw	"execw_8192n1000r"	-n 8192	-p 32	-r 1000
# endif

execw	"execw_32nt"	-n 32	-p 8192	-t
execw	"execw_64nt"	-n 64	-p 4096	-t
execw	"execw_128nt"	-n 128	-p 2048	-t

execw	"execw_32ntv"	-n 32	-p 8192	-t	-v
execw	"execw_64ntv"	-n 64	-p 4096	-t	-v
execw	"execw_128ntv"	-n 128	-p 2048	-t	-v
# ifdef EXTENDED
execw	"execw_256ntv"	-n 256	-p 1024	-t	-v
execw	"execw_512ntv"	-n 512	-p 512	-t	-v
execw	"execw_1024ntv"	-n 1024	-p 256	-t	-v
execw	"execw_2048ntv"	-n 2048	-p 128	-t	-v
execw	"execw_4096ntv"	-n 4096	-p 64	-t	-v
execw	"execw_8192ntv"	-n 8192	-p 32	-t	-v
# endif

# ifdef EXTENDED
execw	"execw_8192ntv1000r"	-n 8192	-p 32	-t	-v	-r 1000
# endif

posix_spawn	"posix_spawn"

posix_spawn	"posix_spawn_1r"	-r 1
posix_spawn	"posix_spawn_10r"	-r 10
posix_spawn	"posix_spawn_100r"	-r 100
# ifdef EXTENDED
posix_spawn	"posix_spawn_1000r"	-r 1000
# endif

posix_spawn	"posix_spawn_32n"	-n 32	-p 8192
posix_spawn	"posix_spawn_64n"	-n 64	-p 4096
posix_spawn	"posix_spawn_128n"	-n 128	-p 2048
posix_spawn	"posix_spawn_256n"	-n 256	-p 1024
posix_spawn	"posix_spawn_512n"	-n 512	-p 512
posix_spawn	"posix_spawn_1024n"	-n 1024	-p 256
# ifdef EXTENDED
posix_spawn	"posix_spawn_2048n"	-n 2048	-p 128
posix_spawn	"posix_spawn_4096n"	-n 4096	-p 64
posix_spawn	"posix_spawn_8192n"	-n 8192	-p 32
# endif

# ifdef EXTENDED
posix_spawn	"posix_spawn_8192n1000r"	-n 8192	-p 32	-r 1000
# endif

posix_spawn	"posix_spawn_32nt"	-n 32	-p 8192	-t
posix_spawn	"posix_spawn_64nt"	-n 64	-p 4096	-t
posix_spawn	"posix_spawn_128nt"	-n 128	-p 2048	-t
# ifdef EXTENDED
posix_spawn	"posix_spawn_256nt"	-n 256	-p 1024	-t
posix_spawn	"posix_spawn_512nt"	-n 512	-p 512	-t
posix_spawn	"posix_spawn_1024nt"	-n 1024	-p 256	-t
posix_spawn	"posix_spawn_2048nt"	-n 2048	-p 128	-t
posix_spawn	"posix_spawn_4096nt"	-n 4096	-p 64	-t
posix_spawn	"posix_spawn_8192nt"	-n 8192	-p 32	-t
# endif

# ifdef EXTENDED
posix_spawn	"posix_spawn_8192nt1000r"	-n 8192	-p 32	-t	-r 1000
# endif

system	"system"

#endif /* _FORKEXEC_ */
