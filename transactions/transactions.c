#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "account.h"

typedef struct transaction {
    double      val;
    long int    sourceCPF;
    long int    targetCPF;
    struct tm   date;
    char        type[10];
} transaction;

typedef struct transactionList {
    struct transactionList *root;      // ponteiro para cabeça da lista
    transaction           *transaction;
    struct transactionList *next;
} transactionList;

typedef struct transactionListsList {
    transactionList                *list;   // lista de transações deste CPF
    struct transactionListsList    *next;   // próximo CPF
    long int                        CPF;    // chave
} transactionListsList;

// lista principal (encapsulada neste módulo) 
static transactionListsList *transactionListsHead = NULL;

// Procura a lista do CPF; devolve ponteiro ou NULL (uso interno)
static transactionListsList *findTransactionList(transactionListsList *head, long int CPF){
    for (; head; head = head->next)
        if (head->CPF == CPF)
            return head;
    return NULL;
}

// Garante que a lista para o CPF exista; cria se necessário (uso interno) 
static transactionListsList *getOrCreateTransactionList(long int CPF){
    transactionListsList *node = findTransactionList(transactionListsHead, CPF);
    if (node) {
        return node;
    }

    node = malloc(sizeof *node);
    node->CPF  = CPF;
    node->list = NULL;
    node->next = transactionListsHead;
    transactionListsHead = node;
    return node;
}

// adiciona a transação 'tx' à lista pertencente ao CPF informado 
static void addTransaction(long int CPF, transaction *tx){
    transactionListsList *owner = getOrCreateTransactionList(CPF);

    transactionList *newNode = malloc(sizeof *newNode);
    newNode->transaction = tx;
    newNode->next = NULL;

    if (!owner->list) {
        owner->list = newNode;
        newNode->root = newNode;        // cabeça
    } else {
        transactionList *cur = owner->list;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
        newNode->root = owner->list;    // aponta para cabeça
    }
}

// cria nó de transação e devolve ponteiro
static transactionList *newTransactionNode(transaction *tx){
    transactionList *node = malloc(sizeof *node);
    if (!node) return NULL;
    node->transaction = tx;
    node->next = NULL;
    node->root = node; /* provisório; será ajustado pela função chamadora */
    return node;
}

// Adiciona uma transação a uma lista já existente (lista pertence a um CPF).
static int addTransactionToList(transactionList *list, transaction *tx){
    if (!list) return 1;

    transactionList *newNode = newTransactionNode(tx);
    if (!newNode) return -1;

    newNode->root = list->root ? list->root : list; /* mantém referência */

    transactionList *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = newNode;
    return 0;
}

// Insere a lista de transações de um CPF na estrutura global.
static int addTransactionListToList(transactionList *list){
    if (!list)               return 1; // ponteiro inválido
    if (!list->transaction)  return 2; // nó mal-formado

    long int CPF = list->transaction->sourceCPF;

    transactionListsList *node = findTransactionList(transactionListsHead, CPF);
    if (node) {
        /* Já existe lista → apenas anexar transação */
        return addTransactionToList(node->list, list->transaction);
    }

    transactionListsList *newList = malloc(sizeof *newList);
    if (!newList) return -1; /* erro de alocação */

    newList->CPF  = CPF;
    newList->list = list;
    newList->next = transactionListsHead;
    transactionListsHead = newList;

    /* Ajusta campo root de todos os nós recém-inseridos */
    for (transactionList *p = list; p; p = p->next) p->root = list;

    return 0;
}

// 0 para depósito, 1 para saque
int makeTransactionSavings(double value, long int sourceCPF, long int targetCPF, int depositOrWithdrawal, struct tm date) {
    int result;
    if (value <= 0) {
        return 1; // valor inválido
    }

    if (sourceCPF <= 0) {
        return 2; // CPF inválido
    }

    if (depositOrWithdrawal != 0 && depositOrWithdrawal != 1) {
        return 4; // tipo de transação inválida
    }

    transaction* tx = malloc(sizeof *tx);
    tx->val = value;
    tx->sourceCPF = sourceCPF;
    tx->targetCPF = targetCPF;
    tx->date = date;

    if (depositOrWithdrawal == 0) {
        strcpy(tx->type, "deposit");
        result = updateSavingsAccountBalance(sourceCPF, value);
        if (result == 2) {
            return 3; // CPF nao encontrado
        }
    } else {
        strcpy(tx->type, "withdrawal");
        result = updateSavingsAccountBalance(sourceCPF, -value);
        if (result == 1) {
            return 5; // saldo insuficiente
        }
        if (result == 2) {
            return 3; // CPF nao encontrado
        }
    }

    addTransaction(sourceCPF, tx);
    return 0;
}

int makeTransactionChecking(double value, long int sourceCPF, long int targetCPF, struct tm date) { 
    int result;
    transaction* tx = malloc(sizeof *tx);
    tx->val = value;
    tx->sourceCPF = sourceCPF;
    tx->targetCPF = targetCPF;
    tx->date = date;
    strcpy(tx->type, "transfer");

    result = updateCheckingAccountBalance(sourceCPF, -value);
    if (result == 1) {
        return 5; // saldo insuficiente
    }
    if (result == 2) {
        return 3; // CPF nao encontrado
    }
    result = updateCheckingAccountBalance(targetCPF, value);
    if (result == 2) {
        return 3; // CPF nao encontrado
    }

    addTransaction(sourceCPF, tx);
    addTransaction(targetCPF, tx);
    return 0;
}

// Exibe todas as transações de um CPF. 
int showTransactions(long int CPF){
    transactionListsList *node = findTransactionList(transactionListsHead, CPF);
    if (!node) {
        // CPF sem transações
        return 3;
    }

    printf("\nTransações do CPF %ld:\n", CPF);
    for (transactionList *cur = node->list; cur; cur = cur->next) {
        transaction *t = cur->transaction;
        printf("  Valor: %.2f, Source: %ld, Target: %ld, Tipo: %s, Data: %02d/%02d/%04d %02d:%02d:%02d\n",
               t->val, t->sourceCPF, t->targetCPF, t->type,
               t->date.tm_mday, t->date.tm_mon + 1, t->date.tm_year + 1900,
               t->date.tm_hour, t->date.tm_min, t->date.tm_sec);
    }
    return 0;
}

// Salva todas as transações em um arquivo
int saveTransactions(FILE *file){
    if (!file) return 1;
    for (transactionListsList *cpfNode = transactionListsHead; cpfNode; cpfNode = cpfNode->next) {
        for (transactionList *txNode = cpfNode->list; txNode; txNode = txNode->next) {
            transaction *t = txNode->transaction;
            fprintf(file, "%ld,%ld,%.2f,%s,%02d/%02d/%04d %02d:%02d:%02d\n",
                    t->sourceCPF, t->targetCPF, t->val, t->type,
                    t->date.tm_mday, t->date.tm_mon + 1, t->date.tm_year + 1900,
                    t->date.tm_hour, t->date.tm_min, t->date.tm_sec);
        }
    }
    return 0;
}

// Lê transações gravadas previamente em arquivo
int readTransactions(FILE *file){
    if (!file) return 1; /* ponteiro inválido */

    char line[256];
    while (fgets(line, sizeof line, file)) {
        /* Remove newline, se houver */
        line[strcspn(line, "\n")] = '\0';

        long int sourceCPF = 0, targetCPF = 0;
        double val = 0.0;
        char type[10] = {0};
        int d, m, y, h, min, s;

        int scanned = sscanf(line, "%ld,%ld,%lf,%9[^,],%d/%d/%d %d:%d:%d",
                             &sourceCPF, &targetCPF, &val, type,
                             &d, &m, &y, &h, &min, &s);
        if (scanned != 10) {    
            // linha mal formatada; ignora
            continue;
        }

        transaction *tx = malloc(sizeof *tx);
        if (!tx) {
            // falta de memória
            return -1;
        }

        tx->val = val;
        tx->sourceCPF = sourceCPF;
        tx->targetCPF = targetCPF;
        strcpy(tx->type, type);

        struct tm date = {0};
        date.tm_mday = d;
        date.tm_mon  = m - 1;
        date.tm_year = y - 1900;
        date.tm_hour = h;
        date.tm_min  = min;
        date.tm_sec  = s;
        date.tm_isdst = -1;
        tx->date = date;

        // Adiciona às listas correspondentes
        addTransaction(sourceCPF, tx);
        if (strcmp(type, "transfer") == 0 && targetCPF != 0) {
            addTransaction(targetCPF, tx);
        }
    }

    return 0; // sucesso
}