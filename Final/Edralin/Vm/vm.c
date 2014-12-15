/* Bronson Edralin
 * EE468 OS
 * Final Exam
 * Problem #6
 * 12/18/14
 */

/*
 * EE 468 Final Exam Problem 
 *
 * This program simulates a virtual memory manager with
 * a TLB with 16 entries.  Total number of memory bytes is 2^16 bytes.
 * Page and frame sizes are 256 bytes.  Number of pages and
 * frames is 256.  It uses the least recently used replacement
 * policy.
 *
 * For the simulation it gets virtual addresses from a file
 * named "address.txt"  
 *
 * However this version doesn't work.  It simulates a
 * memory system with a TLB of size one.  Fix it so
 * that it works according to Final Exam Problem 6.
 */

#include <stdlib.h>
#include <stdio.h>

struct tlb_entry {
   char valid;  /* Valid bit implemented as a char */ 
   int page;    /* Page number */
   int frame;   /* Frame number */
};

struct ptable_entry {
   char valid;  /* Valid bit implemented as a char */ 
   int frame;   /* Frame number */
};

/* The next two data structures is the TLB (tlb) and page table (ptable). 
 * TLB is usually an array but in this (bad) implementation there is only
 * one entry. 
 */

struct tlb_entry tlb[16];              /* Global variable for TLB. */ 
struct ptable_entry ptable[256];   /* Global array for page table */

int main() {
  FILE *fp;
  int vaddr;  /* virtual address */  
  int offset; /* page offset */
  int page;   /* page number */ 
  int frame;  /* frame number of current access */
  int freeframe;  /* pointer to the lowest free frame */
  int tlbhit;     /* indicates a TLB hit */
  int pfault;     /* indicates page fault */
  int physaddr;   /* physical address */
  int i;
  int num_tlbmiss = 0;
  int num_pfault = 0;
  int num_virtual = 0;
  struct tlb_entry swap;

  /* Initialize TLB and page table*/ 
  for (i=0; i<16; i++) tlb[i].valid = 'n';
  for (i=0; i<256; i++) ptable[i].valid = 'n';

  /* Open the file which has virtual addresses */ 
  fp = fopen("address.txt", "r");
  if (fp ==NULL) {
    printf("Cannot open file\n");
    exit(1);
  }

  /* Initialize free frame pointer */
  freeframe = 0;

  /* Read virtual addresses */ 

  while(fscanf(fp, "%d", &vaddr)== 1) { /* Read a virtual address */

    page = vaddr >> 8; /* Page # of virtual address */
    tlbhit = 0; /* indicatess if there is a TLB hit */
    pfault = 0; /* indicates page fault */
    // printf("page is %d\n",page);
    num_virtual++;
 
    tlbhit = 0;
    /* Access TLB */
    for (i=0; i<16; i++) {
      if (tlb[i].valid == 'y' && tlb[i].page == page) { /* TLB hit */ 
        tlbhit = 1; 
        frame = tlb[i].frame;
        
	swap = tlb[i];
	for (i=0; i<15; i++) 
	  tlb[i] = tlb[i+1];
	tlb[15] = swap;
	break;
      }
    }


     if (tlbhit == 0) {  /* TLB miss */
      num_tlbmiss++;
      /* Access the page table */
      if (ptable[page].valid == 'y') { /* No page fault */
        frame = ptable[page].frame;
      }
      else { /* Page fault */
        pfault = 1;
	num_pfault++;
        frame = freeframe;
        ptable[page].frame = freeframe++;
        ptable[page].valid = 'y';
      }
      /* Update TLB */
        if (i<16) {
	  tlb[i].valid = 'y';
          tlb[i].page = page;
          tlb[i].frame = frame;
        }
	else { // TLB full
	  for (i=0; i<15; i++) 
	    tlb[i] = tlb[i+1];
	  tlb[15].valid = 'y';
	  tlb[15].page = page;
	  tlb[15].frame = frame;
	}
	  
    }
    

    physaddr = (frame<<8)+ (0xff & vaddr);  /* Physical address */
    printf("VirtAddr=%d PhysAddr=%d ",vaddr,physaddr);  
    if (tlbhit == 1) printf(" TLB-HIT ");
    else printf(" TLB-MISS ");

    if (pfault == 1) printf(" PAGE-MISS ");
    else printf(" PAGE-HIT ");

    printf("\n");
  }
  printf("\n");
  printf("Number of virtual addresses = %d\n",num_virtual);
  printf("Number of page faults = %d\n",num_pfault);
  printf("Number of TLB misses = %d\n",num_tlbmiss);
  printf("\n"); 

  /*
  for (i=0; i<16; i++) {
    printf("TLB Table: valid=%c, page=%d, frame=%d\n",tlb[i].valid, tlb[i].page, tlb[i].frame);
  }
  for (i=0; i<256; i++) {
    if (ptable[i].frame!=0) printf("Page Table: valid=%c, frame=%d\n",ptable[i].valid, ptable[i].frame);
  } 
  */

  fclose(fp);
  return 0;
}


