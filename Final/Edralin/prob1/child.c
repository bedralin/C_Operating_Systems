/* Bronson Edralin
 * EE468 OS
 * Final Exam
 * Problem #1
 * 12/18/14
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main( )
{
pid_t	pid;
int	i;

for (i = 0;  i < 100;  i++) {
    pid = fork( );
    if (pid < 0) {  
	fprintf(stderr, "Fork Failed");
	return 1;
    }	
    else if (pid == 0) {
	printf("I am child # %d\n", i);
	return 0;
    }
    else {  /* parent process */
	wait(NULL);
	//printf(“All child processes are done \n”);
    }
}
printf("All child processes are done \n");
return 0;

} /* end of main */

