#include <stdio.h>

//#define WOW
int main() {
    #ifdef WOW
    printf("WOW man it worked!!!\n");
    #endif
    printf("Tell me about it....\n");
}
