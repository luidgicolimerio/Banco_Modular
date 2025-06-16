
#include <assert.h>
#include <stdio.h>
#include "accounttests.h"

void test_create_accounts() {
    assert(createSavingsAccount(11111111111) == 0);
    printf("Created savings account.\n");

    assert(createSavingsAccount(11111111111) == 1);
    printf("Duplicate savings account detected.\n");

    assert(createCheckingAccount(11111111111) == 0);
    printf("Created checking account.\n");

    assert(createCheckingAccount(11111111111) == 1);
    printf("Duplicate checking account detected.\n");
}

void test_update_balances() {
    assert(updateSavingsAccountBal(11111111111, 100.0) == 0);
    printf("Savings account credited.\n");

    assert(updateCheckingAccountBal(11111111111, 200.0) == 0);
    printf("Checking account credited.\n");

    assert(updateSavingsAccountBal(11111111111, -200.0) == 1);
    printf("Insufficient funds in savings.\n");

    assert(updateCheckingAccountBal(11111111111, -300.0) == 1);
    printf("Insufficient funds in checking.\n");

    assert(updateSavingsAccountBal(99999999999, 50.0) == 2);
    printf("Invalid CPF for savings.\n");

    assert(updateCheckingAccountBal(99999999999, 50.0) == 2);
    printf("Invalid CPF for checking.\n");
}

void test_get_balance_by_type() {
    double balance;

    assert(getBalanceByType(11111111111, &balance, "savings") == 0);
    printf("Balance for savings: %.2f\n", balance);

    assert(getBalanceByType(11111111111, &balance, "checking") == 0);
    printf("Balance for checking: %.2f\n", balance);

    assert(getBalanceByType(99999999999, &balance, "savings") == 1);
    printf("Handled invalid CPF for savings.\n");

    assert(getBalanceByType(99999999999, &balance, "checking") == 1);
    printf("Handled invalid CPF for checking.\n");

    assert(getBalanceByType(11111111111, NULL, "savings") == 2);
    printf("Handled null balance pointer.\n");

    assert(getBalanceByType(11111111111, &balance, NULL) == 2);
    printf("Handled null type pointer.\n");

    assert(getBalanceByType(11111111111, &balance, "invalid") == 3);
    printf("Handled invalid account type string.\n");
}

void test_save_and_read_accounts() {
    FILE* f1 = fopen("checking_test.txt", "w");
    FILE* f2 = fopen("savings_test.txt", "w");

    assert(saveCheckingAccounts(f1) == 0);
    printf("Saved checking accounts to file.");

    assert(saveSavingsAccounts(f2) == 0);
    printf("Saved savings accounts to file.");

    fclose(f1);
    fclose(f2);

    assert(saveCheckingAccounts(NULL) == 1);
    printf("Handled null file for checking save.");

    assert(saveSavingsAccounts(NULL) == 1);
    printf("Handled null file for savings save.");

    f1 = fopen("checking_test.txt", "r");
    f2 = fopen("savings_test.txt", "r");

    assert(readCheckingAccounts(f1) == 0);
    printf("Read checking accounts from file.");

    assert(readSavingsAccounts(f2) == 0);
    printf("Read savings accounts from file.");

    fclose(f1);
    fclose(f2);

    assert(readCheckingAccounts(NULL) == 1);
    printf("Handled null file for checking read.");

    assert(readSavingsAccounts(NULL) == 1);
    printf("Handled null file for savings read.");
}

int main() {
    test_save_and_read_accounts();
    test_create_accounts();
    test_update_balances();
    test_get_balance_by_type();
    printf("Account tests completed.\n");
    return 0;
}



