#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>

int main (int argc,char ** argv)
{

if (argc == 3)
	{
		FILE* fptr = fopen(argv[1],"a");
	
		if (fptr == NULL)
		{
			int errnumber = errno;
			char *errmsg = strerror(errno);
			openlog(NULL,0,LOG_ERR);
			syslog(LOG_ERR,"Error Number = %d : %s \n",errnumber,errmsg);
			closelog();
			return 1;	
		}
		
		else
			{
				fprintf(fptr,"%s",argv[2]);
				openlog(NULL,0,LOG_USER);
				syslog(LOG_DEBUG,"Writing %s to %s",argv[3],argv[2]);
				fclose(fptr);
				closelog();
				return 0;
			}
	}
else
	{
	openlog(NULL,0,LOG_ERR);
	syslog(LOG_ERR,"Expected %d arguments instead got %d",2,argc-1);
	closelog();
	return 1;
	} 
	
}
