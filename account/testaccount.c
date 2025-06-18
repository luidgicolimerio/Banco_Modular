#include <assert.h>
#include <stdio.h>
#include "accounttests.h"

#define OK 0
#define ERR_ALLOC (-1)

/* createSavingsAccount / createCheckingAccount não possuem código de duplicado na especificação,
   então validamos apenas sucesso. */

static void test_create_accounts(void)
{
    assert(createSavingsAccount(11111111111) == OK);
    assert(createCheckingAccount(11111111111) == OK);
    printf("[OK] createSavingsAccount / createCheckingAccount\n");
}

static void test_update_balances(void)
{
    /* crédito */
    assert(updateSavingsAccountBal(11111111111, 100.0) == OK);
    assert(updateCheckingAccountBal(11111111111, 200.0) == OK);

    /* débito que deixa saldo negativo */
    assert(updateSavingsAccountBal(11111111111, -200.0) == 1);
    assert(updateCheckingAccountBal(11111111111, -300.0) == 1);

    /* CPF inexistente */
    assert(updateSavingsAccountBal(99999999999, 50.0) == 2);
    assert(updateCheckingAccountBal(99999999999, 50.0) == 2);
    printf("[OK] updateSavings/CheckingAccountBal\n");
}

static void test_get_balance(void)
{
    double bal;

    assert(getBalanceByType(11111111111, &bal, "savings") == OK);
    assert(getBalanceByType(11111111111, &bal, "checking") == OK);

    assert(getBalanceByType(99999999999, &bal, "checking") == 1);
    assert(getBalanceByType(99999999999, &bal, "savings") == 1);

    assert(getBalanceByType(11111111111, NULL, "savings") == 2); /* ponteiro balance NULL */
    assert(getBalanceByType(11111111111, &bal, NULL) == 2);       /* ponteiro type NULL   */
    assert(getBalanceByType(11111111111, &bal, "foo") == 3);     /* tipo inválido        */
    printf("[OK] getBalanceByType\n");
}

static void test_save_read(void)
{
    FILE *fchk = fopen("checking_tmp.txt", "w");
    FILE *fsav = fopen("savings_tmp.txt", "w");
    assert(saveCheckingAccounts(fchk) == OK);
    assert(saveSavingsAccounts(fsav) == OK);
    fclose(fchk); fclose(fsav);

    /* ponteiro inválido */
    assert(saveCheckingAccounts(NULL) == 1);
    assert(saveSavingsAccounts(NULL) == 1);

    fchk = fopen("checking_tmp.txt", "r");
    fsav = fopen("savings_tmp.txt", "r");
    assert(readCheckingAccounts(fchk) == OK);
    assert(readSavingsAccounts(fsav) == OK);
    fclose(fchk); fclose(fsav);

    assert(readCheckingAccounts(NULL) == 1);
    assert(readSavingsAccounts(NULL) == 1);

    printf("[OK] save/read accounts\n");
}

int main(void)
{
    printf("==== Testes ACCOUNT ===\n");
    test_create_accounts();
    test_update_balances();
    test_get_balance();
    test_save_read();
    printf("\nTodos os testes ACCOUNT concluídos com sucesso!\n");
    return 0;
}



