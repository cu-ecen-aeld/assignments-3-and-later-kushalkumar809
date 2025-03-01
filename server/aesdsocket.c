#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>

#define port="9000";

int main ()
{
struct addrinfo serv;
struct addrinfo *servinfo;

memset (&serv, 0, sizeof(serv));

socket.ai_family = AF_inet;
socket.ai_socktype = SOCK_STREAM;
socket.ai_flags = 0;
socket.protocol = 0;



if (getaddrinfo(NULL,port,&socket,&servinfo);!=0)
{
int errnum = erno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERROR);
syslog(LOG_ERR,"GETADDRINFO ERROR %d : %s",errnum,errmesg);
closelog();
return -1;
}

int socketfd = socket((*servinfo).ai_family, (*servinfo).ai_socktype, (*servinfo).ai_protocol);

if (socketfd == -1)
{
int errnum = erno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERROR);
syslog(LOG_ERR,"SOCKET OPENING ERROR %d : %s",errnum,errmesg);
closelog();
freeaddrinfo(servinfo);
return -1;
}



if (bind((*servinfo).ai_addr, (*servinfo).ai_addrlen)) !=0)
{
int errnum = erno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERROR);
syslog(LOG_ERR,"SOCKET IP BINDING ERROR %d : %s",errnum,errmesg);
closelog();
freeaddrinfo(servinfo);
return -1;
}

openlog(NULL,0,LOG_INFO);
syslog(LOG_INFO,"SOCKET OPEN SUCCESS");
closelog();

if (listen(socketfd,port) == -1)
{
int errnum = erno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERROR);
syslog(LOG_ERR,"Listen to port %d failed (%d) : %s",port ,errnum ,errmesg);
closelog();
freeaddrinfo(servinfo);
return -1;
}

openlog(NULL,0,LOG_INFO);
syslog(LOG_INFO,"LISTENING TO PORT %d"port);
acceptedfd = accept(socketfd, (*servinfo).ai_adrr, (*servinfo).addr_len); 

if(acceptedfd == -1)
{
syslog(LOG_INFO,"No Client Connected");
closelog();
freeaddrinfo(servinfo);
return -1;
}




closelog();

freeaddrinfo(servinfo);
}
