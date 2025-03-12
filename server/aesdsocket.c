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
#include <fcntl.h>

#define port "9000"
#define BUFFER_SIZE 500

volatile sig_atomic_t keep_running = 1;
int socketfd;
int acceptedfd;
const char *filename = "/var/tmp/aesdsocketdata";
struct addrinfo serv;
struct addrinfo *servinfo;

void signal_handler(int signo) {
    if (signo == SIGINT || signo == SIGTERM) {
        printf("Caught signal, exiting\n");
        if (acceptedfd > 0)
    	{
            close(acceptedfd);
    	}
    	if (socketfd > 0)
    	{
            close(socketfd);
    	}
        keep_running = 0;
        remove(filename);
        shutdown(socketfd, SHUT_RDWR);
	close(socketfd);
	freeaddrinfo(servinfo);
	closelog();
    }
}


int main (int argc,char **argv)
{

signal(SIGINT, signal_handler);
signal(SIGTERM, signal_handler);

int daemon_mode = 0;
    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        daemon_mode = 1;
    }
    



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

socketfd = socket((*servinfo).ai_family, (*servinfo).ai_socktype, (*servinfo).ai_protocol);

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

if (listen(socketfd,10) == -1)
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


struct sockaddr_storage client_addr;
socklen_t client_addr_len = sizeof(client_addr);


while (keep_running)
{

 if (daemon_mode) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        if (pid > 0) {
            exit(0);
        }

        setsid();
        chdir("/");
     
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        int fd = open("/dev/null", O_RDWR);
        if (fd != -1) {
            dup2(fd, STDIN_FILENO);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }
    }



acceptedfd = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len); 
	if(acceptedfd == -1)
	{
	openlog(NULL,0,LOG_INFO);
	syslog(LOG_INFO,"Connection accept failed");
	closelog();
	}
	
	else 
	{

		ssize_t bytes_read;
		char buffer[BUFFER_SIZE];
		while ((bytes_read = read(acceptedfd, buffer, sizeof(buffer))) > 0) 
		{
		
				int s;
		char host[NI_MAXHOST], service[NI_MAXSERV];
		s=getnameinfo((struct sockaddr *)&client_addr,client_addr_len, host, NI_MAXHOST,service, NI_MAXSERV, NI_NUMERICSERV);
		if(s==0)
   		   {
   		   	openlog(NULL,0,LOG_INFO);
			syslog(LOG_INFO,"Accepted connection from %s", service);
			printf("Accepted connection from %s", service);
			fflush(stdout);
			closelog();
      		   }
		
			char *newline = memchr(buffer,'\n',bytes_read);
			if (newline)
			{
				
                	size_t line_length = newline - buffer + 1;
                	int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                	if (fd == -1) {
                    		perror("Error opening file");
                    	continue;
			}
			
			write(fd, buffer, line_length);
                	close(fd);
                	
                	int fd_read = open(filename, O_RDONLY);
                	
                	char buffer2[BUFFER_SIZE];
                	while ((bytes_read = read(fd_read, buffer2, sizeof(buffer2))) > 0) 
                	{
                   		send(acceptedfd, buffer2, bytes_read, 0);
               		 }
               		 
               		  close(fd_read);
	
			}
			
			
			else
           	 	{	
              	  		int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
               		 	if (fd == -1) {
                    			perror("Error opening file");
                    			continue;
                			      }		
               	 		write(fd, buffer, bytes_read);
                 		close(fd);
            		} 
			

		}

	}		

}
	return 0;

}
