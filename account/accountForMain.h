#include <stdio.h>
int getBalanceByType(long int CPF, double* balance, const char* type);
/*
 * nome: getBalanceByType
 *
 * acoplamento:
 *   param 1 � CPF: CPF do cliente
 *   param 2 � balance: ponteiro para double que recebera o saldo
 *   param 3 � type: tipo da conta ("checking" ou "savings")
 *   ret 1 � int: codigo de status (0 = sucesso, 1 = CPF nao encontrado, 2 = ponteiro nulo, 3 = tipo invalido)
 *
 * condicoes de acoplamento:
 *   - balance != NULL
 *   - type != NULL
 *   - type deve ser "checking" ou "savings"
 *
 * descricao:
 *   Retorna o saldo da conta do tipo especificado associada ao CPF fornecido.
 *
 * hipoteses:
 *   - As arvores de contas estao preenchidas corretamente
 *
 * restricoes:
 *   - Tipos diferentes de "checking" e "savings" nao sao aceitos
 *
 * pseudo instrucoes:
 *   // Verifica se balance e type sao validos
 *   // Seleciona a arvore correta com base em type
 *   // Busca o CPF na arvore
 *   // Se nao encontrado, retorna 1
 *   // Se encontrado, armazena saldo em *balance e retorna 0
 *
 * Assertiva de entrada:
 *   - CPF > 0
 *   - balance != NULL
 *   - type  != NULL e igual a "checking" ou "savings"
 * Assertiva de saída:
 *   - Se retorno 0, *balance conterá o saldo da conta requerida.
 */
int saveSavingsAccounts(FILE* file);
/*
 * nome: saveCheckingAccounts
 *
 * acoplamento:
 *   param 1 � file: ponteiro de arquivo em modo escrita
 *   ret 1 � int: codigo de status (0 = sucesso, 1 = ponteiro invalido)
 *
 * condicoes de acoplamento:
 *   - file deve estar aberto e nao ser NULL
 *
 * descricao:
 *   Percorre a arvore AVL de contas correntes e grava os dados em um arquivo texto.
 *
 * hipoteses:
 *   - A arvore AVL de contas correntes esta corretamente montada
 *
 * restricoes:
 *   - Os dados sao salvos como: CPF saldo
 *
 * pseudo instrucoes:
 *   // Se file for NULL, retorna 1
 *   // Percorre a arvore AVL e grava: CPF saldo
 *   // Retorna 0 apos concluir
 *
 * Assertiva de entrada:
 *   - file != NULL e aberto para escrita
 * Assertiva de saída:
 *   - Se retorno 0, todas as contas correntes foram persistidas.
 */
int saveCheckingAccounts(FILE* file);
/*
 * nome: saveSavingsAccounts
 *
 * acoplamento:
 *   param 1 � file: ponteiro de arquivo em modo escrita
 *   ret 1 � int: codigo de status (0 = sucesso, 1 = ponteiro invalido)
 *
 * condicoes de acoplamento:
 *   - file deve estar aberto e nao ser NULL
 *
 * descricao:
 *   Percorre a arvore AVL de contas poupanca e grava os dados em um arquivo.
 *
 * hipoteses:
 *   - A arvore de contas esta completa e acessivel
 *
 * restricoes:
 *   - Cada linha do arquivo tera: CPF saldo juros
 *
 * pseudo instrucoes:
 *   // Se file for NULL, retorna 1
 *   // Percorre a arvore AVL e grava os campos das contas poupanca
 *   // Retorna 0 ao finalizar
 *
 * Assertiva de entrada:
 *   - file != NULL e aberto para escrita
 * Assertiva de saída:
 *   - Se retorno 0, todas as contas poupança foram persistidas.
 */
int readCheckingAccounts(FILE* file);
/*
 * nome: readCheckingAccounts
 *
 * acoplamento:
 *   param 1 � file: ponteiro de arquivo em modo leitura
 *   ret 1 � int: codigo de status (0 = sucesso, 1 = ponteiro invalido)
 *
 * condicoes de acoplamento:
 *   - file deve estar aberto e nao ser NULL
 *
 * descricao:
 *   Le o arquivo contendo contas correntes e reconstroi a arvore AVL com os dados.
 *
 * hipoteses:
 *   - Cada linha do arquivo esta no formato: CPF saldo
 *
 * restricoes:
 *   - CPF deve ser unico
 *   - Saldo deve ser um numero valido
 *
 * pseudo instrucoes:
 *   // Enquanto conseguir ler CPF e saldo
 *   // Cria struct de conta, insere na arvore AVL
 *   // Retorna 0 ao final
 *
 * Assertiva de entrada:
 *   - file != NULL e aberto para leitura
 * Assertiva de saída:
 *   - Se retorno 0, árvore de contas correntes reconstruída.
 */
int readSavingsAccounts(FILE* file);
/*
 * nome: readSavingsAccounts
 *
 * acoplamento:
 *   param 1 � file: ponteiro de arquivo em modo leitura
 *   ret 1 � int: codigo de status (0 = sucesso, 1 = ponteiro invalido)
 *
 * condicoes de acoplamento:
 *   - file deve estar aberto e nao ser NULL
 *
 * descricao:
 *   Le as contas poupanca de um arquivo e insere cada uma na arvore AVL.
 *
 * hipoteses:
 *   - Cada linha do arquivo tem 3 valores validos: CPF saldo juros
 *
 * restricoes:
 *   - CPF deve ser unico
 *   - Os valores devem estar no formato correto
 *
 * pseudo instrucoes:
 *   // Le CPF, saldo, taxa de juros de cada linha
 *   // Aloca struct, insere na arvore
 *   // Retorna 0 ao fim da leitura
 *
 * Assertiva de entrada:
 *   - file != NULL e aberto para leitura
 * Assertiva de saída:
 *   - Se retorno 0, árvore de contas poupança reconstruída.
 */
int createSavingsAccount(long int CPF);
int createCheckingAccount(long int CPF);
int updateCheckingAccountBal(long int CPF, double val);
int updateSavingsAccountBal(long int CPF, double val);