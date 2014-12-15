#include <stdio.h>

int main ()
{
   char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};

   printf("Greeting message: %s\n", greeting );

   //char wow[20];

   char wow[] = "wow man!!!";
   //wow = wow +1;
   printf("\nWow[] = %s\n",wow);
   return 0;
}
