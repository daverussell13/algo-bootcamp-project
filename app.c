#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LIMIT 101

// STRUCT USER
typedef struct user {
    char username[MAX_LIMIT];
    char password[MAX_LIMIT];
    long long balance;
} User;

// GLOBAL VAR
char* U_PATH = "data/user.bin";
User* Users = NULL;

// PROTO
void Menu();
void SignUp();
void Login();

/* ======= UTILITY FUNCTION ======= */
void clearScreen(){printf("\e[1;1H\e[2J");} // regex

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
/* =============================== */

/* ========= LOGIN =========== */
User* isValid(char* user){
    User users;
    FILE* fp = fopen(U_PATH,"r");
    while(fread(&users,sizeof(User),1,fp)){
        if(!strcmp(users.username,user)) {
            User* ptr = (User*)malloc(sizeof(User));
            strcpy(ptr->username,users.username);
            strcpy(ptr->password,users.password);
            ptr->balance = users.balance;
            return ptr;
        }
    }
    return NULL;
}

void Login(){
    clearScreen();
    char user[MAX_LIMIT], pass[MAX_LIMIT];
    puts(" == Login == ");
    printf("Username : ");
    scanf("%s",user); clearBuff();
    printf("Password : ");
    scanf("%s",pass); clearBuff();
    Users = isValid(user);
    char* hashPass = crypt(pass,"00");
    if(Users){
        if(!strcmp(Users->password,hashPass)){
            puts("Login successfull..");
            puts("Press enter to continue...");
            clearBuff();
        }
        else{
            puts("Username / password incorrect\n");
            puts("Press enter to continue...");
            clearBuff(); Login();
        }
    }
}
/* =========================== */

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

/* ===== SIGN UP FUNCTION ======= */
int checkUser(char* userN,int userNLen){
    if(userNLen > 18) return 3;
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
            if(strcmp(userN,users.username) == 0) return 0;
        }
    }
    return 1;
}

void writeNewUser(char* user,char* pass){
    User newUser;
    strcpy(newUser.username,user);
    strcpy(newUser.password,crypt(pass,"00"));
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
        else if(userValid == 0) {
            puts("Username already taken!!!");
            puts("Please pick another username");
            puts("Press enter to continue...");
            clearBuff(); SignUp();
        }
        else {
            puts("Username must not exceed 18 characters");
            puts("Press enter to continue...");
            clearBuff(); SignUp();
        }
    }
    else if(strlen(pass) > 18){
        puts("Password must not exceed 18 characters");
        puts("Press enter to continue...");
        clearBuff(); SignUp();
    }
    else if(strcmp(pass,passConf)){
        puts("Password didnt match!!");
        puts("Press enter to continue...");
        clearBuff(); SignUp();
    }
    else writeNewUser(user,pass);
}
/* ======================== */

void menuOption(short unsigned choice){
    switch(choice){
        case 1:
            Login();
            break;
        case 2:
            SignUp();
            break;
        case 3:
            readData(U_PATH);
            break;
        case 0:
        default:
            clearScreen();
            puts("Invalid Input !!!");
            puts("Press enter to continue..");
            clearBuff(); Menu();
            break;
    }
}

void atmMenu(){
    puts("Welcome to pubji mobile");
    clearBuff();
}

int isLogin(){
    if(Users) return 1;
    return 0;
}

void Menu(){
    short unsigned menuChoice;
    while(1){
        clearScreen();
        puts("ATM MACHINE");
        puts("1. login");
        puts("2. SignUp");
        puts("0. Exit");
        puts("Enter the number : ");
        scanf("%hu",&menuChoice);
        if(!menuChoice) break;
        clearBuff();
        menuOption(menuChoice);
        if(isLogin()) atmMenu();
    }
}

int main() {
    Menu();
    return 0;
}