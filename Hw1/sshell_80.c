/*
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

void runpipe();

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
/*
 * Now 'wbuf' is parsed into the string array 'buf_args'
 *
 * The for-loop uses a string.h function
 *   char *strsep(char **stringp, const char *delim);
 *
 *   Description:  
 *   If *stringp = NULL then it returns NULL and does
 *   nothing else.  Otherwise the function finds the first token in
 *   the string *stringp, where tokens are delimited by symbols
 *   in the string 'delim'.  
 *
 *   In the example below, **stringp is &wbu, and 
 *   the delim = ' ', '\n', and '\t'.  So there are three possible 
 *   delimiters. 
 *
 *   So in the string " Aloha World\n", the spaces and "\n" are
 *   delimiters.  Thus, there are three delimiters.  The tokens
 *   are what's between the delimiters.  So the first token is
 *   "", which is nothing because a space is the first delimiter.
 *   The second token is "Aloha", and the third token is "World".
 *   
 *   The function will scan a character string starting from
 *   *stringp, search for the first delimiter.  It replaces
 *   the delimiter with '\0', and *stringp is updated to point
 *   past the token.  In case no delimiter was found, the
 *   token is taken to be the entire string *stringp, and *stringp
 *   is made NULL.   Strsep returns a pointer to the token. 
 *
 *   Example:  Suppose *stringp -> " Aloha World\n"
 *
 *   The first time strsep is called, the string is "\0Aloha World\n",
 *   and the pointer value returned = 0.  Note the token is nothing.
 *
 *   The second time it is called, the string is "\0Aloha\0World\n",
 *   and the pointer value returned = 1  Note that 'Aloha' is a token.
 *
 *   The third time it is called, the string is '\0Aloha\0World\0', 
 *   and the pointer value returned is 7.  Note that 'World' is a token.
 *
 *   The fourth time it is called, it returns NULL.
 *
 *   The for-loop, goes through buffer starting at the beginning.
 *   wbuf is updated to point to the next token, and cp is
 *   updated to point to the current token, which terminated by '\0'.
 *   Note that pointers to tokens are stored in array buf_args through cp.
 *   The loop stops if there are no more tokens or exceeded the
 *   array buf_args.
 */   
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

    int in[2],out[2];
    int n=0;
    int n1=0;
    int n2=0;
    int array_count;
   
    char * array[10][10];

    int fd[2], nbytes;
    pipe(fd);
    int status;
    int i, j;
 
    while(1){
        printf("ee468>> "); /* Prompt */
        fgets(buffer, BUFFER_SIZE, stdin); /* Read in command line */
              /* Parse the command line into args */
	//printf("buffer is: %s\n",buffer);
	//printf("buffer size is %d",BUFFER_SIZE);
	//printf("stdin is %\n",stdin);
	//printf(ARR_SIZE);
	//printf("nargs is %zu\n",nargs);

        parse_args(buffer, args, ARR_SIZE, &nargs); 
 
        if (nargs==0) continue; /* Nothing entered so prompt again */
        if (!strcmp(args[0], "exit" )) exit(0); 
            //printf("args[0] is %s\n",args[0]);      
            //printf("args[1] is %s\n",args[1]);
            //printf("args[2] is %s\n",args[2]);
	    //printf("argc is %d\n",nargs);

	array_count = 0;	
	i = 0;	
	j = 0;

        for (i = 0; array[i][j] != NULL; i++) {
	    for (j = 0; array[i][j] != NULL; j++) {
		array[i][j] = 0;
	    }
	}
	
	for (n = 0; args[n] != NULL; n++) {
	    if ((strncmp(args[n], "|", 1) != 0) && (args[n] != NULL))  {
		array[array_count][n1] = args[n];
		//printf("array is %s\n",array[array_count][n1]);
		n1++;
		//printf("n is %d\n",n);
	    }
	    if (strncmp(args[n], "|", 1) == 0) {
		array[array_count][n1] = 0;
		//printf("array null is %s\n",array[array_count][n1]);
		n1 = 0;
		array_count++;
	    }
	    //printf("null is %s\n",array[array_count][n1]);
	}
	array[array_count][n1] = 0;
	n1 = 0;

	//n = 0;
	

	switch (pid = fork()) {

	case 0: /* child */
	    runpipe(fd, &array, array_count);
	    //exit(0);

	default: /* parent */
	    while ((pid = wait(&status)) != -1)
	    fprintf(stderr, "Process #%d completed and exited.\n", pid);
	    break;

	case -1:
	    perror("fork");
	    exit(1);
	}
	//exit(0);

	//printf("ret_status is %d\n",*ret_status);
	/*
	// CHILD (2ND ONE AFTER PIPE | )
        if (pid){  # The parent 
	    
	    dup2(fd[1],1);
	    close(fd[0]);
	    if( execvp(array2[0],array2)) {
		puts(strerror(errno));
		printf("error in child has occured.\n");
		exit(127);
		}
	    
	    runpipe(fd,array1, array2);

#ifdef DEBUG
            printf("Waiting for child (%d)\n", pid);
#endif
            pid = wait(ret_status);
#ifdef DEBUG
            printf("Child (%d) finished\n", pid);
#endif
        } 

	// PARENT (1ST ONE BEFORE PIPE | )
        else{  # The child executing the command 
            printf("args[0] is %s\n",args[0]);
            printf("args[1] is %s\n",args[1]);
            printf("args[2] is %s\n",args[2]);
            printf("args[3] is %s\n",args[3]);
            
            printf("array1[0] is %s\n",array1[0]);
            printf("array1[1] is %s\n",array1[1]);
            printf("array1[2] is %s\n",array1[2]);
            printf("array1[3] is %s\n",array1[3]);
    
	    dup2(fd[0], 0);
	    close(fd[1]);
            if( execvp(array1[0], array1)) {
                puts(strerror(errno));
		printf("error in parent has occured.\n");
		//read(
		printf("args[2] is %s\n",args[2]);
                exit(127);
	    }
	    

        }

    */
    }    
    return 0;
}

void runpipe(int pfd[], char *array[10][10], int array_count) {
    int pid;
    int i, j;
    char * array_pass1[10]; 
    char * array_pass2[10];   
    int k = 0;


    //printf("array count is %d\n",array_count);
    if (array_count == 0) {
	//dup2(pfd[1], 1);
	 //close(pfd[0]);   
	if (execvp(array[0][0], *array)) {
	    puts(strerror(errno));
	    printf("Error has occured. You might have put in wrong command.\n");
	    exit(127);
	}
    }

    else {
	for (i = 0; i < array_count; i++) {
		
	switch (pid = fork()) {
	case 0: /* child */
	    dup2(pfd[0], 0);
	    close(pfd[1]);	/* the child does not need this end of the pipe */
	    //printf("Check this array: %s\n",array[i+1][0]);
	    //printf("Check this array[1]: %s\n",array[i+1][1]);
	    //printf("Check this array[2]: %s\n",array[i+1][2]);
	    //printf("Check this array[3]: %s\n",array[i+1][3]);
	    if (execvp(array[i+1][0], &array[i+1][0])) {
		puts(strerror(errno));
		printf("Error has occured. You might have put in wrong command.\n");
		exit(127);
	    }
	default: /* parent */
	    dup2(pfd[1], 1);
	    close(pfd[0]);	/* the parent does not need this end of the pipe */
	    if (execvp(array[i][0], &array[i][0])) {
		puts(strerror(errno));
		printf("Error has occured. You might have put in wrong command.\n");
		exit(127);
	    }

	case -1:
	    perror("fork");
	    exit(1);
	}
    }
    }
}
