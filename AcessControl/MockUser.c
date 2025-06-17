#include <stdio.h>
#include <string.h>

//Funções Mock para teste
int createUserProfile(char* name, char* numeroTel, char* birthdate, long int CPF, char* password) {
    // Simula erro baseado em entradas específicas
    if (strcmp(name, "6767") == 0) return 1;
    if (CPF == 22222222222) return 2;       // CPF já existente
    if (CPF == 123) return 3;       
    if (strcmp(birthdate, "00/00/0000") == 0) return 4;
    if (strcmp(birthdate, "01/01/2020") == 0) return 5;
    if (strcmp(password, "12") == 0) return 6; 
    if (strcmp(numeroTel, "123") == 0) return 7;

    return 0; // sucesso
}

int alterPassword(long int CPF, char* newPass) {
    return 0; // Simula sucesso
}

int alterNumber(long int CPF, char* newNumber) {
    return 0; // Simula sucesso
}
