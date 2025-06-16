#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "transactions.h"  /* apenas API pública */

#define OK 0
#define TRXS_INVALID_VALUE 1
#define TRXS_INVALID_TYPE  4
#define TRXCPF_NO_TRANSACTIONS 3

static struct tm now_tm(void)
{
    time_t now = time(NULL);
    return *localtime(&now);
}

/* Testa makeTransactionSavings com cenários de sucesso/erro */
static void test_makeTransactionSavings(void)
{
    struct tm date = now_tm();
    long int cpf = 24066522700;

    int rc = makeTransactionSavings(500.0, cpf, 0, 0, date);
    assert(rc == OK);
    printf("[OK] makeTransactionSavings sucesso\n");

    rc = makeTransactionSavings(-10.0, cpf, 0, 0, date);
    assert(rc == TRXS_INVALID_VALUE);
    printf("[OK] makeTransactionSavings valor inválido\n");

    rc = makeTransactionSavings(25.0, cpf, 0, 7, date);
    assert(rc == TRXS_INVALID_TYPE);
    printf("[OK] makeTransactionSavings tipo inválido\n");
}

/* Testa makeTransactionChecking entre dois CPFs */
static void test_makeTransactionChecking(void)
{
    struct tm date = now_tm();
    long int src = 41578080703;
    long int dst = 28675806760;

    /* primeiro garante algum saldo na origem */
    makeTransactionSavings(300.0, src, 0, 0, date);

    int rc = makeTransactionChecking(150.0, src, dst, date);
    assert(rc == OK);
    printf("[OK] makeTransactionChecking sucesso\n");
}

/* Testa showTransactions para CPFs com/sem lançamentos */
static void test_showTransactions(void)
{
    long int cpfCom = 41578080703; /* já recebeu transações nos testes anteriores */
    long int cpfSem = 99999999999;

    int rc = showTransactions(cpfCom);
    assert(rc == OK);
    printf("[OK] showTransactions (possui lançamentos)\n");

    rc = showTransactions(cpfSem);
    assert(rc == TRXCPF_NO_TRANSACTIONS);
    printf("[OK] showTransactions (sem lançamentos)\n");
}

/* Testa salvar e ler transações */
static void test_save_and_read(void)
{
    const char *path = "/tmp/transactions_test.txt";
    FILE *f = fopen(path, "w");
    int rc = saveTransactions(f);
    assert(rc == OK);
    fclose(f);
    printf("[OK] saveTransactions sucesso\n");

    f = fopen(path, "r");
    rc = readTransactions(f);
    assert(rc == OK);
    fclose(f);
    printf("[OK] readTransactions sucesso\n");
}

int main(void)
{
    printf("==== Iniciando testes (API pública) ===\n");

    test_makeTransactionSavings();
    test_makeTransactionChecking();
    test_showTransactions();
    test_save_and_read();

    printf("\nTodos os testes da API concluídos com sucesso!\n");
    return 0;
}