#include <stdio.h>
#include <unistd.h>
#include <string.h>

// MACROS & GLOBAL VAR
#define MAX_LIMIT 101
char* U_PATH = "data/user.txt";

// PROTO
void Menu();
void SignUp();
void Login();

typedef struct user {
    char username[MAX_LIMIT];
    char password[MAX_LIMIT];
    long long balance;
} User;

void clearScreen(){printf("\e[1;1H\e[2J");}

void clearBuff(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF){}
}

int fExist(char* fname){
    FILE *file;
    if (file = fopen(fname,"r")) {
        fclose(file);
        return 1;
    }
    else return 0;
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

// debug
void readData(char* fname){
    User users;
    FILE* fp;
    if(fp = fopen(fname,"r")){
        while(fread(&users,sizeof(User),1,fp)){
            printf("%s %s\n%lld\n",users.username,users.password,users.balance);
        }
    }
    else printf("File doesnt exist..");
}

int checkUser(char* userN,int userNLen){
    for(int i = 0; i < userNLen; i++){
        if((userN[i] >= 'A' && userN[i] <= 'Z') ||
           (userN[i] >= 'a' && userN[i] <= 'z') ||
           (userN[i] >= '0' && userN[i] <= '9')) continue;
        else return 2;
    }
    User users;
    FILE* fp;
    if(fp = fopen(U_PATH,"r")){
        while(fread(&users,sizeof(User),1,fp)){
            if(strcmp(userN,users.username) == 0){
                return 0;
            }
        }
    }
    return 1;
}

void writeNewUser(char* user,char* pass){
    User newUser;
    strcpy(newUser.username,user);
    strcpy(newUser.password,pass);
    newUser.balance = 0;
    FILE* fp;
    if(!fExist(U_PATH)){
        fp = fopen(U_PATH,"w");
        fwrite(&newUser,sizeof(User),1,fp);
    }
    else {
        fp = fopen(U_PATH,"a");
        fwrite(&newUser,sizeof(User),1,fp);
    }
    fclose(fp);
}

void SignUp(){
    clearScreen();
    char user[MAX_LIMIT], pass[MAX_LIMIT], passConf[MAX_LIMIT];
    puts(" == Sign Up == ");
    printf("Enter a Username : ");
    scanf("%s",user); clearBuff();
    char *ptr = getpass("Enter a Password : ");
    strcpy(pass,ptr);
    ptr = getpass("Re-enter a password : ");
    strcpy(passConf,ptr);
    int userValid = checkUser(user,strlen(user));
    if(userValid != 1){
        if(userValid == 2){
            puts("Username contains invalid character!!!");
            puts("Press enter to continue...");
            clearBuff(); SignUp();
        }
        else {
            puts("Username already taken!!!");
            puts("Please use another name");
            puts("Press enter to continue...");
            clearBuff(); SignUp();
        }
    }
    else if(!checkPass(pass,passConf)){
        puts("Password didnt match!!");
        puts("Press enter to continue...");
        clearBuff(); SignUp();
    }
    else writeNewUser(user,pass);
}

void menuOption(short unsigned choice){
    switch(choice){
        case 1:
            Login();
            break;
        case 2:
            SignUp();
            break;
        case 3:
            readData("user.txt");
            break;
        default:
            clearScreen();
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