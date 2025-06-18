#include "accountForMain.h"
#include "transactionsForMain.h"
#include "user.h"
#include "AcessControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int startup() {
    FILE* FileForCheckingAccounts = fopen("FileForCheckingAccounts.txt", "r");
    if (FileForCheckingAccounts) {
        readCheckingAccounts(FileForCheckingAccounts);
        fclose(FileForCheckingAccounts);
    }

    FILE* FileForSavingsAccounts = fopen("FileForSavingsAccounts.txt", "r");
    if (FileForSavingsAccounts) {
        readSavingsAccounts(FileForSavingsAccounts);
        fclose(FileForSavingsAccounts);
    }

    FILE* FileForLogin = fopen("FileForLogins.txt", "r");
    if (FileForLogin) {
        readLogins(FileForLogin, &loginRoot);
        fclose(FileForLogin);
    }

    FILE* FileForUser = fopen("FileForUser.txt", "rb");
    if (FileForUser) {
        readUserProfiles(FileForUser);
        fclose(FileForUser);
    }

    FILE* FileForTransaction = fopen("FileForTransaction.txt", "r");
    if (FileForTransaction) {
        readTransactions(FileForTransaction);
        fclose(FileForTransaction);
    }

    return 0;
}

static int shutdown() {
    FILE* FileForCheckingAccounts = fopen("FileForCheckingAccounts.txt", "w");
    if (FileForCheckingAccounts) {
        saveCheckingAccounts(FileForCheckingAccounts);
        fclose(FileForCheckingAccounts);
    }

    FILE* FileForSavingsAccounts = fopen("FileForSavingsAccounts.txt", "w");
    if (FileForSavingsAccounts) {
        saveSavingsAccounts(FileForSavingsAccounts);
        fclose(FileForSavingsAccounts);
    }

    FILE* FileForLogin = fopen("FileForLogins.txt", "w");
    if (FileForLogin) {
        saveLogins(FileForLogin, loginRoot);
        fclose(FileForLogin);
    }

    FILE* FileForUser = fopen("FileForUser.txt", "wb");
    if (FileForUser) {
        saveUserProfiles(FileForUser);
        fclose(FileForUser);
    }

    FILE* FileForTransaction = fopen("FileForTransaction.txt", "w");
    if (FileForTransaction) {
        saveTransactions(FileForTransaction);
        fclose(FileForTransaction);
    }

    return 0;
}

int main(void) {
    startup();

    int loggedIn = 0;
    int userOption = 0;

    while (1) {
        while (!loggedIn) {
            printf("\nSelect option:\n");
            printf("1. Login\n");
            printf("2. Register\n");
            printf("3. Exit\n");
            printf("Option: ");
            scanf("%d", &userOption);

            if (userOption == 3) {
                printf("Exiting...\n");
                shutdown();
                return 0;
            } else if (userOption == 2) {
                long int CPF;
                char name[100], telNo[100], birth[100], password[100];

                printf("Enter CPF: ");
                scanf("%ld", &CPF);
                printf("Enter name: ");
                scanf("%s", name);
                printf("Enter cellphone: ");
                scanf("%s", telNo);
                printf("Enter birthdate: ");
                scanf("%s", birth);
                printf("Enter password: ");
                scanf("%s", password);

                if (registerUser(name, telNo, birth, CPF, password) == 0) {
                    createSavingsAccount(CPF);
                    createCheckingAccount(CPF);
                    printf("Registration successful!\n");
                } else {
                    printf("Registration failed.\n");
                }
            } else if (userOption == 1) {
                long int CPF;
                char password[100];

                printf("Enter CPF: ");
                scanf("%ld", &CPF);
                printf("Enter password: ");
                scanf("%s", password);

                if (login(CPF, password) == 0) {
                    printf("Login successful!\n");
                    loggedIn = 1;
                } else {
                    printf("Login failed.\n");
                }
            } else {
                printf("Invalid option.\n");
            }
        }

        while (loggedIn) {
            printf("\nPost-login Menu:\n");
            printf("1. Check balance\n");
            printf("2. Change password\n");
            printf("3. Change phone number\n");
            printf("4. Transfer from savings to checking\n");
            printf("5. Transfer to another user (checking)\n");
            printf("6. Deposit\n");
            printf("7. Withdraw\n");
            printf("8. Log out\n");
            printf("9. Log out and exit\n");
            printf("Option: ");
            scanf("%d", &userOption);

            long int CPF;
            getLoginCPF(&CPF);
            time_t t = time(NULL);
            struct tm currdate = *localtime(&t);

            if (userOption == 1) {
                double checkingBal = 0, savingsBal = 0;
                getBalanceByType(CPF, &checkingBal, "checking");
                getBalanceByType(CPF, &savingsBal, "savings");
                printf("Checking balance: %.2lf\n", checkingBal);
                printf("Savings balance: %.2lf\n", savingsBal);
            } else if (userOption == 2) {
                char oldPass[100], newPass[100];
                printf("Current password: ");
                scanf("%s", oldPass);
                printf("New password: ");
                scanf("%s", newPass);
                if (changePassword(CPF, oldPass, newPass) == 0)
                    printf("Password changed successfully.\n");
                else
                    printf("Failed to change password.\n");
            } else if (userOption == 3) {
                char currPass[100], newTel[100];
                printf("Current password: ");
                scanf("%s", currPass);
                printf("New phone number: ");
                scanf("%s", newTel);
                if (changeNumber(CPF, currPass, newTel) == 0)
                    printf("Phone number updated.\n");
                else
                    printf("Failed to update phone number.\n");
            } else if (userOption == 4) {
                double val;
                printf("Value to transfer (savings -> checking): ");
                scanf("%lf", &val);
                if (makeTransactionSavings(val, CPF, CPF, 0, currdate) == 0) {
                    updateSavingsAccountBal(CPF, -val);
                    updateCheckingAccountBal(CPF, val);
                    printf("Transfer successful.\n");
                } else {
                    printf("Transfer failed.\n");
                }
            } else if (userOption == 5) {
                double val;
                long int targetCPF;
                printf("Target CPF: ");
                scanf("%ld", &targetCPF);
                printf("Value to transfer: ");
                scanf("%lf", &val);
                if (makeTransactionChecking(val, CPF, targetCPF, currdate) == 0) {
                    updateCheckingAccountBal(CPF, -val);
                    createCheckingAccount(targetCPF); // garante conta destino
                    updateCheckingAccountBal(targetCPF, val);
                    printf("Transfer successful.\n");
                } else {
                    printf("Transfer failed.\n");
                }
            } else if (userOption == 6) {
    int type;
    double amount;
    printf("1. Deposit to checking\n2. Deposit to savings\nOption: ");
    scanf("%d", &type);
    printf("Amount: ");
    scanf("%lf", &amount);

    int result = -1;
    if (type == 1) {
        result = makeTransactionChecking(amount, CPF, CPF, currdate);
    } else if (type == 2) {
        result = makeTransactionSavings(amount, CPF, CPF, 0, currdate);
    } else {
        printf("Invalid type.\n");
    }

    if (result == 0) {
        printf("Deposit successful.\n");

        double bal = 0;
        const char* accountType = (type == 1) ? "checking" : "savings";
        if (getBalanceByType(CPF, &bal, accountType) == 0) {
            printf("Updated balance (%s): %.2lf\n", accountType, bal);
        } else {
            printf("Error retrieving updated balance.\n");
        }

    } else {
        printf("Deposit failed.\n");
    }

            } else if (userOption == 7) {
                int type;
                double amount;
                printf("1. Withdraw from checking\n2. Withdraw from savings\nOption: ");
                scanf("%d", &type);
                printf("Amount: ");
                scanf("%lf", &amount);

                if (type == 1) {
                    if (makeTransactionChecking(-amount, CPF, CPF, currdate) == 0)
                        printf("Withdrawal successful.\n");
                    else
                        printf("Withdrawal failed.\n");
                } else if (type == 2) {
                    if (makeTransactionSavings(amount, CPF, CPF, 1, currdate) == 0)
                        printf("Withdrawal successful.\n");
                    else
                        printf("Withdrawal failed.\n");
                } else {
                    printf("Invalid option.\n");
                }
            } else if (userOption == 8) {
                if (logout(CPF) == 0) {
                    printf("Logged out.\n");
                    loggedIn = 0;
                }
            } else if (userOption == 9) {
                logout(CPF);
                printf("Logging out and exiting...\n");
                shutdown();
                return 0;
            } else {
                printf("Invalid option.\n");
            }
        }
    }

    return 0;
}
