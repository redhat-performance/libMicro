#ifndef _HARDWARE_
#define _HARDWARE_

memrand	"memrand_1k"	-s 1k	-B 32m
memrand	"memrand_2k"	-s 2k	-B 32m
memrand	"memrand_4k"	-s 4k	-B 32m
memrand	"memrand_8k"	-s 8k	-B 32m
memrand	"memrand_16k"	-s 16k	-B 32m
memrand	"memrand_32k"	-s 32k	-B 32m
memrand	"memrand_64k"	-s 64k	-B 24m
memrand	"memrand_128k"	-s 128k	-B 24m
memrand	"memrand_256k"	-s 256k	-B 24m
memrand	"memrand_512k"	-s 512k	-B 24m
memrand	"memrand_1m"	-s 1m	-B 24m
memrand	"memrand_2m"	-s 2m	-B 24m
memrand	"memrand_4m"	-s 4m	-B 24m
memrand	"memrand_8m"	-s 8m	-B 24m
memrand	"memrand_16m"	-s 16m	-B 24m
memrand	"memrand_32m"	-s 32m	-B 24m
memrand	"memrand_64m"	-s 64m	-B 16m
memrand	"memrand"	-s 128m	-B 32m
# ifdef EXTENDED
memrand	"memrand_256m"	-s 256m	-B 64m
memrand	"memrand_512m"	-s 512m	-B 128m
memrand	"memrand_1g"	-s 1g	-B 256m
memrand	"memrand_2g"	-s 2g	-B 512m
# endif

recurse	"recurse"	-B 200k
recurse	"recurse_1k"	-d 1k	-B 10k
recurse	"recurse_10k"	-d 10k	-B 1k

#endif /* _HARDWARE_ */
