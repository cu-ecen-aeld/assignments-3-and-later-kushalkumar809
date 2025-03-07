#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h> 
#include <netdb.h>
#include <netinet/in.h> 
#include <unistd.h>

#define port "9000"
#define buff_len 500


int main (int argc,char **argv)
{
struct addrinfo serv;
struct addrinfo *servinfo;

memset (&serv, 0, sizeof(serv));

serv.ai_family = AF_INET;
serv.ai_socktype = SOCK_STREAM;
serv.ai_flags = 0;
serv.ai_protocol = 0;



if (getaddrinfo(NULL,port,&serv,&servinfo)!=0)
{
int errnum = errno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERR);
syslog(LOG_ERR,"GETADDRINFO ERROR %d : %s",errnum,errmesg);
closelog();
return -1;
}

int socketfd = socket((*servinfo).ai_family, (*servinfo).ai_socktype, (*servinfo).ai_protocol);

if (socketfd == -1)
{
int errnum = errno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERR);
syslog(LOG_ERR,"SOCKET OPENING ERROR %d : %s",errnum,errmesg);
closelog();
freeaddrinfo(servinfo);
return -1;
}



if (bind(socketfd,(*servinfo).ai_addr, (*servinfo).ai_addrlen) !=0)
{
int errnum = errno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERR);
syslog(LOG_ERR,"SOCKET IP BINDING ERROR %d : %s",errnum,errmesg);
closelog();
freeaddrinfo(servinfo);
return -1;
}

openlog(NULL,0,LOG_INFO);
syslog(LOG_INFO,"SOCKET OPEN SUCCESS");
closelog();

if (listen(socketfd,0) == -1)
{
int errnum = errno;
char *errmesg = strerror(errno);
openlog(NULL,0,LOG_ERR);
syslog(LOG_ERR,"Listen to port %s failed (%d) : %s",port ,errnum ,errmesg);
closelog();
freeaddrinfo(servinfo);
return -1;
}

openlog(NULL,0,LOG_INFO);
syslog(LOG_INFO,"LISTENING TO PORT %s",port);
closelog();



for (;;)
{
struct sockaddr_storage client_addr;
socklen_t client_addr_len = sizeof(client_addr);
int acceptedfd = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len); 
	if(acceptedfd == -1)
	{
	openlog(NULL,0,LOG_INFO);
	syslog(LOG_INFO,"Connection accept failed");
	closelog();
	}
	
	else 
	{
	        openlog(NULL,0,LOG_INFO);
		//syslog(LOG_INFO,"Accepted Connection from %s",client_addr);
		closelog();
		
		///printf("\nAccepted Connection from %s\n",client_addr);

		for (;;)
		{
		char buff[buff_len];
		recv(acceptedfd,buff,sizeof(buff),0);
		printf("1: %s",buff);
		
		FILE *fptr = fopen("/var/tmp/aesdsocketdata","a");
		fputs(buff,fptr);
		fclose(fptr);
		FILE *rptr = fopen("/var/tmp/aesdsocketdata","r");
		
		int line[10];
		size_t read = fread(line,sizeof(line),sizeof(*line),rptr);
		send(acceptedfd,line, sizeof(line) ,0);
		
		char line[256];
		 while (fgets(line, sizeof(line), fptr) != NULL) 
		{
        		printf("2 :%s", line); // Print the line
        		send(acceptedfd,line, sizeof(line) ,0);
   		}
   		
		
		fclose(rptr);
		
		}
		
	}



}



closelog();
freeaddrinfo(servinfo);
}
