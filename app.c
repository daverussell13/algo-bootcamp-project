#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LIMIT 101
#define BLNC_LIMIT (long long unsigned)(18*(1e18))

// STRUCT USER
typedef struct user {
    char username[MAX_LIMIT];
    char password[MAX_LIMIT];
    unsigned long long balance;
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

void freezePrompt(){
    puts("Tekan enter untuk melanjutkan...");
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
                    freezePrompt(); flag = 0;
                }
                else {
                    puts("Username/password salah atau tidak terdaftar");
                    freezePrompt(); flag = 1;
                }
            }
            else {
                puts("Username/password salah atau tidak terdaftar");
                freezePrompt(); flag = 1;
            }
        }
        else {
            puts("Silahkan melakukan SignUp terlebih dahulu...");
            freezePrompt(); flag = 0;
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
            printf("%s %s\n%llu\n",users.username,users.password,users.balance);
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
    newUser.balance = 17999999999999999999;
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
                freezePrompt(); flag = 1;
            }
            else if(userValid == 0) {
                puts("Username telah digunakan!!!");
                puts("Mohon gunakan username lain");
                freezePrompt(); flag = 1;
            }
            else {
                puts("Username tidak melebihi 18 character!!!");
                freezePrompt(); flag = 1;
            }
        }
        else if(strlen(pass) > 18){
            puts("Password tidak melebihi 18 character!!!");
            freezePrompt(); flag = 1;
        }
        else if(strcmp(pass,passConf)){
            puts("Konfirmasi password tidak sesuai!!!");
            freezePrompt(); flag = 1;
        }
        else writeNewUser(user,pass), flag = 0;
    } while(flag);
}
/* ======================== */

/* ===== Banking utils ===== */
long long unsigned getBalance(const char* uname){
    User u;
    FILE* fp = fopen(U_PATH,"rb");
    while(fread(&u,sizeof(User),1,fp)){
        if(!strcmp(u.username,Users->username)){
            return u.balance;
        }
    }
    fclose(fp);
}

int enoughBalance(long long money,const char* uName){
    long long unsigned balance = getBalance(uName);
    if(balance >= money) return 1;
    return 0;
}

long long getMoney(int opt){
    switch(opt){
        case 0:
            return 0;
        case 1:
            return 50000;
        case 2:
            return 100000;
        case 3:
            return 500000;
        case 4:
            return 1000000;
        case 5:
            return 5000000;
        case 6:
            return 10000000;
        default:
            return -1;
    }
}

void cutBalance(long long unsigned money,const char* uName){
    User u;
    FILE* fp = fopen(U_PATH,"rb+");
    while(fread(&u,sizeof(User),1,fp)){
        if(!strcmp(u.username,Users->username)){
            u.balance -= money;
            fseek(fp,-sizeof(u),1);
            fwrite(&u,sizeof(u),1,fp);
        }
    }
    fclose(fp);
}

void addBalance(long long unsigned money,const char* uName){
    User u;
    FILE* fp = fopen(U_PATH,"rb+");
    while(fread(&u,sizeof(User),1,fp)){
        if(!strcmp(u.username,uName)){
            u.balance += money;
            fseek(fp,-sizeof(u),1);
            fwrite(&u,sizeof(u),1,fp);
        }
    }
    fclose(fp);
}

int aboveLimit(long long unsigned money,long long unsigned balance){
    if(balance+money > BLNC_LIMIT) return 1;
    return 0;
}

void success(){
    puts("Transaksi berhasil dilakukan..");
    printf("Saldo anda saat ini : Rp. %llu\n",getBalance(Users->username));
}
/* ============================= */

/* ===== ATM FUNCTION ======= */
void cekSaldo(){
    clearScreen();
    int opt;
    long long unsigned saldo = getBalance(Users->username);
    puts("======= Cek Saldo ========");
    printf("Saldo anda : Rp. %llu\n",saldo);
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
    int flag = 0;
    do {
        int opt; clearScreen();
        puts("===== Tarik tunai =====");
        puts("1. Rp. 50.000,00");
        puts("2. Rp. 100.000,00");
        puts("3. Rp. 500.000,00");
        puts("4. Rp. 1.000.000,00");
        puts("5. Rp. 5.000.000,00");
        puts("6. Rp. 10.000.000,00");
        puts("0. Kembali ke halaman menu");
        puts("=======================");
        printf("Masukan pilihan anda : ");
        scanf("%d",&opt); clearBuff();
        long long money = getMoney(opt);
        if(money < 0) invalidInput(), flag = 1;
        else if(opt == 0) return;
        else {
            if(!enoughBalance(money,Users->username)){
                puts("Maaf saldo anda tidak cukup..");
                flag = 1;
                freezePrompt();
            }
            else {
                cutBalance(money,Users->username);
                flag = 0;
                success();
                freezePrompt();
            }
        }
    } while(flag);
}

void setorTunai(){
    int flag = 0;
    do {
        clearScreen();
        long long unsigned nominalSetor;
        puts("======= Setor Tunai =======");
        puts("(Nominal maksimal sebesar Rp. 1.000.000.000 per transaksi)");
        printf("Masukan nominal uang : ");
        scanf("%llu",&nominalSetor); clearBuff();
        if(aboveLimit(nominalSetor,Users->balance)){
            puts("Saldo anda sudah melebihi batas yang telah ditentukan..");
            printf("Saldo maksimal yang bisa anda setor sebesar : Rp %lld\n",BLNC_LIMIT-(Users->balance));
            freezePrompt(); flag = 1;
        }
        else {
            if(nominalSetor > 1e9){
                puts("Nominal maksimal per transaksi sebesar Rp. 1.000.000.000!!!");
                freezePrompt(); flag = 1;
            }
            else {
                addBalance(nominalSetor,Users->username);
                flag = 0;
                success();
                freezePrompt();
            }
        }
    } while(flag);
}

void transfer(){
    // int flag = 0;
    // do {
    //     clearScreen();
    //     User *receiver;
    //     char rName[MAX_LIMIT];
    //     long long unsigned nominal;
    //     puts("========= TRANSFER =========");
    //     printf("Masukan username yang dituju : ");
    //     scanf("%s",rName);
    //     printf("Masukan nominal uang yang akan di transfer : ");
    //     scanf("%lld",&nominal); clearBuff();
    //     if(nominal > 1e10){
    //         puts("Saldo anda tidak mencukupi..");
    //         freezePrompt();
    //     }
    //     else if(nominal > Users->balance){

    //     }
    //     else {
    //         receiver = isValid(rName);
    //         if(receiver){

    //         }
    //     }

    // } while(flag);
    // clearBuff();
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
        puts("=========== MENU ==========");
        puts("||    1. Cek saldo       ||");
        puts("||                       ||");
        puts("||    2. Tarik tunai     ||");
        puts("||                       ||");
        puts("||    3. Setor tunai     ||");
        puts("||                       ||");
        puts("||    4. Transfer        ||");
        puts("||                       ||");
        puts("||    0. Logout          ||");
        puts("===========================");
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
        // clearScreen();
        // printASCII();
        puts("==========================");
        puts("||      ATM MACHINE     ||");
        puts("==========================");
        puts("||\t1. Login\t||");
        puts("||\t\t\t||");
        puts("||\t2. SignUp\t||");
        puts("||\t\t\t||");
        puts("||\t0. Keluar\t||");
        puts("==========================");
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