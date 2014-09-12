/*  Bronson Edralin
 *  EE468 Operating Systems HW1
 *
 *  This is a simple shell program from
 *  rik0.altervista.org/snippetss/csimpleshell.html
 *  It's been modified a bit and comments were added.
 *
 *  It doesn't allow misdirection, e.g., <, >, >>, or |
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#define BUFFER_SIZE 80
#define ARR_SIZE 80
//#define DEBUG 1  /* In case you want debug messages */

typedef struct children {
    char *args[ARR_SIZE]; //command to execute
    int fd[2]; //pair of pipes per child
}Child;

Child child[10];



void
run_source(Child *child, int child_num, int array_count)	/* run the first part of the pipeline, cmd1 */
{
    int pid;	/* we don't use the process ID here, but you may wnat to print it for debugging */

    switch (pid = fork()) {

    case 0: /* child */
	dup2(child[child_num].fd[1], 1);    /* this end of the pipe becomes the standard output */
	close(child[child_num].fd[0]);	    /* this process don't need the other end */
	execvp(child[child_num].args[0], child[child_num].args);	/* run the command */
	perror(child[child_num].args[0]);    /* it failed! */

    default: /* parent does nothing */
	break;

    case -1:
	perror("fork");
	exit(1);
    }
}

void
run_dest(Child *child, int child_num, int array_count)  /* run the second part of the pipeline, cmd2 */
{
    int pid;

    switch (pid = fork()) {

    case 0: /* child */
	dup2(child[child_num-1].fd[0], 0);    /* this end of the pipe becomes the standard input */
	close(child[child_num-1].fd[1]);	    /* this process doesn't need the other end */
	execvp(child[child_num].args[0], child[child_num].args);	/* run the command */
	perror(child[child_num].args[0]);    /* it failed! */

    default: /* parent does nothing */
	break;

    case -1:
	perror("fork");
	exit(1);
    }
}


void parse_args(char *buffer, char** args, 
                size_t args_size, size_t *nargs)
{
/* 
 * size_t data type is defined in the 1999 ISO C standard (C99).
 * It is used to represent the sizes of objects. size_t is the
 * preferred way to declare arguments or variables that hold the
 * size of an object.
 */
    char *buf_args[args_size]; /* You need C99.  Note that args_size
                                  is normally a constant. */
    char **cp;  /* This is used as a pointer into the string array */
    char *wbuf;  /* String variable that has the command line */
    size_t i, j; 
    
    wbuf=buffer;
    buf_args[0]=buffer; 
    args[0] =buffer;
    /* cp is a pointer to buff_args */ 
    for(cp=buf_args; (*cp=strsep(&wbuf, " \n\t")) != NULL ;){
        if ((*cp != '\0') && (++cp >= &buf_args[args_size]))
            break; 
    }

/* 
 * Copy 'buf_args' into 'args'
 */    
    for (j=i=0; buf_args[i]!=NULL; i++){ 
        if(strlen(buf_args[i])>0)  /* Store only non-empty tokens */
            args[j++]=buf_args[i];
    }
    *nargs=j;
    args[j]=NULL;
}


int main(int argc, char *argv[], char *envp[]){
    char buffer[BUFFER_SIZE];
    char *args[ARR_SIZE];

    int *ret_status;
    size_t nargs;
    pid_t pid;

    int n=0;
    int n1=0;
    int n2=0;
    int array_count = 0;
   
    int status;
    int i, j;

    while(1){
        printf("ee468>> "); /* Prompt */
        fgets(buffer, BUFFER_SIZE, stdin); /* Read in command line */
              /* Parse the command line into args */
	printf("buffer is: %s\n",buffer);

        parse_args(buffer, args, ARR_SIZE, &nargs); 
 
        if (nargs==0) continue; /* Nothing entered so prompt again */
        if (!strcmp(args[0], "exit" )) exit(0); 
#ifdef DEBUG
            printf("args[0] is %s\n",args[0]);      
            printf("args[1] is %s\n",args[1]);
            printf("args[2] is %s\n",args[2]);
	    printf("argc is %d\n",argc);
#endif
	i = 0;	
	j = 0;

        for (i = 0; i < 9; i++) {
	    for (j = 0; child[i].args[j] != NULL; j++) {
		child[i].args[j] = 0;
	    }
	}


	for (n = 0; args[n] != NULL; n++) {
	    if ((strncmp(args[n], "|", 1) != 0) && (args[n] != NULL))  {
		child[array_count].args[n1] = args[n];
#ifdef DEBUG
		printf("child[array_count].args[n1] is %s\n",child[array_count].args[n1]);
#endif
		n1++;
	    }
	    if (strncmp(args[n], "|", 1) == 0) {
		child[array_count].args[n1] = 0;
		
#ifdef DEBUG
		printf("Array null is %s\n",child[array_count].args[n1]);
#endif
		n1 = 0;
		array_count++;
	    }
	}

	child[array_count].args[n1] = 0;
	n1 = 0;
	printf(" WOW \n");
	//n = 0;
	//runpipe(child, array_count);

	int pid, status;
	int child_num = 0;
	int pipe_count = 0;
	int count = 0;
	int i = 0;

	for (i=0; i <= array_count; i++) 
            pipe(child[i].fd);
            if (array_count == 0) {
                switch (pid = fork()) {
                
                case 0: // child
                    execvp(child[0].args[0], &child[0].args[0]);
                    perror(child[0].args[0]);
		    //break;

                default:  // parent does nothing
                    break;
                case -1:
                    perror("fork");
                    exit(1);
                }
                break;
            }
	else {

	while (child_num < array_count) {

	    int pid;
	    run_source(child, child_num, (array_count));

	    child_num++;
	    run_dest(child, child_num, (array_count));

	    child_num++;
	
            close(child[pipe_count].fd[0]); close(child[pipe_count].fd[1]); // close both file descriptors on pipe
	    
	    pipe_count++;
	
	    while((pid = wait(&status)) != -1) {  // pick up dead children
		fprintf(stderr, "process %d exits with %d\n",pid, WEXITSTATUS(status));
		//exit(0);
	    dup2(child[child_num-1].fd[1],1);
	    close(child[child_num-1].fd[0]);	
	    }
        }
	}
	
	close(child[child_num+1].fd[0]); close(child[child_num+1].fd[1]); // close both file descriptors on pipe
	close(child[child_num].fd[0]); close(child[child_num].fd[1]);
	array_count = 0;
	child_num = 0;
	pipe_count = 0;
	count = 0;
	

    }    
    return 0;
}

// http://101.lv/learn/C++
