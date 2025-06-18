#define ACCESS_CONTROL_OWN
#include "AcessControl.h"
#include "MockUser.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>

// -----------------------------------------------------------------------------------------------------------------
// Estruturas internas (definicoes completas aqui para manter encapsulamento).
// Apenas os ponteiros sao expostos no header.
typedef struct login {
    long int currentCPF;
    char*    currentPassword;
} Login;

typedef struct loginTree {
    Login*              loginData;
    struct loginTree*   left;
    struct loginTree*   right;
} LoginTree;

// -----------------------------------------------------------------------------------------------------------------
// Variáveis globais
LoginTree* loginRoot = NULL; // raiz da árvore de logins
Login*    currentLogin = NULL; // ponteiro para o login atualmente autenticado

//**********************************************************************************************************
//Funções internas

//Procura login
Login* findLogin(LoginTree* root, long int CPF) {
    if (root == NULL) return NULL;

    long int currentCPF = root->loginData->currentCPF;

    if (CPF == currentCPF) {
        return root->loginData; // CPF encontrado
    } else if (CPF < currentCPF) {
        return findLogin(root->left, CPF); // busca na subárvore esquerda
    } else {
        return findLogin(root->right, CPF); // busca na subárvore direita
    }
}

//Garante que tem de 10 ou 11 digitos e que é tudo numero
int isValidPhone(char* number) {
    int len = strlen(number);
    if (len < 10 || len > 11) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(number[i])) return 0;
    }
    return 1;
}

// Insere login na árvore binária de busca (ordem crescente de CPF)
int addLoginToTree(LoginTree** root, Login* newLogin) {
    if (newLogin == NULL) return -1;

    if (*root == NULL) {
        *root = (LoginTree*) malloc(sizeof(LoginTree));
        if (*root == NULL) return -1;

        (*root)->loginData = newLogin;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return 0;
    }

    long int currentCPF = (*root)->loginData->currentCPF;
    long int newCPF = newLogin->currentCPF;

    if (newCPF < currentCPF) {
        return addLoginToTree(&((*root)->left), newLogin);
    } else if (newCPF > currentCPF) {
        return addLoginToTree(&((*root)->right), newLogin);
    } else {
        return 1; // CPF já existe
    }
}

// Função utilitária local para duplicar strings (substitui strdup para manter portabilidade e evitar avisos)
static char* duplicateString(const char* source) {
    if (source == NULL) return NULL;
    size_t len = strlen(source) + 1;
    char* dest = (char*)malloc(len);
    if (dest) {
        memcpy(dest, source, len);
    }
    return dest;
}

//**********************************************************************************************************
// Funções externas

//Realiza o login na conta salvando na variavel global o login atual
int login(long int CPF, char* password) {
    if (CPF <= 0) return 1; // CPF inválido
    
    Login* foundLogin = findLogin(loginRoot, CPF);
    if (foundLogin == NULL) return 2; // CPF não encontrado
    
    if (strcmp(foundLogin->currentPassword, password) != 0) return 3; // Senha incorreta
    
    currentLogin = foundLogin;
    return 0; // Sucesso
}

//Retorna o CPF da pessoa que ta logada
int getLoginCPF(long int* CPF) {
    if (currentLogin == NULL) {
        return 1; // Usuário não está logado
    }
    
    *CPF = currentLogin->currentCPF;
    return 0; // Sucesso
}

//Cria um novo usuario para uma pessoa associa o login a esse mesmo CPF
int registerUser(char* name, char* numeroTel, char* birthdate, long int CPF, char* password) {
    // Cria o perfil de usuário (função do módulo USER)
    int status = createUserProfile(name, numeroTel, birthdate, CPF, password);
    if (status != 0) {
        return status;  // Retorna erro de criação de perfil
    }
    
    // Aloca memória para novo login
    Login* novoLogin = (Login*) malloc(sizeof(Login));
    if (novoLogin == NULL) {
        return -1;  // Erro de alocação de memória
    }
    
    // Preenche os dados do login
    novoLogin->currentCPF = CPF;
    novoLogin->currentPassword = duplicateString(password);
    
    // Adiciona à árvore de logins
    addLoginToTree(&loginRoot, novoLogin);
    
    return 0; // Registro realizado com sucesso
}

//Deslogar tirar da variavel global 
int logout(long int CPF) {
    // Se não há usuário logado OU CPF não corresponde, retorna erro genérico 1
    if (currentLogin == NULL || currentLogin->currentCPF != CPF) {
        return 1;
    }

    // Caso contrário, efetua logout
    currentLogin = NULL;
    return 0; // Sucesso
}

//Muda a senha tanto no login quanto no usuario relacionado a ele
int changePassword(long int CPF, char* currentPass, char* newPass) {
    // 1: usuário não encontrado/logado ou CPF não confere
    if (currentLogin == NULL || currentLogin->currentCPF != CPF) {
        return 1;
    }

    // 2: senha atual incorreta
    if (strcmp(currentLogin->currentPassword, currentPass) != 0) {
        return 2;
    }

    // 3: nova senha com menos de 8 caracteres
    if (strlen(newPass) < 8) {
        return 3;
    }

    // Atualiza senha
    free(currentLogin->currentPassword);
    currentLogin->currentPassword = duplicateString(newPass);
    alterPassword(CPF, newPass);
    return 0;
}

//Muda o numero de telefone do perfil que ta associando a esse login
int changeNumber(long int CPF, char* password, char* newNumber) {
    // 1: usuário não encontrado ou CPF não confere
    if (currentLogin == NULL || currentLogin->currentCPF != CPF) {
        return 1;
    }

    // 2: senha incorreta
    if (strcmp(currentLogin->currentPassword, password) != 0) {
        return 2;
    }

    // 3: novo número inválido
    if (!isValidPhone(newNumber)) {
        return 3;
    }

    alterNumber(CPF, newNumber);
    return 0;
}

//Salvar todos os logins da arvore em um arquivo
int saveLogins(FILE* file, LoginTree* root) {
    if (file == NULL) return 1; // Ponteiro de arquivo inválido
    if (root == NULL) return 2; // Nada para salvar aqui

    // Escreve CPF e senha em texto (uma linha por login)
    fprintf(file, "%ld;%s\n", root->loginData->currentCPF, root->loginData->currentPassword);

    // Recursão nos filhos
    saveLogins(file, root->left);
    saveLogins(file, root->right);

    return 0; // Sucesso
}

//Ler todos os logins do arquivo e adicionar na arvore
int readLogins(FILE* file, LoginTree** root) {
    if (file == NULL) return 1; // Ponteiro inválido
    if (root == NULL) return 2; // Nada para salvar aqui

    char linha[256];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        long int cpf;
        char senha[200];

        // Espera o formato: 12345678901;senha
        if (sscanf(linha, "%ld;%199[^\n]", &cpf, senha) != 2) {
            continue; // Pula linha mal formatada
        }

        // Cria novo login
        Login* novo = (Login*) malloc(sizeof(Login));
        if (novo == NULL) return -1;

        novo->currentCPF = cpf;
        novo->currentPassword = duplicateString(senha); // duplica string

        addLoginToTree(root, novo);
    }

    return 0; // Sucesso
}
