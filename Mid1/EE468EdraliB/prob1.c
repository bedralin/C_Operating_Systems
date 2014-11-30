/* 
 * Bronson Edralin
 * EE 468 Midterm Exam 1
 *
 * Do not change the child.
 *
 * Modify the program so that children 0, 1, 2 are created then terminated.
 * Then children 3, 4, 5 are created then terminated.  Then
 * children 6, 7, 8 are created then terminated.  Note that
 * a child goes to sleep for one second.  So your program should run
 * for approximately 3 seconds.
 */
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void) {
    pid_t childPID;
    int k;
    int return_status;

    for (k=0; k<9; k++) {

	childPID = fork();

	if ( ((k+1)%3) == 0) 
	    waitpid(childPID, &return_status, 0);
    

	if (childPID == 0) {  /* Start of Child */
	    printf("Child %d: Created\n",k);
	    sleep(1);
	    printf("Child %d: Terminated\n",k);
	    return 0;
	}                  /* End of Child*/

	else if (childPID < 0) {
	    printf("\n Creating child %d failed, quitting!\n",k);
	    return 1;
	} 
    }
    return 0;
}
