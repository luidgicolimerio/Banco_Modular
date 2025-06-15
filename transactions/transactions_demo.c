#include <stdio.h>
#include <time.h>
#include <string.h>

/* Inclui diretamente o módulo de transações (código já contém tudo que precisa) */
#include "transactions.c"

static struct tm now_tm(void)
{
    time_t now = time(NULL);
    return *localtime(&now);
}

int main(void)
{
    /* CPFs utilizados no teste */
    long int cpf1 = 24066522700;
    long int cpf2 = 41578080703;
    long int cpf3 = 28675806760;

    struct tm today = now_tm();

    printf("\n=== DEMO: Depósitos em poupança ===\n");
    makeTransactionSavings(500.0, cpf1, 0, 0, today); // depósito CPF1
    makeTransactionSavings(300.0, cpf2, 0, 0, today); // depósito CPF2

    printf("\n=== DEMO: Transferências entre contas correntes ===\n");
    makeTransactionChecking(150.0, cpf1, cpf2, today); // CPF1 -> CPF2
    makeTransactionChecking( 50.0, cpf2, cpf3, today); // CPF2 -> CPF3

    printf("\n=== DEMO: Novo depósito em poupança ===\n");
    makeTransactionSavings(700.0, cpf3, 0, 0, today); // depósito CPF3

    printf("\n=== Exibindo transações por CPF ===\n");
    showTransactions(cpf1);
    showTransactions(cpf2);
    showTransactions(cpf3);

    printf("\n=== Salvando todas as transações em transactions.txt ===\n");
    FILE *f = fopen("transactions.txt", "w");
    if (saveTransactions(f) == 0)
        printf("Arquivo salvo com sucesso.\n");
    fclose(f);

    return 0;
} 