#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Struct definitions
typedef struct savingsAccount {
    long int CPF;
    double balance;
    double interestRate;
} savingsAccount;

typedef struct checkingAccount {
    long int CPF;
    double balance;
} checkingAccount;

typedef struct savingsAccountTree {
    savingsAccount* account;
    struct savingsAccountTree* left;
    struct savingsAccountTree* right;
    int height;
} savingsAccountTree;

typedef struct checkingAccountTree {
    checkingAccount* account;
    struct checkingAccountTree* left;
    struct checkingAccountTree* right;
    int height;
} checkingAccountTree;

// Global roots
savingsAccountTree* savingsRoot = NULL;
checkingAccountTree* checkingRoot = NULL;

// Height and balance for AVL
static int heightSavings(savingsAccountTree* node) {
    return node ? node->height : 0;
}

static int heightChecking(checkingAccountTree* node) {
    return node ? node->height : 0;
}

static int getBalanceSavings(savingsAccountTree* node) {
    return node ? heightSavings(node->left) - heightSavings(node->right) : 0;
}

static int getBalanceChecking(checkingAccountTree* node) {
    return node ? heightChecking(node->left) - heightChecking(node->right) : 0;
}

// Rotations for AVL
static savingsAccountTree* rightRotateSavings(savingsAccountTree* y) {
    savingsAccountTree* x = y->left;
    savingsAccountTree* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + max(heightSavings(y->left), heightSavings(y->right));
    x->height = 1 + max(heightSavings(x->left), heightSavings(x->right));
    return x;
}

static savingsAccountTree* leftRotateSavings(savingsAccountTree* x) {
    savingsAccountTree* y = x->right;
    savingsAccountTree* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + max(heightSavings(x->left), heightSavings(x->right));
    y->height = 1 + max(heightSavings(y->left), heightSavings(y->right));
    return y;
}

static checkingAccountTree* rightRotateChecking(checkingAccountTree* y) {
    checkingAccountTree* x = y->left;
    checkingAccountTree* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + max(heightChecking(y->left), heightChecking(y->right));
    x->height = 1 + max(heightChecking(x->left), heightChecking(x->right));
    return x;
}

static checkingAccountTree* leftRotateChecking(checkingAccountTree* x) {
    checkingAccountTree* y = x->right;
    checkingAccountTree* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + max(heightChecking(x->left), heightChecking(x->right));
    y->height = 1 + max(heightChecking(y->left), heightChecking(y->right));
    return y;
}

// AVL Insertions
static savingsAccountTree* insertSavings(savingsAccountTree* node, savingsAccount* acc) {
    if (!node) {
        savingsAccountTree* newNode = malloc(sizeof(savingsAccountTree));
        newNode->account = acc;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }
    if (acc->CPF < node->account->CPF)
        node->left = insertSavings(node->left, acc);
    else if (acc->CPF > node->account->CPF)
        node->right = insertSavings(node->right, acc);
    else
        return node;

    node->height = 1 + max(heightSavings(node->left), heightSavings(node->right));
    int balance = getBalanceSavings(node);

    if (balance > 1 && acc->CPF < node->left->account->CPF)
        return rightRotateSavings(node);
    if (balance < -1 && acc->CPF > node->right->account->CPF)
        return leftRotateSavings(node);
    if (balance > 1 && acc->CPF > node->left->account->CPF) {
        node->left = leftRotateSavings(node->left);
        return rightRotateSavings(node);
    }
    if (balance < -1 && acc->CPF < node->right->account->CPF) {
        node->right = rightRotateSavings(node->right);
        return leftRotateSavings(node);
    }
    return node;
}

static checkingAccountTree* insertChecking(checkingAccountTree* node, checkingAccount* acc) {
    if (!node) {
        checkingAccountTree* newNode = malloc(sizeof(checkingAccountTree));
        newNode->account = acc;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }
    if (acc->CPF < node->account->CPF)
        node->left = insertChecking(node->left, acc);
    else if (acc->CPF > node->account->CPF)
        node->right = insertChecking(node->right, acc);
    else
        return node;

    node->height = 1 + max(heightChecking(node->left), heightChecking(node->right));
    int balance = getBalanceChecking(node);

    if (balance > 1 && acc->CPF < node->left->account->CPF)
        return rightRotateChecking(node);
    if (balance < -1 && acc->CPF > node->right->account->CPF)
        return leftRotateChecking(node);
    if (balance > 1 && acc->CPF > node->left->account->CPF) {
        node->left = leftRotateChecking(node->left);
        return rightRotateChecking(node);
    }
    if (balance < -1 && acc->CPF < node->right->account->CPF) {
        node->right = rightRotateChecking(node->right);
        return leftRotateChecking(node);
    }
    return node;
}

// Search
static savingsAccountTree* findSavings(savingsAccountTree* root, long int CPF) {
    if (!root) return NULL;
    if (CPF < root->account->CPF) return findSavings(root->left, CPF);
    if (CPF > root->account->CPF) return findSavings(root->right, CPF);
    return root;
}

static checkingAccountTree* findChecking(checkingAccountTree* root, long int CPF) {
    if (!root) return NULL;
    if (CPF < root->account->CPF) return findChecking(root->left, CPF);
    if (CPF > root->account->CPF) return findChecking(root->right, CPF);
    return root;
}

int createSavingsAccount(long int CPF) {

     /*
     * nome: createSavingsAccount
     *
     * acoplamento:
     *   param 1 — CPF: número de CPF do cliente
     *   ret 1 — int: código de status (0 = sucesso, 1 = conta já existe, -1 = erro de alocação)
     *
     * condições de acoplamento:
     *   - CPF deve ser válido e não pode existir previamente na árvore de contas poupança
     *
     * descrição:
     *   Cria uma nova conta poupança com saldo 0 e juros 0 para o CPF fornecido e insere na árvore AVL.
     *
     * hipóteses:
     *   - A estrutura da árvore AVL está válida
     *
     * restrições:
     *   - A conta não pode ser duplicada
     *   - O CPF deve ser único na árvore
     *
     * pseudo instruções:
     *   // Verifica se o CPF já existe
     *   // Se sim, retorna 1
     *   // Aloca nova struct
     *   // Insere na árvore e retorna 0
     *   // Em caso de falha de alocação, retorna -1
     */

    if (findSavings(savingsRoot, CPF)) return 1;
    savingsAccount* acc = malloc(sizeof(savingsAccount));
    if (!acc) return -1;
    acc->CPF = CPF;
    acc->balance = 0.0;
    acc->interestRate = 0.0;
    savingsRoot = insertSavings(savingsRoot, acc);
    return 0;
}

int createCheckingAccount(long int CPF) {

    /*
     * nome: createCheckingAccount
     *
     * acoplamento:
     *   param 1 — CPF: número de CPF do cliente
     *   ret 1 — int: código de status (0 = sucesso, 1 = conta já existe, -1 = erro de alocação)
     *
     * condições de acoplamento:
     *   - CPF deve ser válido e único na árvore de contas correntes
     *
     * descrição:
     *   Cria uma nova conta corrente com saldo 0 para o CPF especificado e insere na árvore AVL.
     *
     * hipóteses:
     *   - A árvore AVL está funcionando corretamente
     *
     * restrições:
     *   - O CPF não pode estar duplicado
     *
     * pseudo instruções:
     *   // Verifica se o CPF já existe
     *   // Se sim, retorna 1
     *   // Aloca struct da conta corrente
     *   // Insere na árvore e retorna 0
     *   // Se malloc falhar, retorna -1
     */

    if (findChecking(checkingRoot, CPF)) return 1;
    checkingAccount* acc = malloc(sizeof(checkingAccount));
    if (!acc) return -1;
    acc->CPF = CPF;
    acc->balance = 0.0;
    checkingRoot = insertChecking(checkingRoot, acc);
    return 0;
}

int updateCheckingAccountBal(long int CPF, double val) {
    /*
     * nome: updateCheckingAccountBal
     *
     * acoplamento:
     *   param 1 — CPF: número de CPF do cliente
     *   param 2 — val: valor a ser somado ou subtraído do saldo
     *   ret 1 — int: código de status (0 = sucesso, 1 = saldo insuficiente, 2 = CPF não encontrado)
     *
     * condições de acoplamento:
     *   - CPF deve estar presente na árvore de contas correntes
     *   - Saldo não pode ficar negativo
     *
     * descrição:
     *   Atualiza o saldo da conta corrente identificada por CPF, se existir e se a operação for válida.
     *
     * hipóteses:
     *   - A árvore de contas correntes foi corretamente inicializada
     *   - O CPF é válido e único
     *
     * restrições:
     *   - Não permite saldo negativo
     *   - A árvore está acessível pela variável global checkingRoot
     *
     * pseudo instruções:
     *   // Busca o CPF na árvore de contas correntes
     *   // Se não encontrado, retorna 2
     *   // Se a operação deixaria o saldo negativo, retorna 1
     *   // Caso contrário, aplica a variação ao saldo e retorna 0
     */

    checkingAccountTree* node = findChecking(checkingRoot, CPF);
    if (!node) return 2;
    if (node->account->balance + val < 0) return 1;
    node->account->balance += val;
    return 0;
}

int updateSavingsAccountBal(long int CPF, double val) {
    /*
     * nome: updateSavingsAccountBal
     *
     * acoplamento:
     *   param 1 — CPF: número do CPF do cliente
     *   param 2 — val: valor a aplicar no saldo
     *   ret 1 — int: código de status (0 = sucesso, 1 = saldo insuficiente, 2 = CPF não encontrado)
     *
     * condições de acoplamento:
     *   - CPF deve estar presente na árvore de contas poupança
     *   - Saldo não pode ficar negativo após a operação
     *
     * descrição:
     *   Atualiza o saldo da conta poupança identificada por CPF com o valor especificado.
     *
     * hipóteses:
     *   - A árvore AVL de contas poupança está corretamente construída
     *
     * restrições:
     *   - Não permite saldo negativo
     *
     * pseudo instruções:
     *   // Busca o CPF na árvore de contas poupança
     *   // Se não encontrado, retorna 2
     *   // Se a operação deixaria o saldo negativo, retorna 1
     *   // Caso contrário, aplica o valor ao saldo e retorna 0
     */

    savingsAccountTree* node = findSavings(savingsRoot, CPF);
    if (!node) return 2;
    if (node->account->balance + val < 0) return 1;
    node->account->balance += val;
    return 0;
}

int getBalanceByType(long int CPF, double* balance, const char* type) {
    /*
     * nome: getBalanceByType
     *
     * acoplamento:
     *   param 1 — CPF: CPF do cliente
     *   param 2 — balance: ponteiro para double que receberá o saldo
     *   param 3 — type: tipo da conta ("checking" ou "savings")
     *   ret 1 — int: código de status (0 = sucesso, 1 = CPF não encontrado, 2 = ponteiro nulo, 3 = tipo inválido)
     *
     * condições de acoplamento:
     *   - balance != NULL
     *   - type != NULL
     *   - type deve ser "checking" ou "savings"
     *
     * descrição:
     *   Retorna o saldo da conta do tipo especificado associada ao CPF fornecido.
     *
     * hipóteses:
     *   - As árvores de contas estão preenchidas corretamente
     *
     * restrições:
     *   - Tipos diferentes de "checking" e "savings" não são aceitos
     *
     * pseudo instruções:
     *   // Verifica se balance e type são válidos
     *   // Seleciona a árvore correta com base em type
     *   // Busca o CPF na árvore
     *   // Se não encontrado, retorna 1
     *   // Se encontrado, armazena saldo em *balance e retorna 0
     */

    if (!balance || !type) return 2;  // Invalid pointer

    if (strcmp(type, "savings") == 0) {
        savingsAccountTree* sv = findSavings(savingsRoot, CPF);
        if (!sv) return 1;  // Not found
        *balance = sv->account->balance;
        return 0;
    }
    else if (strcmp(type, "checking") == 0) {
        checkingAccountTree* ch = findChecking(checkingRoot, CPF);
        if (!ch) return 1;  // Not found
        *balance = ch->account->balance;
        return 0;
    }

    return 3;  // Invalid type
}

static void saveCheckingTree(checkingAccountTree* node, FILE* file) {
    if (!node) return;
    fprintf(file, "%ld %.2f\n", node->account->CPF, node->account->balance);
    saveCheckingTree(node->left, file);
    saveCheckingTree(node->right, file);
}

static void saveSavingsTree(savingsAccountTree* node, FILE* file) {
    if (!node) return;
    fprintf(file, "%ld %.2f %.2f\n", node->account->CPF, node->account->balance, node->account->interestRate);
    saveSavingsTree(node->left, file);
    saveSavingsTree(node->right, file);
}

int saveCheckingAccounts(FILE* file) {
    /*
     * nome: saveCheckingAccounts
     *
     * acoplamento:
     *   param 1 — file: ponteiro de arquivo em modo escrita
     *   ret 1 — int: código de status (0 = sucesso, 1 = ponteiro inválido)
     *
     * condições de acoplamento:
     *   - file deve estar aberto e não ser NULL
     *
     * descrição:
     *   Percorre a árvore AVL de contas correntes e grava os dados em um arquivo texto.
     *
     * hipóteses:
     *   - A árvore AVL de contas correntes está corretamente montada
     *
     * restrições:
     *   - Os dados são salvos como: CPF saldo
     *
     * pseudo instruções:
     *   // Se file for NULL, retorna 1
     *   // Percorre a árvore AVL e grava: CPF saldo
     *   // Retorna 0 após concluir
     */
    if (!file) return 1;
    saveCheckingTree(checkingRoot, file);
    return 0;
}

int saveSavingsAccounts(FILE* file) {
    /*
     * nome: saveSavingsAccounts
     *
     * acoplamento:
     *   param 1 — file: ponteiro de arquivo em modo escrita
     *   ret 1 — int: código de status (0 = sucesso, 1 = ponteiro inválido)
     *
     * condições de acoplamento:
     *   - file deve estar aberto e não ser NULL
     *
     * descrição:
     *   Percorre a árvore AVL de contas poupança e grava os dados em um arquivo.
     *
     * hipóteses:
     *   - A árvore de contas está completa e acessível
     *
     * restrições:
     *   - Cada linha do arquivo terá: CPF saldo juros
     *
     * pseudo instruções:
     *   // Se file for NULL, retorna 1
     *   // Percorre a árvore AVL e grava os campos das contas poupança
     *   // Retorna 0 ao finalizar
     */
    if (!file) return 1;
    saveSavingsTree(savingsRoot, file);
    return 0;
}

int readCheckingAccounts(FILE* file) {
    /*
     * nome: readCheckingAccounts
     *
     * acoplamento:
     *   param 1 — file: ponteiro de arquivo em modo leitura
     *   ret 1 — int: código de status (0 = sucesso, 1 = ponteiro inválido)
     *
     * condições de acoplamento:
     *   - file deve estar aberto e não ser NULL
     *
     * descrição:
     *   Lê o arquivo contendo contas correntes e reconstroi a árvore AVL com os dados.
     *
     * hipóteses:
     *   - Cada linha do arquivo está no formato: CPF saldo
     *
     * restrições:
     *   - CPF deve ser único
     *   - Saldo deve ser um número válido
     *
     * pseudo instruções:
     *   // Enquanto conseguir ler CPF e saldo
     *   // Cria struct de conta, insere na árvore AVL
     *   // Retorna 0 ao final
     */
    if (!file) return 1;
    long int cpf; double bal;
    while (fscanf(file, "%ld %lf", &cpf, &bal) == 2) {
        checkingAccount* acc = malloc(sizeof(checkingAccount));
        if (!acc) continue;
        acc->CPF = cpf;
        acc->balance = bal;
        checkingRoot = insertChecking(checkingRoot, acc);
    }
    return 0;
}

int readSavingsAccounts(FILE* file) {
    /*
     * nome: readSavingsAccounts
     *
     * acoplamento:
     *   param 1 — file: ponteiro de arquivo em modo leitura
     *   ret 1 — int: código de status (0 = sucesso, 1 = ponteiro inválido)
     *
     * condições de acoplamento:
     *   - file deve estar aberto e não ser NULL
     *
     * descrição:
     *   Lê as contas poupança de um arquivo e insere cada uma na árvore AVL.
     *
     * hipóteses:
     *   - Cada linha do arquivo tem 3 valores válidos: CPF saldo juros
     *
     * restrições:
     *   - CPF deve ser único
     *   - Os valores devem estar no formato correto
     *
     * pseudo instruções:
     *   // Lê CPF, saldo, taxa de juros de cada linha
     *   // Aloca struct, insere na árvore
     *   // Retorna 0 ao fim da leitura
     */
    if (!file) return 1;
    long int cpf; double bal, rate;
    while (fscanf(file, "%ld %lf %lf", &cpf, &bal, &rate) == 3) {
        savingsAccount* acc = malloc(sizeof(savingsAccount));
        if (!acc) continue;
        acc->CPF = cpf;
        acc->balance = bal;
        acc->interestRate = rate;
        savingsRoot = insertSavings(savingsRoot, acc);
    }
    return 0;
}


static int getSavingsAccount(savingsAccountTree* root, long int CPF, savingsAccount* account) {
    savingsAccountTree* node = findSavings(root, CPF);
    if (!node) return 1;
    *account = *(node->account);
    return 0;
}

static int getCheckingAccount(checkingAccountTree* root, long int CPF, checkingAccount* account) {
    checkingAccountTree* node = findChecking(root, CPF);
    if (!node) return 1;
    *account = *(node->account);
    return 0;
}

static int addSavingAccountToTree(savingsAccountTree** root, savingsAccount* accountPtr) {
    if (!accountPtr) return 1;
    if (findSavings(*root, accountPtr->CPF)) return 1;
    *root = insertSavings(*root, accountPtr);
    return 0;
}

static int addCheckingAccountToTree(checkingAccountTree** root, checkingAccount* accountPtr) {
    if (!accountPtr) return 1;
    if (findChecking(*root, accountPtr->CPF)) return 1;
    *root = insertChecking(*root, accountPtr);
    return 0;
}
