/* Bronson Edralin
 * EE468 Operating Systems
 * HW 3
 * 9/17/14
 * 
 * This is the client. Run it by doing this:
 * ./server
 * ./client wow
 *
 *  This program will create a shared memory block
 *  that can be accessed by client programs.  The
 *  The server and shared memory block will terminate
 *  terminate when the first byte of the shared memory
 *  is 't', i.e., shm[0] = 't'.  The server will check
 *  this byte every second.
 */

#include <sys/types.h> /* for shared memory */
#include <sys/ipc.h>   /* for shared memory */
#include <sys/shm.h>   /* for shared memory */
#include <stdio.h>
#include <stdlib.h>  /* for exit() */
#include <unistd.h>  /* for sleep() */
#include <string.h>

#define SHMSZ 50 /* Shared memory size */
#define NUMSEC 60  /* 60 seconds of lifetime */
#define SHMNAME 5678 /* Name for the shared memory */

int main(int argc, char *argv[]) {
    int shmid;
    key_t key;
    char *shm, *s;
   
    key = 5678; // We need to get the segment named "5678", created by the server
    if (argc != 2) {
	printf("\nError: You must have only 1 ARGUMENT!\n\n");
	exit(1);
    }

    else {
    
	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) { // Locate the segment
	    perror("shmget");
	    exit(1);
	}
	if ((shm = shmat(shmid, (void *)0, 0)) == ((char *)-1)) { // We attach segment
	    printf("\nShared Memory not successfully attached...\n\n");
	    perror("shmat");
	    exit(1);
	}
	printf("\nShared Memory successfully attached...\n");
	printf("Current Shared Memory = %s\n",shm);
	strncpy(shm, argv[1], SHMSZ);  // Replace old shared memory
	printf("New shared memory = %s\n\n",shm);
    }
    return 0;
    // Now read what the server put in the memory and print it
    //for (s = shm; s != NULL; s++) putchar(*s);
    //putchar('\n');
    // Finally, change the first character of the segment to '*',
    // indicating we have read * the segment.
    //*shm = '*';
    //exit(0); 
}
 
