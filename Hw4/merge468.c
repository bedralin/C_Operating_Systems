// Bronson Edralin
// EE468
// HW4
// 9/24/14

/* 
 * This is an implementation of merge sort assuming the
 * data is an array a[] of length N, and N is a power of 2.
 *
 * The main() program uses a function called merge() which
 * merges subarrays in sorted order.  Rewrite the program
 * so that it uses threads to do the merging of the
 * subarrays.  In particular, each call to merge() creates
 * a thread.  
 *
 * The merging is done in a for-loop.  Each
 * pass through the for-loop causes subarrays of the same size
 * to be merged together.  The mergings within a pass should 
 * be done in parallel using one thread per merge. However,
 * the main program must wait for all the threads to complete
 * before doing another pass of the for-loop.  Thus, mergings
 * of subarrays of size 1 can be done in parallel, then
 * mergings of subarrays of size 2 can be done in parallel and
 * so forth.
 *
 * Use the pthread API
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 32 

typedef struct {
    int index;
    int arrsize;
}Data;

/* Global variables -- be careful */
int a[N];     /* Array to sort */
int temp[N];  /* Temporary storage */

void genvalues(int n); /* Initializes array a[] */
void prnvalues(int n); /* Prints array a[] */
void *merge(void *array_context); /* Merges subarrays */

int main() 
{

int arrsize;    /* Size of subarrays to merge */
int numarr;     /* Number of subarrays */
int newarrsize;  /* New subarray size */
int newnumarr;   /* New number of subarrays */
int i;

int count_thrd = 0;
Data A[N];
pthread_t thread_index[N];


genvalues(N); /* Initialize a[] with random values */
printf("\nInitial values:\n");
prnvalues(N); /* Display the values */

arrsize = 1;
numarr = 2;

while (numarr > 1) {
   arrsize= 2*arrsize; /* merge subarrays to double subarray size */
   numarr = N/arrsize;
   printf("\n\n......................................................\n\n");
   count_thrd = 0;
   for (i=0; i<N; i+=arrsize) {
      A[count_thrd].index = i;
      A[count_thrd].arrsize = arrsize;
      if(pthread_create(&thread_index[count_thrd],NULL,(void *)&merge,(void *)&A[count_thrd])) {
         printf("\nError in creating thread...\n");
	 exit(1);
      }
      else {
         printf("Thread %d created...",count_thrd+1);
	 count_thrd++;
      }  
   }
   printf("\n\n");
   for(i=0;i < count_thrd; i++) {

      if(pthread_join(thread_index[i], NULL)) {
         printf("\nError in joining thread...\n");
         exit(1);
      }
      printf("Thread %d Successful...",i+1);
   }
    

}
printf("\n\n......................................................\n\n");
printf("\nOutput:\n");
prnvalues(N); /* Display the sorted values */
printf("\n");
}

/*
 * Merges subarrays (a[first], ..., a[midpoint-1])
 *    and (a[midpoint],..., a[last-1]) into
 *    subarray temp[i],..., temp[last-1] in increasing order.
 *    Then it copies temp[first],..., temp[last-1] back into
 *    a[i],..., a[last-1].
 */
void *merge(void *array_context)
{
int leftptr;
int rightptr;
int midpoint;
int last;
int k;
int delay;

Data *A = array_context;

/* 
 * Do not delete the next three lines.  They cause the function to
 * delay by a amount that is proportional to the subarray it is merging
 */
if (A->arrsize > 0) delay = A->arrsize;
else delay = 1;
usleep(delay*250000);

midpoint= A->index + (A->arrsize/2);
last = A->index + A->arrsize;
leftptr = A->index;
rightptr = midpoint; 

for(k=A->index; k<last; k++) {
   if (leftptr >= midpoint) temp[k] = a[rightptr++];
   else if (rightptr >= last) temp[k] = a[leftptr++];
   else if (a[leftptr] < a[rightptr]) temp[k] = a[leftptr++];
   else if (a[leftptr] >= a[rightptr]) temp[k] = a[rightptr++];
   else printf("There's a bug \n");
}

for(k=A->index; k<last; k++) a[k] = temp[k];

return 0;
}



/*
 * Initializes array a[] with random values.
 */
void genvalues(int n)
{
int k;
int i;
int current; 

k = 2*n;
current = 0;
for (i=0; i<n; i++) {
   current = (current*73 + 19)%k;
   a[i] = current;
}
}
//Data
/*
 * Prints the values in the array a[]
 */
void prnvalues(int n)
{
int i;
for (i=0; i<n; i++) {
   printf(" %d ", a[i]);
   if ((i+1)%10 == 0) printf("\n");
}
printf("\n");

}


