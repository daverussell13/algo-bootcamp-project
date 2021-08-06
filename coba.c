#include <stdio.h>
#include <unistd.h>
#define ENTER 13
#define TAB 9
#define BKSP 8

int main(){
    char *str;
    str = getpass("Enter password : ");
    printf("%s",str);
}

