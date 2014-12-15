/*
 *  EE 468 Final Exam Problem 
 *
 *  This program simulates a dynamic page system with 3 pages for three
 *  replacement algorithms:  FIFO, least recently used (LRU), and
 *  optimal.  The list of address references come from a data
 *  file named "scheddata".  The first line in the data file
 *  is the size of the list, e.g., if the line has the integer 20
 *  then the list has 20 integer entries.  The subsequent lines
 *  have the references.  The program will then output the
 *  entries to verify that the data was loaded properly.  Then
 *  it will determine the hits and misses for each of the algorithms.
 *  The current program has optimal implemented.  But FIFO and LRU
 *  are not implemented properly.  There are functions fifo() and lru()
 *  for FIFO and LRU but they are presently just stubs.  You must
 *  implement them.
 *
 *  Note:  the prob6data file that is in this directory is the same
 *  as the one in textbook problem 9.21.
 */


#include <stdio.h>
#include <stdlib.h>

#define MAXREF    40  /* Max number of references */
#define NUMPAGES  3   /* Number of pages */

void dispOutcome(int numref, int ref[], int outcome[]);
void fifo(int numref, int ref[], int outcome[]);
void lru(int numref, int ref[], int outcome[]);
void optimal(int numref, int ref[], int outcome[]);

int main()
{

FILE * pFile;

int i;

int numref;       /* Number of references */
int ref[MAXREF];  /* References */
int refvalue;

/* 
 * The following array is used to store the outcome of a
 * replacement algorithm.  Outcome[k] is the result of the
 * kth reference.  If Outcome[k] = 0 then it's a miss (page
 * fault); and if Outcome[k] = 1 then it's a hit.
 */
int outcome[MAXREF]; /* Outcome of an algorithm */

/* 
 * Load references
 */
pFile = fopen("scheddata","r");
if (pFile == NULL) {
   printf("Didn't open prob7data file\n");
   return 0;
}
fscanf(pFile, "%d", &numref);
printf("numref = %d\n",numref);
if (numref > MAXREF) {
   printf("Number of references %d is too large\n",numref);
}
for (i=0; i<numref; i++) { /* Read all the reference values */
   fscanf(pFile," %d ", &refvalue);
   ref[i] = refvalue;
}

printf("\n");  /* Print out references to verify */
printf("References that were loaded\n");
printf("Number of references = %d\n",numref);
for (i=0; i<numref; i++) {
   printf("Reference[%d] = %d\n",i,ref[i]);
}

printf("\nFIFO outcomes\n");
fifo(numref, ref, outcome);
dispOutcome(numref, ref, outcome);

printf("\nLRU outcomes\n");
lru(numref, ref, outcome);
dispOutcome(numref, ref, outcome);

printf("\nOptimal outcomes\n");
optimal(numref, ref, outcome);
dispOutcome(numref, ref, outcome);

}

void dispOutcome(int numref, int ref[], int outcome[])
{
int i;

for (i=0; i<numref; i++) { 
   printf("Ref[%d] = %d outcome = ",i,ref[i]);
   if (outcome[i] == 0) printf("Miss\n");
   else if (outcome[i] == 1) printf("Hit\n");
   else printf("invalid outcome\n");
}
}

/*
 * FIFO replacement algorithm.  It doesn't work and 
 * you have to fix it.
 */
void fifo(int numref, int ref[], int outcome[])
{

int pagecount = 0; /* Number of stored pages */
int page[NUMPAGES]; /* References in the stored pages */

int i, j, k, n;
int nextref[NUMPAGES]; /* When next replacement occurs */
int swap;

for (i=0; i<numref; i++) {
 
   /* Check if reference i is in the stored pages */
   outcome[i] = 0;
   for (k=0; k<pagecount; k++) {
      if (ref[i] == page[k]) {
         outcome[i] = 1; // A hit occured
         break;
      }
   }

   /* If it's a miss then store the new page */
   if (outcome[i] == 0) {

      /* If there is empty space then store it there */
      if (pagecount < NUMPAGES) {
         page[pagecount] = ref[i];
         pagecount++;
      }
      else { /* Otherwise, you must find the page to replace */
	 
         for (j=0; j<pagecount; j++) {
	   page[j] = page[j+1];
         }

         /* Replace with ref[i] */
         page[2] = ref[i];
      }
   }
   //printf("Ref[i] is %d, Outcome[i] is: %d\n",ref[i],outcome[i]);
   //printf("page[0]=%d, page[1]=%d, page[2]=%d\n\n",page[0],page[1],page[2]);

}


}

/* 
 * Least recently used replacement algorithm.  It doesn't work
 * and you have to fix it.  
 */
void lru(int numref, int ref[], int outcome[])
{

int pagecount = 0; /* Number of stored pages */
int page[NUMPAGES]; /* References in the stored pages */

int i, j, k, n;
int nextref[NUMPAGES]; /* When next replacement occurs */
int swap;

for (i=0; i<numref; i++) {

   /* Check if reference i is in the stored pages */
   outcome[i] = 0;
   for (k=0; k<pagecount; k++) {
      if (ref[i] == page[k]) {
         outcome[i] = 1; // A hit occured
         swap = page[k];
	 break;
      }
   }
    if (outcome[i] == 1) {
         for (j=0; j<pagecount; j++) {
           page[j] = page[j+1];
         }
         /* Replace with ref[i] */
         page[2] = swap;
   }
   /* If it's a miss then store the new page */
   if (outcome[i] == 0) {

      /* If there is empty space then store it there */
      if (pagecount < NUMPAGES) {
         page[pagecount] = ref[i];
         pagecount++;
      }
      else { /* Otherwise, you must find the page to replace */
         
         for (j=0; j<pagecount; j++) {
           page[j] = page[j+1];
         }

         /* Replace with ref[i] */
         page[2] = ref[i];
      }
   }
   //printf("Ref[i] is %d, Outcome[i] is: %d\n",ref[i],outcome[i]);
   //printf("page[0]=%d, page[1]=%d, page[2]=%d\n\n",page[0],page[1],page[2]);

}


}

/*
 * Optimal replacement algorithm
 *
 */
void optimal(int numref, int ref[], int outcome[])
{

int pagecount = 0; /* Number of stored pages */
int page[NUMPAGES]; /* References in the stored pages */

int i, j, k, n;
int nextref[NUMPAGES]; /* When next replacement occurs */
int bestpageindex;
int bestpagevalue;

for (i=0; i<numref; i++) {

   /* Check if reference i is in the stored pages */
   outcome[i] = 0;
   for (k=0; k<pagecount; k++) {
      if (ref[i] == page[k]) {
         outcome[i] = 1;
         break;
      }
   }

   /* If it's a miss then store the new page */
   if (outcome[i] == 0) {

      /* If there is empty space then store it there */
      if (pagecount < NUMPAGES) {
         page[pagecount] = ref[i];
         pagecount++;
      }
      else { /* Otherwise, you must find the page to replace */

         /* Find the next replacements for each page j*/
         for (j=0; j<pagecount; j++) { 
            for (n=i+1; n<numref; n++) {
               if (page[j] == ref[n]) break;
            }
            nextref[j] = n;
         }

         /* Choose the best page to replace */
         bestpageindex = 0;
         bestpagevalue = 0; 
         for (j=0; j<pagecount; j++) { 
            if (nextref[j] > bestpagevalue) {
               bestpageindex = j; 
               bestpagevalue = nextref[j];
            } 
         }

         /* Replace with ref[i] */
         page[bestpageindex] = ref[i];
      }   
   }
}
}

