#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#ifdef ACCESS_CONTROL_OWN
    #define ACCESS_SCOPE 
#else
    #define ACCESS_SCOPE extern
#endif

#include <stdio.h>

// --------------------------------------
// Estrutura do login
typedef struct login {
    long int currentCPF;
    char* currentPassword;
} Login;

// Estrutura da árvore de logins
typedef struct loginTree {
    Login* loginData;
    struct loginTree* left;
    struct loginTree* right;
} LoginTree;

// --------------------------------------
// Variáveis globais
ACCESS_SCOPE LoginTree* loginRoot;
ACCESS_SCOPE Login* currentLogin;

// --------------------------------------
// Funções externas (interface principal)
int login(long int CPF, char* password);
int getLoginCPF(long int* CPF);
int registerUser(char* name, char* numeroTel, char* birthdate, long int CPF, char* password);
int logout(long int CPF);
int changePassword(long int CPF, char* currentPass, char* newPass);
int changeNumber(long int CPF, char* password, char* newNumber);
int saveLogins(FILE* file, LoginTree* root);
int readLogins(FILE* file, LoginTree** root);

#endif
