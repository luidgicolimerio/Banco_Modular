#ifndef ACCOUNT_H_TESTS
#define ACCOUNT_H_TESTS

#include <stdio.h>

int updateCheckingAccountBal(long int CPF, double val);
int createSavingsAccount(long int CPF);
int createCheckingAccount(long int CPF);
int updateSavingsAccountBal(long int CPF, double val);
int getBalanceByType(long int CPF, double* balance, const char* type);
int saveSavingsAccounts(FILE* file);
int saveCheckingAccounts(FILE* file);
int readCheckingAccounts(FILE* file);
int readSavingsAccounts(FILE* file);
#endif // ACCOUNT_H_TESTS
