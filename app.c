#include <stdio.h>
#include <string.h>

// macros
#define MAX_LIMIT 255
#define ENTER 13
#define TAB 9
#define BKSP 8

// proto
void Menu();

void clearScreen(){printf("\e[1;1H\e[2J");}

void clearBuff(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF){}
}

short unsigned checkPass(char pass[],char passConf[]){
    if(!strcmp(pass,passConf)) return 1;
    else return 0;
}

void Login(){
    clearScreen();
    char user[MAX_LIMIT], pass[MAX_LIMIT];
    printf("Username : ");
    scanf("%s",user); clearBuff();
    printf("Password : ");
    scanf("%s",pass); clearBuff();
}

void SignUp(){
    clearScreen();
    char user[MAX_LIMIT], pass[MAX_LIMIT], passConf[MAX_LIMIT];
    printf("Username : ");
    scanf("%s",user); clearBuff();
    printf("Password : ");
    scanf("%s",pass); clearBuff();
    printf("Re-enter Password : ");
    scanf("%s",passConf); clearBuff();
    // checkUser();
    if(!checkPass(pass,passConf)){
        puts("Password not match !!!");
        puts("Press enter to continue...");
        clearBuff(); SignUp();
    }
}

void menuOption(short unsigned choice){
    switch(choice){
        case 1:
            Login();
            break;
        case 2:
            SignUp();
            break;
        default:
            puts("Invalid Input !!!");
            puts("Press enter to continue..");
            clearBuff(); Menu();
            break;
    }
}

void Menu(){
    short unsigned menuChoice;
    puts("ATM MACHINE");
    puts("1. login");
    puts("2. SignUp");
    puts("Enter the number : ");
    scanf("%hu",&menuChoice);
    clearBuff();
    menuOption(menuChoice);
}

int main() {
    Menu();
    return 0;
}