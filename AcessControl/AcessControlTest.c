#include "AcessControl.h"
#include "MockUser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_registerUser() {
    // Registro bem-sucedido
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 11111111111, "senha123") == 0);

    // Simulações de falhas específicas
    assert(registerUser("6767", "11999999999", "01/01/2000", 11111111112, "senha123") == 1); // nome inválido
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 22222222222, "senha123") == 2); // CPF duplicado
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 123, "senha123") == 3); // CPF inválido
    assert(registerUser("Rafael", "11999999999", "00/00/0000", 11111111113, "senha123") == 4); // data inválida
    assert(registerUser("Rafael", "11999999999", "01/01/2020", 11111111114, "senha123") == 5); // menor de idade
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 11111111115, "12") == 6); // senha curta
    assert(registerUser("Rafael", "123", "01/01/2000", 11111111116, "senha123") == 7); // telefone inválido
}

void test_login() {
    // Login bem-sucedido
    assert(login(11111111111, "senha123") == 0);

    // CPF inválido
    assert(login(0, "senha123") == 1);

    // CPF inexistente
    assert(login(22222222222, "senha123") == 2);

    // Senha incorreta
    assert(login(11111111111, "errada") == 3);

}

void test_getLoginCPF() {
    long int cpf = 0;

    // Deve retornar CPF do logado
    assert(getLoginCPF(&cpf) == 0);
    assert(cpf == 11111111111);

    // Desloga e testa erro
    logout(11111111111);
    assert(getLoginCPF(&cpf) == 1);
}

void test_logout() {
    login(11111111111, "senha123");

    // Logout correto
    assert(logout(11111111111) == 0);

    // Logout com ninguém logado
    assert(logout(11111111111) == 2);

    // Logout incorreto (CPF errado)
    login(11111111111, "senha123");
    assert(logout(99999999999) == 1);
}

void test_changePassword() {
    login(11111111111, "senha123");

    // Nova senha muito curta
    assert(changePassword(11111111111, "senha123", "123") == 3);

    // Senha atual errada
    assert(changePassword(11111111111, "errada", "novaSenha123") == 2);

    // CPF errado
    assert(changePassword(99999999999, "senha123", "novaSenha123") == 1);

    // Sucesso
    assert(changePassword(11111111111, "senha123", "novaSenha123") == 0);
    
    //Mudar a senha sem estar nenhuma conta logada
    logout(11111111111);
    assert(changePassword(11111111111, "senha123", "novaSenha123") == 4);
}

void test_changeNumber() {
    login(11111111111, "novaSenha123");

    // Correto
    assert(changeNumber(11111111111, "novaSenha123", "11988887777") == 0);

    // Senha errada
    assert(changeNumber(11111111111, "errada", "11988887777") == 2);

    // CPF errado
    assert(changeNumber(22222222222, "novaSenha123", "11988887777") == 1);

    // Telefone inválido
    assert(changeNumber(11111111111, "novaSenha123", "abc") == 3);

    //Mudar a senha sem estar nenhuma conta logada
    logout(11111111111);
    assert(changeNumber(11111111111, "novaSenha123", "novaSenha123") == 4);
}

void test_save_and_read_logins() {
    // Cria e registra dois usuários
    assert(registerUser("Rafael", "11999999999", "01/01/2000", 33333333333, "senha1") == 0);
    assert(registerUser("Marina", "11999999988", "01/01/1995", 44444444444, "senha2") == 0);

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
    assert(login(33333333333, "senha1") == 0);
    assert(login(44444444444, "senha2") == 0);

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
