/*  This program will create a shared memory block
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

#define SHMSZ 50 /* Shared memory size */
#define NUMSEC 60  /* 60 seconds of lifetime */
#define SHMNAME 5678 /* Name for the shared memory */
int main() 
{
int id;  /* shared memory id */
key_t key;
char *shm;
int i;


/* Create shared memory */
id = shmget(SHMNAME, SHMSZ, IPC_CREAT | 0666);
if ( id < 0) {
   perror("shmget");
   printf("Server: Creation of shared memory failed\n");
   exit(1);
}


/* Attached shared memory */
shm = shmat(id, NULL, 0);
if (shm == (char *)-1) { 
   perror("shmat");
   printf("Server: Couldn't attach shared memory\n");
   exit(1);
}

/* Initialize shared memory with "Aloha" followed by null characther */
shm[0] = 'A';
shm[1] = 'l';
shm[2] = 'o';
shm[3] = 'h';
shm[4] = 'a';
shm[5] = '\0';

printf("Server: shared memory %d is turned on for %d seconds\n",SHMNAME,NUMSEC);
for (; shm[0] != 't'; sleep(1));

shmdt(shm); /* detach shared memory */
shmctl(id, IPC_RMID, NULL); /* remove shared memory */

printf("Server: shared memory %d is turned off\n",SHMNAME);
}


