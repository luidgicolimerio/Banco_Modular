#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <stdio.h>   /* FILE */
#include <time.h>    /* struct tm */

/*
 * Funções públicas do módulo transactions
 */
/*
 * nome: makeTransactionSavings
 *
 * acoplamento:
 *   param1 double value         – valor a ser movimentado
 *   param2 long int sourceCPF   – CPF de origem (normalmente o próprio cliente)
 *   param3 long int targetCPF   – CPF de destino (0 se não houver)
 *   param4 int   depositOrWithdrawal – 0 = depósito, 1 = saque, 2 = transferência interna
 *   param5 struct tm date       – data/hora da operação
 *   ret   int                   – 0 = sucesso, 1 = valor inválido, 3 = CPF sem transações, 4 = tipo inválido
 *
 * descrição:
 *   Registra uma transação na conta poupança conforme o tipo indicado.
 *
 * pseudo instruções:
 *   // valida value (>0)
 *   // valida tipo
 *   // grava transação em log interno
 *
 * Assertiva de entrada:
 *   - value > 0
 *   - sourceCPF > 0
 *   - targetCPF >= 0
 *   - depositOrWithdrawal deve ser 0, 1 ou 2
 *   - date deve ser uma data/hora válida
 * Assertiva de saída:
 *   - Se retorno 0, a transação foi gravada com sucesso no log interno.
 */
int makeTransactionSavings(double value, long int sourceCPF, long int targetCPF,
                           int depositOrWithdrawal, struct tm date);

/*
 * nome: makeTransactionChecking
 * acoplamento:
 *   param1 double value
 *   param2 long int sourceCPF
 *   param3 long int targetCPF
 *   param4 struct tm date
 *   ret int (0 = sucesso, 1 = valor inválido, 2 = saldo insuficiente, 3 = CPF não encontrado)
 *
 * Assertiva de entrada:
 *   - value > 0
 *   - sourceCPF > 0
 *   - targetCPF >= 0
 *   - date válida
 * Assertiva de saída:
 *   - Se retorno 0, operação registrada e saldo atualizado.
 */
int makeTransactionChecking(double value, long int sourceCPF, long int targetCPF,
                            struct tm date);

/*
 * nome: showTransactions
 * acoplamento:
 *   param1 long int CPF – CPF cujo extrato será mostrado
 *   ret int (0 = sucesso, 3 = CPF não possui transações)
 *
 * Assertiva de entrada:
 *   - CPF > 0
 * Assertiva de saída:
 *   - Se retorno 0, todas as transações do CPF foram listadas.
 */
int showTransactions(long int CPF);

/*
 * nome: saveTransactions
 * acoplamento:
 *   param1 FILE* file – arquivo aberto em modo texto para escrita
 *   ret int (0 = sucesso, 1 = ponteiro inválido)
 *
 * Assertiva de entrada:
 *   - file != NULL (aberto para escrita)
 * Assertiva de saída:
 *   - Se retorno 0, transações persistidas no arquivo.
 */
int saveTransactions(FILE *file);

/*
 * nome: readTransactions
 * acoplamento:
 *   param1 FILE* file – arquivo aberto para leitura
 *   ret int (0 = sucesso, 1 = ponteiro inválido)
 *
 * Assertiva de entrada:
 *   - file != NULL (aberto para leitura)
 * Assertiva de saída:
 *   - Se retorno 0, transações carregadas para memória.
 */
int readTransactions(FILE *file);

#endif /* TRANSACTIONS_H */
