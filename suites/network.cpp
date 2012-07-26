#ifndef _NETWORK_
#define _NETWORK_

getsockname	"getsockname"
getpeername	"getpeername"

fcntl_ndelay	"fcntl_ndelay"

socket	"socket_u"	-B 10k
socket	"socket_i"	-B 10k	-f PF_INET

socketpair	"socketpair"	-B 25000

setsockopt	"setsockopt"	

bind	"bind"	-B 200

listen	"listen"	-B 50k

# ifdef EXTENDED
connection	"connection"	-B 512
# endif
connection	"conn_connect"	-B 512	-c
# ifdef EXTENDED
connection	"conn_accept"	-B 512	-a
# endif

close_tcp	"close_tcp"	-B 1k

poll	"poll_10"	-n 10
poll	"poll_100"	-n 100	-B 10k
poll	"poll_1000"	-n 1000	-B 1k

poll	"poll_w10"	-n 10	-w 1
poll	"poll_w100"	-n 100	-w 10	-B 10k
poll	"poll_w1000"	-n 1000	-w 100	-B 1k

select	"select_10"	-n 10
select	"select_100"	-n 100	-B 10k
select	"select_1000"	-n 1000	-B 1k

select	"select_w10"	-n 10	-w 1
select	"select_w100"	-n 100	-w 10	-B 10k
select	"select_w1000"	-n 1000	-w 100	-B 1k

#include "localtcp.cpp"

#endif /* _NETWORK_ */
