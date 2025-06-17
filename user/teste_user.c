#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "user.h"

#define CPF1 12345654321LL
#define CPF2 22222222222LL

int main(void)
{
	/* 2.1 createUserProfile */

	// Nome inv�lido
	assert(createUserProfile("", "21999999999", "01/01/2000", CPF1, "12345678") == 1);
	// CPF inv�lido
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", 123L, "12345678") == 3);
	// Data de nascimento inv�lida
	assert(createUserProfile("Pessoa", "21999999999", "a0/01/2000", CPF1, "12345678") == 4);
	// Menor de idade
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2020", CPF1, "12345678") == 5);
	// Senha curta
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", CPF1, "1234567") == 6);
	// Telefone inv�lido
	assert(createUserProfile("Pessoa", "12345", "01/01/2000", CPF1, "12345678") == 7);
	// Sucesso
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", CPF1, "12345678") == 0);
	// CPF j� existente
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", CPF1, "12345678") == 2);


	/* 2.2 getUser */

	userProfile teste = { "Teste", "01/01/2000", CPF2, "12345678", "21999999999" };
	userProfileTree noTeste = { &teste, NULL, NULL };
	// Sucesso
	assert(getUser(CPF2, &noTeste) == 0);
	// CPF inv�lido
	assert(getUser(123L, &noTeste) == 1);
	// N�o encontrado
	assert(getUser(CPF1, &noTeste) == 2);


	/* 2.3 alterPassword */

	// CPF n�o encontrado
	assert(alterPassword(CPF2, "senhaNova") == 1);
	// Senha curta
	assert(alterPassword(CPF1, "1234567") == 2);
	// Sucesso
	assert(alterPassword(CPF1, "senha12345") == 0);


	/* 2.4 alterNumber */

	// CPF n�o encontrado
	assert(alterNumber(CPF2, "21987654321") == 1);
	// N�mero inv�lido
	assert(alterNumber(CPF1, "1234") == 2);
	// Sucesso
	assert(alterNumber(CPF1, "21987654321") == 0);


	/* 2.5 addProfileToTree */

	userProfile* perfilTeste = malloc(sizeof(userProfile));
	if (perfilTeste == NULL)
	{
		printf("Erro na aloca��o de memoria para teste de addProfileToTree\n");
		return 1;
	}
	perfilTeste->name = "Nome";
	perfilTeste->birthdate = "01/01/1950";
	// Perfil j� existe
	perfilTeste->CPF = CPF1;
	perfilTeste->password = "12345678";
	perfilTeste->telNum = "21999999999";
	assert(addProfileToTree(perfilTeste) == 1);
	free(perfilTeste);


	/* 2.6 saveUserProfiles e 2.7 readUserProfiles */

	// Ponteiro inv�lido
	assert(saveUserProfiles(NULL) == 1);
	// Ponteiro inv�lido
	assert(readUserProfiles(NULL) == 1);
	// Sucesso
	FILE* arq = fopen("arquivo.bin", "wb");
	assert(arq != NULL);
	assert(saveUserProfiles(arq) == 0);
	fclose(arq);
	// Sucesso
	arq = fopen("arquivo.bin", "rb");
	assert(arq != NULL);
	assert(readUserProfiles(arq) == 0);
	fclose(arq);

	printf("Ok!\n");
	return 0;
}