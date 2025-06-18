#include "AcessControl.h"
#include "MockUser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Códigos de erro (conforme AcessControl/acess_control_test.txt)
 * Somente para as funções de acesso.                
 * -------------------------------------------------------------------------*/
#define OK 0

/* login */
#define LOGIN_CPF_INVALID      1
#define LOGIN_CPF_NOT_FOUND    2
#define LOGIN_WRONG_PASSWORD   3

/* registerUser */
#define REG_NAME_INVALID       1
#define REG_CPF_DUPLICATE      2
#define REG_CPF_INVALID        3
#define REG_DATE_INVALID       4
#define REG_UNDERAGE           5
#define REG_SHORT_PASSWORD     6
#define REG_PHONE_INVALID      7

/* logout */
#define LOGOUT_CPF_INVALID     1  /* inclui tentativa de logout sem usuário logado */

/* changePassword */
#define CP_USER_NOT_FOUND      1  /* ponteiro / CPF não encontrado ou usuário não logado */
#define CP_WRONG_PASSWORD      2
#define CP_SHORT_NEW_PASS      3

/* changeNumber */
#define CN_USER_NOT_FOUND      1
#define CN_WRONG_PASSWORD      2
#define CN_INVALID_NUMBER      3

static void test_registerUser(void)
{
    /* Sucesso */
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 11111111111, "senha123") == OK);

    /* Falhas específicas */
    assert(registerUser("6767", "11999999999", "01/01/2000", 11111111112, "senha123") == REG_NAME_INVALID);
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 22222222222, "senha123") == REG_CPF_DUPLICATE);
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 123, "senha123") == REG_CPF_INVALID);
    assert(registerUser("Rafael", "11999999999", "00/00/0000", 11111111113, "senha123") == REG_DATE_INVALID);
    assert(registerUser("Rafael", "11999999999", "01/01/2020", 11111111114, "senha123") == REG_UNDERAGE);
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 11111111115, "12") == REG_SHORT_PASSWORD);
    assert(registerUser("Rafael", "123", "01/01/2000", 11111111116, "senha123") == REG_PHONE_INVALID);

    printf("[OK] registerUser\n");
}

static void test_login(void)
{
    assert(login(11111111111, "senha123") == OK);
    assert(login(0,             "senha123") == LOGIN_CPF_INVALID);
    assert(login(22222222222,   "senha123") == LOGIN_CPF_NOT_FOUND);
    assert(login(11111111111,   "errada")   == LOGIN_WRONG_PASSWORD);

    printf("[OK] login\n");
}

static void test_getLoginCPF(void)
{
    long int cpf = 0;

    // Deve retornar CPF do logado
    assert(getLoginCPF(&cpf) == OK);
    assert(cpf == 11111111111);

    // Desloga e testa erro
    logout(11111111111);
    assert(getLoginCPF(&cpf) == 1); /* único código de erro definido */

    printf("[OK] getLoginCPF\n");
}

static void test_logout(void)
{
    login(11111111111, "senha123");

    // Logout correto
    assert(logout(11111111111) == OK);

    // Logout sem usuário logado ou CPF inválido
    assert(logout(11111111111) == LOGOUT_CPF_INVALID);

    // Logout incorreto (CPF errado)
    login(11111111111, "senha123");
    assert(logout(99999999999) == LOGOUT_CPF_INVALID);

    printf("[OK] logout\n");
}

static void test_changePassword(void)
{
    login(11111111111, "senha123");

    // Nova senha muito curta
    assert(changePassword(11111111111, "senha123", "123") == CP_SHORT_NEW_PASS);

    // Senha atual errada
    assert(changePassword(11111111111, "errada", "novaSenha123") == CP_WRONG_PASSWORD);

    // CPF errado
    assert(changePassword(99999999999, "senha123", "novaSenha123") == CP_USER_NOT_FOUND);

    // Sucesso
    assert(changePassword(11111111111, "senha123", "novaSenha123") == OK);
    
    //Mudar a senha sem estar nenhuma conta logada
    logout(11111111111);
    assert(changePassword(11111111111, "senha123", "novaSenha123") == CP_USER_NOT_FOUND);

    printf("[OK] changePassword\n");
}

static void test_changeNumber(void)
{
    login(11111111111, "novaSenha123");

    // Correto
    assert(changeNumber(11111111111, "novaSenha123", "11988887777") == OK);

    // Senha errada
    assert(changeNumber(11111111111, "errada", "11988887777") == CN_WRONG_PASSWORD);

    // CPF errado
    assert(changeNumber(22222222222, "novaSenha123", "11988887777") == CN_USER_NOT_FOUND);

    // Telefone inválido
    assert(changeNumber(11111111111, "novaSenha123", "abc") == CN_INVALID_NUMBER);

    //Mudar a senha sem estar nenhuma conta logada
    logout(11111111111);
    assert(changeNumber(11111111111, "novaSenha123", "11988887777") == CN_USER_NOT_FOUND);

    printf("[OK] changeNumber\n");
}

static void test_save_and_read_logins(void)
{
    // Cria e registra dois usuários
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 33333333333, "senha1") == OK);
    assert(registerUser("Marina", "11999999988", "01/01/1995", 44444444444, "senha2") == OK);

    // Salva em arquivo
    FILE* outFile = fopen("logins_test.txt", "w");
    assert(outFile != NULL);
    assert(saveLogins(outFile, loginRoot) == 0);
    fclose(outFile);

    // Limpa a árvore original para simular reinício
    loginRoot = NULL;

    // Lê do arquivo e reconstrói árvore
    FILE* inFile = fopen("logins_test.txt", "r");
    assert(inFile != NULL);
    assert(readLogins(inFile, &loginRoot) == 0);
    fclose(inFile);

    // Verifica se os dados foram restaurados corretamente
    assert(login(33333333333, "senha1") == OK);
    assert(login(44444444444, "senha2") == OK);

    printf("[OK] save_and_read_logins\n");
}

int main() {
    test_registerUser();
    test_login();
    test_getLoginCPF();
    test_logout();
    test_changePassword();
    test_changeNumber();
    test_save_and_read_logins();

    printf("\nTodos os testes passaram com sucesso!\n");
    return 0;
}
