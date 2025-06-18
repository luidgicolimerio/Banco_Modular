#ifndef USER_INTERNAL_H
#define USER_INTERNAL_H

#include "user.h"

// Estruturas completas (visíveis apenas para o próprio módulo e testes white-box)
struct userProfile {
    char* name;
    char* birthdate;
    long long CPF;
    char* password;
    char* telNum;
};
typedef struct userProfile userProfile;

struct userProfileTree {
    userProfile*            profile;
    struct userProfileTree* left;
    struct userProfileTree* right;
};
typedef struct userProfileTree userProfileTree;

#endif 