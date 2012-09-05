#ifndef _NETWORK_
#define _NETWORK_

getsockname	"getsockname"
getpeername	"getpeername"

fcntl_ndelay	"fcntl_ndelay"

socket	"socket_u"	-B 600
socket	"socket_i"	-B 400	-f PF_INET

socketpair	"socketpair"	-B 300

setsockopt	"setsockopt"	

bind	"bind"	-B 1k

listen	"listen"

# ifdef EXTENDED
connection	"connection"	-B 40
# endif
connection	"conn_connect"	-B 125	-c
# ifdef EXTENDED
connection	"conn_accept"	-B 125	-a
# endif

# ifdef EXTENDED
close_tcp	"close_tcp"	-B 100
# endif

poll	"poll_10"	-n 10
poll	"poll_100"	-n 100
poll	"poll_1000"	-n 1000

poll	"poll_w10"	-n 10	-w 1
poll	"poll_w100"	-n 100	-w 10
poll	"poll_w1000"	-n 1000	-w 100

select	"select_10"	-n 10
select	"select_100"	-n 100
select	"select_1000"	-n 1000

select	"select_w10"	-n 10	-w 1
select	"select_w100"	-n 100	-w 10
select	"select_w1000"	-n 1000	-w 100

#include "localtcp.cpp"

#endif /* _NETWORK_ */
