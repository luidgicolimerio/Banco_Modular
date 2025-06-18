#include <assert.h>
#include <stdio.h>
#include "user.h"

#define OK 0

/* Códigos de retorno conforme user_test.txt */
#define NAME_INVALID 1
#define CPF_DUPLICATE 2
#define CPF_INVALID 3
#define GETCPF_INVALID 1
#define DATE_INVALID 4
#define UNDERAGE 5
#define SHORT_PASS 6
#define PHONE_INVALID 7
#define ERR_ALLOC -1

#define USER_NOT_FOUND 1
#define PASS_TOO_SHORT 2
#define NUMBER_INVALID 2

#define CPF1 12345654321LL
#define CPF2 22222222222LL

static void test_createUserProfile(void)
{
	assert(createUserProfile("", "21999999999", "01/01/2000", CPF1, "12345678") == NAME_INVALID);
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", 123L, "12345678") == CPF_INVALID);
	assert(createUserProfile("Pessoa", "21999999999", "a0/01/2000", CPF1, "12345678") == DATE_INVALID);
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2020", CPF1, "12345678") == UNDERAGE);
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", CPF1, "1234567") == SHORT_PASS);
	assert(createUserProfile("Pessoa", "12345", "01/01/2000", CPF1, "12345678") == PHONE_INVALID);
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", CPF1, "12345678") == OK);
	assert(createUserProfile("Pessoa", "21999999999", "01/01/2000", CPF1, "12345678") == CPF_DUPLICATE);
	printf("[OK] createUserProfile\n");
}

static void test_getUser(void)
{
	/* sucesso (já criado antes) */
	assert(getUser(CPF1, NULL) == OK);
	/* CPF inválido */
	assert(getUser(123L, NULL) == GETCPF_INVALID);
	/* não encontrado */
	assert(getUser(CPF2, NULL) == 2);
	printf("[OK] getUser\n");
}

static void test_alterPassword(void)
{
	/* usuário inexistente */
	assert(alterPassword(CPF2, "abc12345") == USER_NOT_FOUND);
	/* nova senha curta */
	assert(alterPassword(CPF1, "1234567") == PASS_TOO_SHORT);
	/* sucesso */
	assert(alterPassword(CPF1, "senhaNova123") == OK);
	printf("[OK] alterPassword\n");
}

static void test_alterNumber(void)
{
	/* usuário inexistente */
	assert(alterNumber(CPF2, "21987654321") == USER_NOT_FOUND);
	/* número inválido */
	assert(alterNumber(CPF1, "1234") == NUMBER_INVALID);
	/* sucesso */
	assert(alterNumber(CPF1, "21987654321") == OK);
	printf("[OK] alterNumber\n");
}

static void test_save_read(void)
{
	assert(saveUserProfiles(NULL) == 1);
	assert(readUserProfiles(NULL) == 1);

	FILE* f = fopen("user_test.bin", "wb");
	assert(f && saveUserProfiles(f) == OK);
	fclose(f);

	f = fopen("user_test.bin", "rb");
	assert(f && readUserProfiles(f) == OK);
	fclose(f);
	printf("[OK] saveUserProfiles / readUserProfiles\n");
}

int main(void)
{
	printf("==== Testes USER ====""\n");
	test_createUserProfile();
	test_getUser();
	test_alterPassword();
	test_alterNumber();
	test_save_read();
	printf("\nTodos os testes USER concluídos com sucesso!\n");
	return 0;
}