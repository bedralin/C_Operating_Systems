#include <stdlib.h>
#include <stdio.h>

struct tlb_entry {
    char valid;
    int page;
    int frame;
};

int main() {
    struct tlb_entry tlb[16];
    int array[16];
    int i;

    tlb[0].page = 1;
    tlb[1].page = 3;
    printf("tlb is %d\n",tlb[1].page);
    printf("tlb is %d\n",tlb[0].page);
    printf("tlb is %d\n",tlb[2].page);
    printf("tlb is %d\n",tlb[3].page);

    int vaddr = 256;
    array[2] = 3;
    int page = vaddr >> 8; 
    printf("tlb is %d\n",page);
    for (i=0; i<16; i++) {
	printf("Array is: %d\n",array[i]);
	if (array[i] == 0)
	    printf("Null was found in i = %d!!!\n",i);
    }
    int j = 0;
    if (j==1) 
	printf("j is 1\n");
    else
	printf("j is 0\n");

    char valid;
    valid = 'c';
    printf("c is %c\n",valid);

}

    
