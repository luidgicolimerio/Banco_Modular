#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

// Incluímos o módulo a ser testado 
#include "transactions.c"

// Mapeamento dos códigos de retorno
#define OK                          0

// addTransactionToList
#define TRX_PTR_NULL                1
#define TRX_MEM_ALLOC_ERR          -1

// makeTransactionSavings
#define TRXS_INVALID_VALUE          1
#define TRXS_INVALID_CPF            2
#define TRXS_CPF_NOT_FOUND          3
#define TRXS_INVALID_TYPE           4
#define TRXS_INSUFFICIENT_BALANCE   5
#define TRXS_MEM_ALLOC_ERR         -1

// getTransactions / showTransactions
#define TRXCPF_INVALID_CPF          1
#define TRXCPF_NOT_FOUND            2
#define TRXCPF_NO_TRANSACTIONS      3

// addTransactionListToList
#define TRXLL_PTR_INVALID           1
#define TRXLL_BAD_NODE              2

// makeTransactionChecking
#define TRXC_INVALID_VALUE          1
#define TRXC_INVALID_TARGET_CPF     2
#define TRXC_INVALID_SOURCE_CPF     3
#define TRXC_SOURCE_NOT_FOUND       4
#define TRXC_TARGET_NOT_FOUND       5
#define TRXC_MEM_ALLOC_ERR         -1

// Funções auxiliares 
static struct tm now_tm(void){
    time_t now = time(NULL);
    return *localtime(&now);
}

static void test_addTransactionToList_success(){
    transaction *tx = malloc(sizeof *tx);
    tx->val = 100.0;
    tx->sourceCPF = 11111111111;
    tx->targetCPF = 0;
    tx->date = now_tm();
    strcpy(tx->type, "deposit");

    transactionList first = { .root = NULL, .transaction = tx, .next = NULL };

    int rc = addTransactionToList(&first, tx);
    assert(rc == OK);
    printf("[OK] addTransactionToList sucesso\n");
}

static void test_addTransactionToList_null(){
    transaction *tx = NULL;
    int rc = addTransactionToList(NULL, tx);
    assert(rc == TRX_PTR_NULL);
    printf("[OK] addTransactionToList ponteiro nulo\n");
}

static void test_addTransactionListToList(){
    transaction *tx = malloc(sizeof *tx);
    tx->val = 50.0;
    tx->sourceCPF = 22222222222;
    tx->targetCPF = 0;
    tx->date = now_tm();
    strcpy(tx->type, "deposit");

    transactionList *head = malloc(sizeof *head);
    head->root = head;
    head->transaction = tx;
    head->next = NULL;

    int rc = addTransactionListToList(head);
    assert(rc == OK);
    printf("[OK] addTransactionListToList sucesso (nova lista)\n");

    /* tentar adicionar outra transação para o mesmo CPF deve retornar 3 */
    transaction *tx2 = malloc(sizeof *tx2);
    *tx2 = *tx;
    tx2->val = 20.0;

    transactionList second = { .root = head, .transaction = tx2, .next = NULL };
    rc = addTransactionListToList(&second);
    assert(rc == TRXCPF_NO_TRANSACTIONS || rc == OK);
    printf("[OK] addTransactionListToList já existente\n");
}

static void test_makeTransactionSavings(){
    struct tm date = now_tm();
    int rc;

    /* caso de sucesso */
    rc = makeTransactionSavings(200.0, 28675806760, 0, 0, date);
    assert(rc == OK);
    printf("[OK] makeTransactionSavings sucesso (depósito)\n");

    /* valor inválido */
    rc = makeTransactionSavings(-10.0, 28675806760, 0, 0, date);
    assert(rc == TRXS_INVALID_VALUE);
    printf("[OK] makeTransactionSavings valor inválido\n");

    /* tipo inválido */
    rc = makeTransactionSavings(20.0, 28675806760, 0, 7, date);
    assert(rc == TRXS_INVALID_TYPE);
    printf("[OK] makeTransactionSavings tipo inválido\n");
}

static void test_showTransactions(){
    int rc = showTransactions(28675806760);
    assert(rc == OK);
    printf("[OK] showTransactions sucesso\n");

    rc = showTransactions(41578080703); // CPF sem transações
    assert(rc == TRXCPF_NO_TRANSACTIONS);
    printf("[OK] showTransactions sem transações\n");
}

static void test_saveTransactions(){
    FILE *f = fopen("/tmp/tx.csv", "w");
    int rc = saveTransactions(f);
    assert(rc == OK);
    fclose(f);
    printf("[OK] saveTransactions sucesso\n");

    rc = saveTransactions(NULL);
    assert(rc == 1); // ponteiro inválido
    printf("[OK] saveTransactions ponteiro inválido\n");
}

static void test_readTransactions(){
    /* cria arquivo temporário com uma transação */
    const char *path = "/tmp/tx_read.txt";
    FILE *fw = fopen(path, "w");
    fprintf(fw, "12345678900,0,100.00,deposit,01/01/2023 00:00:00\n");
    fclose(fw);

    FILE *fr = fopen(path, "r");
    int rc = readTransactions(fr);
    assert(rc == OK);
    fclose(fr);
    printf("[OK] readTransactions sucesso\n");

    rc = readTransactions(NULL);
    assert(rc == 1);
    printf("[OK] readTransactions ponteiro inválido\n");
}

int main(){
    printf("==== Iniciando testes de transactions ===\n");

    test_addTransactionToList_success();
    test_addTransactionToList_null();
    test_addTransactionListToList();
    test_makeTransactionSavings();
    test_showTransactions();
    test_saveTransactions();
    test_readTransactions();

    printf("\nTodos os testes concluídos com sucesso!\n");
    return 0;
}