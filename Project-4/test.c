#include <stdio.h>
#include <string.h>

int main() {
    char myStr[10];
    
    gets(myStr);
    printf("length of string %s is %d.\n", myStr, strlen(myStr));
    
    return 0;
}


