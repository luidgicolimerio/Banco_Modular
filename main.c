#include "accountForMain.h"
#include "transactionsForMain.h"
#include "user.h"
#include "AccessControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int startup() {


	FILE* FileForCheckingAccounts = fopen("FileForCheckingAccounts.txt", 'r');
	readCheckingAccounts(FileForCheckingAccounts);
	fclose(FileForCheckingAccounts);

	FILE* FileForSavingsAccounts = fopen("FileForSavingsAccounts.txt", 'r');
	readSavingsAccounts(FileForSavingsAccounts);
	fclose(FileForSavingsAccounts);

	FILE* FileForLogin = fopen("FileForLogins.txt", 'r');
	readLogins(FileForLogin);
	fclose(FileForLogin);

	FILE* FileForUser = fopen("FileForUser.txt", 'r');
	readUserProfiles(FileForUser);
	fclose(FileForUser);

	FILE* FileForTransaction = fopen("FileForTransaction.txt", 'r');
	readTransactions(FileForTransaction);
	fclose(FileForTransaction);

	return 0;
}

static int shutdown() {

	FILE* FileForCheckingAccounts = fopen("FileForCheckingAccounts.txt", 'r');
	saveCheckingAccounts(FileForCheckingAccounts);
	fclose(FileForCheckingAccounts);

	FILE* FileForSavingsAccounts = fopen("FileForSavingsAccounts.txt", 'r');
	saveSavingsAccounts(FileForSavingsAccounts);
	fclose(FileForSavingsAccounts);

	FILE* FileForLogin = fopen("FileForLogins.txt", 'r');
	saveLogins(FileForLogin);
	fclose(FileForLogin);

	FILE* FileForUser = fopen("FileForUser.txt", 'r');
	saveUserProfiles(FileForUser);
	fclose(FileForUser);

	FILE* FileForTransaction = fopen("FileForTransaction.txt", 'r');
	saveTransactions(FileForTransaction);
	fclose(FileForTransaction);

	return 0;
}



int main(void) {

	int loggedBool = 0;

	int userOption = 0;
	/*Fluxo de register ou Login*/
	while (loggedBool == 0) {
		printf("Select option:\n");
		printf("1. Login\n");
		printf("2. Register\n");
		printf("3. Exit");

		scanf("%d", &userOption);

		if (userOption == 3) {
			printf("Exiting...");
			if (shutdown() == 0) {
				exit(0);
			}
		}

		if (userOption == 2) {

			long int CPF = 0;
			char name[100];
			char telNo[100];
			char birth[100];
			char password[100];

			printf("Type in your CPF:\n");
			scanf("%ld", &CPF);

			printf("Type in your name:\n");
			scanf("%s", &name);

			printf("Type in your cellphone number:\n");
			scanf("%s", &telNo);

			printf("Type in your birthdate:\n");
			scanf("%s", &birth);

			printf("Type in your password:\n");
			scanf("%s", &password);

			if (registerUser(name, telNo, birth, CPF, password) == 0) {
				printf("Registered!\n");
			}
		}

		if (userOption == 1) {
			long int CPF = 0;
			char password[100];
			printf("Type in your CPF:\n");
			scanf("%ld", &CPF);

			printf("Type in your password:\n");
			scanf("%s", &password);

			if (login(CPF, password) == 0) {
				printf("Login Success!\n");
				loggedBool = 1;
			}
			else {
				printf("Login Failed.\n");
			}
		}

		if (userOption != 1 && userOption != 2) {
			printf("Choose a valid option!\n");
		}
		userOption = 0;
	}

	/*Fluxo pos login*/
	while (loggedBool = 1) {
		printf("Select option:\n");
		printf("1. Check your balance\n");
		printf("2. Change your password\n");
		printf("3. Change your phone number\n");
		printf("4. Transfer from your savings account.\n");
		printf("5. Transfer from your checking account.\n");
		printf("6. Log out\n");
		printf("7. Log out and exit.\n");
		scanf("%d", &userOption);

		if (userOption == 1) {
			double checkingBal = 0;
			double savingsBal = 0;
			long int CPF = 0;

			getLoginCPF(&CPF);

			getBalanceByType(CPF, &checkingBal, "checking");
			getBalanceByType(CPF, &savingsBal, "savings");

			printf("Balance in checking: &lf\n", checkingBal);
			printf("Balance in savings: &lf\n", savingsBal);

			userOption = 0;
		}

		else if (userOption == 2) {
			char currPass[100];
			char newPass[100];
			long int CPF = 0;
			getLoginCPF(&CPF);

			printf("Insert your current password:\n");
			scanf("%s", &currPass);
			printf("Insert your new password:\n");
			scanf("%s", &newPass);

			if (changePassword(CPF, currPass, newPass) == 0) {
				printf("Password Changed!");
			}
			else {
				printf("Password Change Failed.");
			}
			userOption = 0;
		}

		else if (userOption == 3) {
			char currPass[100];
			char newNum[100];
			long int CPF = 0;
			getLoginCPF(&CPF);

			printf("Insert your current password:\n");
			scanf("%s", &currPass);

			printf("Insert your new number:\n");
			scanf("%s", &newNum);

			if (changeNumber(CPF, currPass, newNum) == 0) {
				printf("Number changed!");
			}
			else {
				printf("Number Change Failed.");
			}
			userOption = 0;
		}

		else if (userOption == 4) {
			double val = 0;
			long int CPF = 0;
			time_t t = time(NULL);
			struct tm currdate = *localtime(&t);
			getLoginCPF(&CPF);

			printf("Insert value to send from savings to checking:\n");
			scanf("%lf", &val);

			if (makeTransactionSavings(val, CPF, CPF, currdate) == 0) {
				printf("Transaction Complete!");
			}
			else {
				printf("Transaction Failed...");
			}
			userOption = 0;
		}

		else if (userOption == 5) {
			double val = 0;
			long int CPF = 0;
			long int targetCPF = 0;
			time_t t = time(NULL);
			struct tm currdate = *localtime(&t);
			getLoginCPF(&CPF);

			printf("Insert value to transfer:\n");
			scanf("%lf", &val);
			printf("What CPF do you want to transfer to?\n");
			scanf("%ld", &targetCPF);

			if (makeTransactionChecking(val, CPF, targetCPF, currdate) == 0) {
				printf("Transaction complete.\n");
			}
			else {
				printf("Transaction failed.\n");
			}
			userOption = 0;
		}

		else if (userOption == 6) {
			if (logout() == 0) {
				printf("Logout successful.\n");
			}
			userOption = 0;
		}

		else if (userOption == 7) {
			logout();
			printf("Logout successful.\n");
			printf("Exiting ...\n");
			if (shutdown() == 0) {
				exit(0);
			}
			userOption = 0;
		}

	}


	return 0;
}