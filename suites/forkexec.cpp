#ifndef _FORKEXEC_
#define _FORKEXEC_

fork	"fork_100"	-B 100
# ifdef EXTENDED
fork	"fork_1000"	-B 1000
# endif

exit	"exit_100"	-B 100
exit	"exit_100_nolibc"	-e	-B 100
# ifdef EXTENDED
exit	"exit_1000"	-B 1000
exit	"exit_1000_nolibc"	-e	-B 1000
# endif

exec	"exec"	-B 100

execw	"execw"	-B 175

execw	"execw_1r"	-B 125	-r 1
execw	"execw_10r"	-B 100	-r 10
execw	"execw_100r"	-B 50	-r 100
# ifdef EXTENDED
execw	"execw_1000r"	-B 25	-r 1000
# endif

execw	"execw_32n"	-B 125	-n 32	-p 8192
execw	"execw_64n"	-B 125	-n 64	-p 4096
execw	"execw_128n"	-B 125	-n 128	-p 2048
execw	"execw_256n"	-B 100	-n 256	-p 1024
execw	"execw_512n"	-B 100	-n 512	-p 512
execw	"execw_1024n"	-B 70	-n 1024	-p 256
# ifdef EXTENDED
execw	"execw_2048n"	-B 35	-n 2048	-p 128
execw	"execw_4096n"	-B 15	-n 4096	-p 64
execw	"execw_8192n"	-B 5	-n 8192	-p 32
# endif

# ifdef EXTENDED
execw	"execw_8192n1000r"	-B 5	-n 8192	-p 32	-r 1000
# endif

execw	"execw_32nt"	-B 25	-n 32	-p 8192	-t
execw	"execw_64nt"	-B 15	-n 64	-p 4096	-t
execw	"execw_128nt"	-B 10	-n 128	-p 2048	-t

execw	"execw_32ntv"	-B 200	-n 32	-p 8192	-t	-v
execw	"execw_64ntv"	-B 200	-n 64	-p 4096	-t	-v
execw	"execw_128ntv"	-B 200	-n 128	-p 2048	-t	-v
# ifdef EXTENDED
execw	"execw_256ntv"	-B 200	-n 256	-p 1024	-t	-v
execw	"execw_512ntv"	-B 200	-n 512	-p 512	-t	-v
execw	"execw_1024ntv"	-B 200	-n 1024	-p 256	-t	-v
execw	"execw_2048ntv"	-B 200	-n 2048	-p 128	-t	-v
execw	"execw_4096ntv"	-B 200	-n 4096	-p 64	-t	-v
execw	"execw_8192ntv"	-B 200	-n 8192	-p 32	-t	-v
# endif

# ifdef EXTENDED
execw	"execw_8192ntv1000r"	-B 200	-n 8192	-p 32	-t	-v	-r 1000
# endif

posix_spawn	"posix_spawn"	-B 200

posix_spawn	"posix_spawn_1r"	-B 200	-r 1
posix_spawn	"posix_spawn_10r"	-B 200	-r 10
posix_spawn	"posix_spawn_100r"	-B 200	-r 100
# ifdef EXTENDED
posix_spawn	"posix_spawn_1000r"	-B 200	-r 1000
# endif

posix_spawn	"posix_spawn_32n"	-B 200	-n 32	-p 8192
posix_spawn	"posix_spawn_64n"	-B 200	-n 64	-p 4096
posix_spawn	"posix_spawn_128n"	-B 200	-n 128	-p 2048
posix_spawn	"posix_spawn_256n"	-B 200	-n 256	-p 1024
posix_spawn	"posix_spawn_512n"	-B 200	-n 512	-p 512
posix_spawn	"posix_spawn_1024n"	-B 200	-n 1024	-p 256
# ifdef EXTENDED
posix_spawn	"posix_spawn_2048n"	-B 200	-n 2048	-p 128
posix_spawn	"posix_spawn_4096n"	-B 200	-n 4096	-p 64
posix_spawn	"posix_spawn_8192n"	-B 200	-n 8192	-p 32
# endif

# ifdef EXTENDED
posix_spawn	"posix_spawn_8192n1000r"	-B 200	-n 8192	-p 32	-r 1000
# endif

posix_spawn	"posix_spawn_32nt"	-B 200	-n 32	-p 8192	-t
posix_spawn	"posix_spawn_64nt"	-B 200	-n 64	-p 4096	-t
posix_spawn	"posix_spawn_128nt"	-B 200	-n 128	-p 2048	-t
# ifdef EXTENDED
posix_spawn	"posix_spawn_256nt"	-B 200	-n 256	-p 1024	-t
posix_spawn	"posix_spawn_512nt"	-B 200	-n 512	-p 512	-t
posix_spawn	"posix_spawn_1024nt"	-B 200	-n 1024	-p 256	-t
posix_spawn	"posix_spawn_2048nt"	-B 200	-n 2048	-p 128	-t
posix_spawn	"posix_spawn_4096nt"	-B 200	-n 4096	-p 64	-t
posix_spawn	"posix_spawn_8192nt"	-B 200	-n 8192	-p 32	-t
# endif

# ifdef EXTENDED
posix_spawn	"posix_spawn_8192nt1000r"	-B 200	-n 8192	-p 32	-t	-r 1000
# endif

system	"system"	-B 50

#endif /* _FORKEXEC_ */
