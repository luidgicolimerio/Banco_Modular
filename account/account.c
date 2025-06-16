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


    if (findChecking(checkingRoot, CPF)) return 1;
    checkingAccount* acc = malloc(sizeof(checkingAccount));
    if (!acc) return -1;
    acc->CPF = CPF;
    acc->balance = 0.0;
    checkingRoot = insertChecking(checkingRoot, acc);
    return 0;
}

int updateCheckingAccountBal(long int CPF, double val) {
   
    checkingAccountTree* node = findChecking(checkingRoot, CPF);
    if (!node) return 2;
    if (node->account->balance + val < 0) return 1;
    node->account->balance += val;
    return 0;
}

int updateSavingsAccountBal(long int CPF, double val) {

    savingsAccountTree* node = findSavings(savingsRoot, CPF);
    if (!node) return 2;
    if (node->account->balance + val < 0) return 1;
    node->account->balance += val;
    return 0;
}

int getBalanceByType(long int CPF, double* balance, const char* type) {

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

    if (!file) return 1;
    saveCheckingTree(checkingRoot, file);
    return 0;
}

int saveSavingsAccounts(FILE* file) {

    if (!file) return 1;
    saveSavingsTree(savingsRoot, file);
    return 0;
}

int readCheckingAccounts(FILE* file) {

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
