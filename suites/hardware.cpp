#ifndef _HARDWARE_
#define _HARDWARE_

memrand	"memrand_1k"	-s 1k
memrand	"memrand_2k"	-s 2k
memrand	"memrand_4k"	-s 4k
memrand	"memrand_8k"	-s 8k
memrand	"memrand_16k"	-s 16k
memrand	"memrand_32k"	-s 32k
memrand	"memrand_64k"	-s 64k
memrand	"memrand_128k"	-s 128k
memrand	"memrand_256k"	-s 256k
memrand	"memrand_512k"	-s 512k
memrand	"memrand_1m"	-s 1m
memrand	"memrand_2m"	-s 2m
memrand	"memrand_4m"	-s 4m
memrand	"memrand_8m"	-s 8m
memrand	"memrand_16m"	-s 16m
memrand	"memrand_32m"	-s 32m
memrand	"memrand_64m"	-s 64m
memrand	"memrand"	-s 128m
# ifdef EXTENDED
memrand	"memrand_256m"	-s 256m
memrand	"memrand_512m"	-s 512m
memrand	"memrand_1g"	-s 1g
memrand	"memrand_2g"	-s 2g
# endif

recurse	"recurse"
recurse	"recurse_1k"	-d 1k
recurse	"recurse_10k"	-d 10k

#endif /* _HARDWARE_ */
