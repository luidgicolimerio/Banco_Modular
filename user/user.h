#ifndef USER_H
#define USER_H

#include <stdio.h>

typedef struct userProfile {
    char* name;
    char* birthdate;
    long long CPF;
    char* password;
    char* telNum;
} userProfile;

typedef struct userProfileTree {
    userProfile* profile;
    struct userProfileTree* left;
    struct userProfileTree* right;
} userProfileTree;

int createUserProfile(char* name, char* numeroTel, char* birthdate, long long CPF, char* password);
int getUser(long long CPF, userProfileTree* no);
int alterPassword(long long CPF, const char* newPass);
int alterNumber(long long CPF, const char* numero);
int addProfileToTree(userProfile* profile);
int saveUserProfiles(FILE* arquivo);
int readUserProfiles(FILE* arquivo);

#endif