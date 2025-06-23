#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h"





int novac;
int uplatta() {
    int *novauplata = malloc(sizeof(int));  // alokacija prostora za int
    
    scanf("%d", novauplata);                // spremi korisnikov unos

    if (*novauplata == 0) {      
        free(novauplata);
        printf("%d je premali iznos!\nKoliko zelis uplatiti?\n", *novauplata);
        return *novauplata;             //Ranjivost: dereferenciranje oslobođene memorije
    }
    if(*novauplata > novac){
        printf("%d je prevelik iznos!\nKoliko zelis uplatiti?\n", *novauplata);
    }

    return *novauplata;
    free(novauplata);       //Ranjivost: memory leak
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int callMoney(void *data, int argc, char **argv, char **azColName){
    novac = atoi(argv[0]);
    return 0;
}


sqlite3 *db;
int callSvi(void *data, int argc, char **argv, char **azColName) {
    printf("[%s] [%s] [%d]", argv[0], argv[1], atoi(argv[2]));
    return 0;
}

int regCallback(void *data, int argc, char **argv, char **azColName) {
    int *userFound = (int *)data;
    *userFound = 1;
    return 0;
}

int loginCallback(void *data, int argc, char **argv, char **azColName) {
    int *userFound = (int *)data;
    *userFound = 1;
    return 0;
}

char* registriraj() {
    int rc;
    char *errmsg = NULL;
    char* username = malloc(20);
    char pass[20];

    printf("Username: ");
    fgets(username, 256, stdin);    //Previse upisa u username
    remove_newline(username);

    char sql_select[128];
    int userExists = 0;
    sprintf(sql_select, "SELECT name FROM users WHERE name = '%s';", username);
    rc = sqlite3_exec(db, sql_select, regCallback, &userExists, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Greška kod SELECT: %s\n", errmsg);
        sqlite3_free(errmsg);
        free(username);
        return NULL;
    }

    if (userExists) {
        printf("Ime je zauzeto.\n");
        free(username);
        return NULL;
    }

    printf("Password: ");
    scanf("%s", pass);

    srand(time(NULL));
    int broj = rand() % (9901) + 100;   //U svrhu jednostavnosti, bira se nasumičan broj kao količina novca korisnika

    char sql_insert[128];
    sprintf(sql_insert, "INSERT INTO users (name, pass, money) VALUES ('%s', '%s', %d);", username, pass, broj);
    rc = sqlite3_exec(db, sql_insert, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Greška u dodavanju usera: %s\n", errmsg);
        sqlite3_free(errmsg);
        free(username);
        return NULL;
    }

    return username;
}

char* login() {
    int rc;
    char *errmsg = NULL;
    char* username = malloc(20);
    char pass[20];

    printf("Username: ");
    fgets(username, 256, stdin);    //Previse upisa u username
    remove_newline(username);
    printf("Password: ");
    scanf("%s", pass);

    char sql_select[256];
    sprintf(sql_select, "SELECT * FROM users WHERE name = '%s' AND pass = '%s';", username, pass);
    
    int userFound = 0;
    rc = sqlite3_exec(db, sql_select, loginCallback, &userFound, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Greška kod SELECT: %s\n", errmsg);
        sqlite3_free(errmsg);
        free(username);
        return NULL;
    }

    if (!userFound) {
        printf("Krivo ime ili lozinka.\n");
        free(username);
        return NULL;
    }

    return username;
}

int main() {
    char *errMsg = NULL;
    int rc;

    rc = sqlite3_open("test.db", &db);
    if (rc) {
        fprintf(stderr, "Ne mogu otvoriti bazu: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char *sql_create_table = "CREATE TABLE IF NOT EXISTS users (name TEXT PRIMARY KEY, pass TEXT, money INTEGER);";
    rc = sqlite3_exec(db, sql_create_table, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Greška prilikom stvaranja tablice: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    int akcija1;
    char *username = NULL;

    while (1) {
        printf("Register(0) ili login(1)?: ");
        scanf("%d", &akcija1);
        int ch;                         //Ciscenje stdina, da fgets ne procita \n
        while ((ch = getchar()) != '\n' && ch != EOF);

        if (akcija1) {
            username = login();
        } else {
            username = registriraj();
        }

        if (username) break;
    }

    printf("Tvoje ime je ");
    printf(username);   // Za format string
    printf("\n");       // Sada je korisnik ulogiran svojim usernameom

    char sql_select_money[256];
    sprintf(sql_select_money, "SELECT money FROM users WHERE name = '%s'", username);
    rc = sqlite3_exec(db, sql_select_money, callMoney, 0, &errMsg);
    printf("Imas %d novca na racunu.\n\n", novac);

    printf("Koliko novca zelite uplatiti?\n");
    int uplata=-1;
    while(uplata <= 0 || uplata >= novac){
        uplata = uplatta();
    }
    char sql_pay_money[256];
    sprintf(sql_pay_money, "UPDATE users SET money = %d WHERE name = '%s'", novac - uplata, username);
    novac -= uplata;
    rc = sqlite3_exec(db, sql_pay_money, 0, 0, &errMsg);
    printf("Sada imas %d novca na racunu.\n", novac);

    free(username);
    // const char *sql_svi_table = "SELECT * FROM users";
    // rc = sqlite3_exec(db, sql_svi_table, callSvi, 0, &errMsg);

    sqlite3_close(db);
    return 0;
}
