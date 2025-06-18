#ifndef USER_H
#define USER_H

#include <stdio.h>

// Encapsulated (opaque) structures. Campos internos são definidos apenas em user.c
typedef struct userProfile userProfile;
typedef struct userProfileTree userProfileTree;

/* 
Nome: createUserProfile
Objetivo: criar e inserir um perfil na árvore (árvore global)
Acoplamento: 
param1 char* name
param2 char* numeroTel
param3 char* birthdate
param4 long long CPF
param5 char* password
ret int
Condições de acoplamento: CPF com 11 dígitos, senha ≥8 caracteres, data "DD/MM/AAAA", se ret==0 a árvore contém o perfil
Descrição:
0 =  Usuário criado com sucesso
1 =  Nome inválido
2 =  CPF já existente
3 =  CPF inválido
4 =  Data de nascimento inválida
5 =  Data de nascimento de menor de idade
6 =  Senha tem menos 8 caracteres
7 = Número de telefone inválido
-1 = Erro de alocação de memória
Hipóteses: árvore já inicializada ou vazia
Restrições: não grava perfil se qualquer validação falhar
*/
int createUserProfile(char* name, char* numeroTel, char* birthdate, long long CPF, char* password);

/*
Nome: getUser
Objetivo: procurar o CPF na árvore
Acoplamento: 
param1 long long CPF
param2 userProfileTree* raizSub
ret int
Condições de acoplamento: raiz pode ser NULL
Descrição: 
0 = Usuário encontrado com sucesso
1 = CPF inválido
2 = CPF não encontrado
Hípóteses: a árvore é uma ABB de CPF
Restrições: tempo de busca de ABB, que é tempo O(h)
*/
int getUser(long long CPF, userProfileTree* no);

/*
Nome: alterPassword
Objetivo: trocar a senha do perfil
Acoplamento:
param1 long long CPF
param2 const char* novaSenha
ret int
Descrição:
0 = Senha alterada com sucesso
1 = User CPF não encontrado
2 = Nova senha com menos de 8 caracteres
Hípóteses: novaSenha já alocada pelo chamador
Restrições: libera senha antiga mas duplica a nova
*/
int alterPassword(long long CPF, const char* newPass);

/*
Nome: alterNumber
Objetivo: alterar telefone do perfil
Acoplamento: 
param1 long long CPF
param2 const char* novoTel
ret int
Descrição:
0 = Número alterado com sucesso
1 = User CPF não encontrado
2 = Novo número inválido
Hípóteses: telefone deve conter 8-13 dígitos
Restrições: duplica a string pra dentro do perfil
*/
int alterNumber(long long CPF, const char* numero);

/*
Nome: addProfileToTree
Objetivo: inserir um nó na árvore (árvore global)
Acoplamento: 
param1 userprofile* profilePtr
ret int
Descrição:
0 = Usuário encontrado com sucesso.
1 = Perfil já existe.
Hípóteses: profilePtr já alocado e todo preenchido
Restrições: Se não for ABB com balanceamento
*/
int addProfileToTree(userProfile* profile);

/*
Nome: saveUserProfiles
Objetivo: gravar a árvore em um arquivo binário
Acoplamento: 
param1 FILE* arquivo
ret int
Descrição:
0 = Perfis escritos com sucesso.
1 = Ponteiro de arquivo inválido.
Hípóteses: arquivo já aberto em modo wb
Restrições: só pré-ordem e grava tamanho + string + CPF
*/
int saveUserProfiles(FILE* arquivo);

/*
Nome: readUserProfiles
Objetivo: recriar uma árvore a partir de um arquivo binário
Acoplamento: 
param1 FILE* arquivo
ret int
Descrição:
0 = Leitura de sucesso.
1 = Ponteiro de arquivo inválido.
Hípóteses: arquivo já aberto em modo rb e salvo por saveUserProfiles
Restrições: limpa a árvore antiga??
*/
int readUserProfiles(FILE* arquivo);

#endif