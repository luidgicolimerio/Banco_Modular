#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <stdio.h>   /* FILE */
#include <time.h>    /* struct tm */

/*
 * Funções públicas do módulo transactions
 */
int makeTransactionSavings(double value, long int sourceCPF, long int targetCPF,
                           int depositOrWithdrawal, struct tm date);

int makeTransactionChecking(double value, long int sourceCPF, long int targetCPF,
                            struct tm date);

int showTransactions(long int CPF);

int saveTransactions(FILE *file);

int readTransactions(FILE *file);

#endif /* TRANSACTIONS_H */
