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
char* U_PATH = "user.bin";
User* Users = NULL;

// PROTO
void Menu();
void SignUp();
void Login();
void tarikTunai();
void setorTunai();

/* ======= UTILITY FUNCTION ======= */
void clearScreen(){printf("\e[1;1H\e[2J");}

void clearBuff() { int c; while ((c = getchar()) != '\n' && c != EOF){} }

int fExist(char* fname){
    FILE *file;
    if (file = fopen(fname,"rb")) {
        fclose(file);
        return 1;
    }
    return 0;
}

void delay(){ for (int i=0; i<1.5e8; i++){} }

int isLogin(){ if(Users) return 1; return 0; }

void Logout(){ Users = NULL; }

void invalidInput(){
    puts("Input tidak sesuai !!!");
    puts("Tekan enter untuk melanjutkan..");
    clearBuff();
}
/* =============================== */

/* ========= LOGIN =========== */
User* isValid(char* user){
    User users;
    FILE* fp = fopen(U_PATH,"rb");
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
    int flag = 0;
    do {
        clearScreen();
        char user[MAX_LIMIT], pass[MAX_LIMIT];
        puts(" == Login == ");
        printf("Username : ");
        scanf("%s",user); clearBuff();
        char* ptr = getpass("Password : ");
        strcpy(pass,ptr);
        if(fExist(U_PATH)){
            Users = isValid(user);
            char* hashPass = crypt(pass,"00");
            if(Users){
                if(!strcmp(Users->password,hashPass)){
                    puts("Berhasil login..");
                    puts("Tekan enter untuk melanjutkan...");
                    clearBuff(); flag = 0;
                }
                else {
                    puts("Username/password salah atau tidak terdaftar");
                    puts("Tekan enter untuk melanjutkan...");
                    clearBuff(); flag = 1;
                }
            }
            else {
                puts("Username/password salah atau tidak terdaftar");
                puts("Tekan enter untuk melanjutkan...");
                clearBuff(); flag = 1;
            }
        }
        else {
            puts("Silahkan melakukan SignUp terlebih dahulu...");
            puts("Tekan enter untuk melanjutkan...");
            clearBuff(); flag = 0;
        }
    } while(flag);
}
/* =========================== */

// debug
void readData(char* fname){
    User users;
    FILE* fp;
    if(fp = fopen(fname,"rb")){
        while(fread(&users,sizeof(User),1,fp)){
            printf("%s %s\n%lld\n",users.username,users.password,users.balance);
        }
    }
    else printf("File doesnt exist...");
    clearBuff();
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
    if((fp = fopen(U_PATH,"rb"))){
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
        fp = fopen(U_PATH,"wb");
        fwrite(&newUser,sizeof(User),1,fp);
    }
    else {
        fp = fopen(U_PATH,"ab");
        fwrite(&newUser,sizeof(User),1,fp);
    }
    fclose(fp);
}

void SignUp(){
    int flag = 0;
    do {
        clearScreen();
        char user[MAX_LIMIT], pass[MAX_LIMIT], passConf[MAX_LIMIT];
        puts(" == Sign Up == ");
        printf("Masukan Username : ");
        scanf("%s",user); clearBuff();
        char *ptr = getpass("Masukan Password : ");
        strcpy(pass,ptr);
        ptr = getpass("Konfirmasi password : ");
        strcpy(passConf,ptr);
        int userValid = checkUser(user,strlen(user));
        if(userValid != 1){
            if(userValid == 2){
                puts("Username mengandung character terlarang!!!");
                puts("Tekan enter untuk melanjutkan...");
                clearBuff(); flag = 1;
            }
            else if(userValid == 0) {
                puts("Username telah digunakan!!!");
                puts("Mohon gunakan username lain");
                puts("Tekan enter untuk melanjutkan...");
                clearBuff(); flag = 1;
            }
            else {
                puts("Username tidak melebihi 18 character!!!");
                puts("Tekan enter untuk melanjutkan...");
                clearBuff(); flag = 1;
            }
        }
        else if(strlen(pass) > 18){
            puts("Password tidak melebihi 18 character!!!");
            puts("Tekan enter untuk melanjutkan...");
            clearBuff(); flag = 1;
        }
        else if(strcmp(pass,passConf)){
            puts("Konfirmasi password tidak sesuai!!!");
            puts("Tekan enter untuk melanjutkan...");
            clearBuff(); flag = 1;
        }
        else writeNewUser(user,pass), flag = 0;
    } while(flag);
}
/* ======================== */

/* ===== ATM FUNCTION ======= */
void cekSaldo(){
    clearScreen();
    int opt;
    puts("======= Cek Saldo ========");
    printf("Saldo anda : Rp.%lld\n",Users->balance);
    puts("1. Tarik tunai");
    puts("2. Setor tunai");
    puts("0. Kembali ke halaman menu");
    puts("====================");
    printf("Masukan pilihan anda : ");
    scanf("%d",&opt); clearBuff();
    if(opt == 1) tarikTunai();
    else if(opt == 2) setorTunai();
    else if(opt == 0) return;
    else invalidInput();
}

void tarikTunai(){
    puts("tarik tunai");
    clearBuff();
}

void setorTunai(){
    puts("setor tunai");
    clearBuff();
}

void transfer(){
    puts("transfer");
    clearBuff();
}
/* ========================== */

/* ===== MENU FUNCTION ===== */
void atmMenuOption(short unsigned opt){
    switch(opt){
        case 0:
            Logout();
            break;
        case 1:
            cekSaldo();
            break;
        case 2:
            tarikTunai();
            break;
        case 3:
            setorTunai();
            break;
        case 4:
            transfer();
            break;
        default:
            invalidInput();
            break;
    }
}

void atmMenu(){
    short unsigned opt;
    while(Users){
        clearScreen();
        printf("Selamat datang %s\n",Users->username);
        puts("====== MENU ======");
        puts("1. Cek saldo");
        puts("2. Tarik tunai");
        puts("3. Setor tunai");
        puts("4. Transfer");
        puts("0. Logout");
        puts("==================");
        printf("Masukan pilihan anda : ");
        scanf("%hu",&opt);
        clearBuff();
        atmMenuOption(opt);
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
        case 3:
            readData(U_PATH);
            break;
        default:
            invalidInput();
            break;
    }
}

void printASCII() {
    puts("           _.-------._");delay();
    puts("        _-'_.------._ `-_");delay();
    puts("      _- _-          `-_/");delay();
    puts("     -  -");delay();
    puts(" ___/  /______________");delay();
    puts("/___  .______________/");delay();
    puts(" ___| |_____________");delay();
    puts("/___  .____________/");delay();
    puts("    \\  \\");delay();
    puts("     -_ -_             /|");delay();
    puts("       -_ -._        _- |");delay();
    puts("         -._ `------'_./");delay();
    puts("            `-------'");delay();
}

void Menu(){
    short unsigned opt;
    while(1){
        clearScreen();
        // printASCII();
        puts("ATM MACHINE");
        puts("1. Login");
        puts("2. SignUp");
        puts("0. Keluar");
        printf("Masukan pilihan anda : ");
        scanf("%hu",&opt);
        if(!opt) break;
        clearBuff();
        menuOption(opt);
        if(isLogin()) atmMenu();
    }
}
/* ==================== */

int main() {
    Menu();
    return 0;
}