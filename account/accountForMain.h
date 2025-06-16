#include <stdio.h>
int getBalanceByType(long int CPF, double* balance, const char* type);
int saveSavingsAccounts(FILE* file);
int saveCheckingAccounts(FILE* file);
int readCheckingAccounts(FILE* file);
int readSavingsAccounts(FILE* file);