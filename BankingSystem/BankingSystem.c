#include "Account.h" //Contains the definition of the Account class
//Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define MAX_ACCOUNTS 500 //Defines the maximum number of accounts that can be stored in the program
#define MAX_LINE_LENGTH 255 // Defines the maximum number of characters that can be read from a single line of text in a file

// Accounts array stores all the account objects created by the program
Account accounts[MAX_ACCOUNTS];
int num_accounts = 0; //Number of accounts currently stored in the accounts array
char filepath[] = "../accounts.csv"; //number of accounts currently stored in the accounts array

// To change the color of text in the console output
void color(int bg, int ft) {
	//[0] - black [1] - Dark Blue [2] - Dark Green [3] - Light Blue [4] - Red [5] - Violet
	//[6] - Gold [7] - Murky white [8] - Gray [9] - Indigo [10] - Light Green
	//[11] - Ice Blue //[12] - Pink //[13] - Purple //[14] - Yellow //[15] - WHITE WHITE

	int colors = 16 * bg + ft;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors);
}


//Function that displays a loading screen with a progress bar and waits for user input to continue or exit the program
void loading() {
	//Set console background color to dark blue
	system("COLOR B0");

	// Loop through progress bar values 0 to 100
	for (int i = 0; i <= 100; i++) {
		printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  LOADING %d %c", i, '%');
		Sleep(3); //Wait 3 milliseconds to slow down
		system("cls");  // Clear screen
	}
	// Display loading complete message and wait for user input
	color(11, 2); printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t LOADING COMPLETE");
	color(11, 0); printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t      Press ");
	color(11, 2); printf("[Any Key] ");
	color(11, 0); printf("to Start");
	color(11, 0); printf("\n\n\t\t\t\t\t      Press ");
	color(11, 4); printf("[Esc Key] ");
	color(11, 0); printf("to Exit");

	// If user presses the Esc key, display exit message and terminate the program
	if (_getch() == 27) {
		system("cls");
		color(11, 1); printf("\n\n\n\n\n\n\n\n\t\t\t\t\t THANK YOU FOR USING OUR PROGRAM! \n\n\n\n\n\n\n\n\n\n\n\n\n");
		color(11, 0); exit(0);
	}

}

//Creates a new account by taking input from the user for account information
void create_account() {

	system("cls"); //for clearing screen

	// code to create account goes here

	// add an exit button
	printf("\nPress 'X' to exit or any other key to continue...\n\n");
	//Reads the user input for exiting the function or continuing account creation
	char c = _getch();
	if (c == 'X' || c == 'x') {
		return; // exit the function and return to admin menu
	}

	system("cls");
	//Declare a variables to store account information
	char account_no[13], name[50], gender[7], contact_no[12], address[150];
	double balance;

	// Get user input for account information
	printf("Enter account name: ");
	fgets(name, sizeof(name), stdin); // get account name 
	fgets(name, sizeof(name), stdin); // handle extra newline character
	name[strcspn(name, "\n")] = '\0'; // remove newline character


	printf("Enter gender (Male/Female): ");
	scanf_s("%s", gender, sizeof(gender));

	printf("Enter phone number: ");
	scanf_s("%s", contact_no, sizeof(contact_no));

	printf("Enter address: ");
	fgets(address, sizeof(address), stdin);
	fgets(address, sizeof(address), stdin);
	address[strcspn(address, "\n")] = '\0';

	printf("Enter account balance: ");
	scanf_s("%lf", &balance); // get account balance

	generate_account_no(&account_no); // generate account number

	//Check if the maximum number of accounts has been reached. If yes, an error message is printed and the function is exited
	if (num_accounts == MAX_ACCOUNTS) {
		printf("Error: Unable to create account. Maximum number of accounts reached.\n");
		return;
	}

	// Create a new account
	Account account;
	strcpy_s(account.account_no, sizeof(account.account_no), account_no);
	strcpy_s(account.name, sizeof(account.name), name);
	strcpy_s(account.gender, sizeof(account.gender), gender);
	strcpy_s(account.contact_no, sizeof(account.contact_no), contact_no);
	strcpy_s(account.address, sizeof(account.address), address);
	account.balance = balance;
	accounts[num_accounts] = account; // add the new account to the array
	num_accounts++; // increment the number of accounts

	system("cls");
	printf("\nAccount created successfully!\n\n");
	system("pause"); // wait for user input before returning to the admin menu
}

// Saves the account information to a CSV file
void save_accounts(Account accounts[], int num_accounts, const char* filename) {
	system("cls");

	// Open file for writing
	FILE* fp;
	fopen_s(&fp, filename, "w");

	if (fp == NULL) {
		printf("Error: Unable to open file '%s'\n", filename);
		return;
	}

	// Write header row
	fprintf(fp, "Account No., Name, Gender, Contact No., Address, Balance\n");

	// Write account information to the file
	for (int i = 0; i < num_accounts; i++) {
		fprintf(
			fp,
			"%s,%s,%s,%s,%s,%.2f\n",
			accounts[i].account_no,
			accounts[i].name,
			accounts[i].gender,
			accounts[i].contact_no,
			accounts[i].address,
			accounts[i].balance);
	}

	// Close the file
	fclose(fp);
	system("cls");
	//printf("Data saved to file '%s' successfully!\n", filename);
}

// Reads the account information from a CSV file and loads it into an array of Account structures
int load_accounts(Account accounts[]) {
	// Open the file for reading
	FILE* fp;

	// Declare variables to store the current line and the number of accounts loaded
	char line[MAX_LINE_LENGTH];
	int num_accounts = 0, i = 0;

	errno_t error = fopen_s(&fp, filepath, "r");

	if (error != 0) {
		printf("Error: Unable to open file 'accounts.csv'\n");
		return -1;
	}

	char* next_token = NULL;

	// Loop through each line in the file
	while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
		// If file pointer is at the first line
		// Where the file headers are placed
		// We will skip it.
		if (i == 0) {
			i++;
			continue;

		}

		// Split the line into tokens
		char* next_token;
		char* token = strtok_s(line, ",", &next_token);

		// Store the tokens in the appropriate fields of the Account structure
		error = strcpy_s(accounts[num_accounts].account_no, 13, token);

		token = strtok_s(NULL, ",", &next_token);
		error = strcpy_s(accounts[num_accounts].name, 50, token);

		token = strtok_s(NULL, ",", &next_token);
		error = strcpy_s(accounts[num_accounts].gender, 7, token);

		token = strtok_s(NULL, ",", &next_token);
		error = strcpy_s(accounts[num_accounts].contact_no, 12, token);

		token = strtok_s(NULL, ",", &next_token);
		error = strcpy_s(accounts[num_accounts].address, 150, token);

		token = strtok_s(NULL, ",", &next_token);
		accounts[num_accounts].balance = strtod(token, &next_token);

		// Increment the number of accounts loaded
		num_accounts++;
	}

	// Close the file and return the number of accounts loaded
	fclose(fp);
	return num_accounts;
}

/*  This function lists all the accounts stored in the system
	It displays the account number, account name and balance for each account */
void list_accounts() {
	system("cls");

	printf("Accounts list:\n\n");

	// Loop through all accounts and display their information
	for (int i = 0; i < num_accounts; i++) {
		printf(
			"%d. %s - %s\n",
			i + 1,
			accounts[i].account_no,
			accounts[i].name
		);
		printf("\tBalance: %.2f\n", accounts[i].balance);
	}
	printf("\n\n\nPress any key to return to the admin menu...");
	_getch();

	system("cls");
}

// Deletes an account from the accounts array based on the account number entered by the user
void delete_account() {
	system("cls");

	// code to delete account goes here n 

	// add an exit button
	printf("\nPress 'X' to exit or any other key to continue...");
	char c = _getch();
	if (c == 'X' || c == 'x') {
		return; // exit the function and return to admin menu
	}
	system("cls");

	// Ask the user for the account number to delete
	char account_no[13];
	printf("\nEnter account number: ");
	scanf_s("%s", account_no, sizeof(account_no));

	// Search for the account to delete in the accounts array
	for (int i = 0; i < num_accounts; i++) {
		if (strcmp(accounts[i].account_no, account_no) == 0) {

			// Found the account to delete
			for (int j = i; j < num_accounts; j++) {
				accounts[j] = accounts[j + 1];
			}
			// Decrement the total number of accounts
			num_accounts--;

			system("cls");
			printf("Account Successfully Deleted!\n\n");
			system("pause");

			return;
		}
	}
	system("cls");
	printf("\nAccount not found!\n");
	return;
}

int main() {

	// Changes console background and text color to black (0) and white (1)
	system("COLOR 00");

	// Loop to simulate loading screen
	for (int i = 0; i <= 100; i++) {
		printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  LOADING %d %c", i, '%');
		Sleep(1.2);
		system("cls");
	}

	/* Sets console text color to white on blue background
	   Print current date and time */
	color(0, 1); printf("\n\n\n\n\n\t\t\t\t\t");
	color(0, 1); printf("==========================================\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\tWELCOME TO BANKING SYSTEM\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t||\t\t\t\t\t||\n");
	color(0, 1); printf("\t\t\t\t\t");

	// Displays program credits
	color(0, 1); printf("==========================================\n");
	color(0, 1); printf("\t\t\t\t\t    ||        Provided By:           ||");
	color(0, 1); printf("\n\t\t\t\t\t    ||                               ||");
	color(0, 1); printf("\n\t\t\t\t\t    ||    Hernandez, Justin Ann      ||");
	color(0, 1); printf("\n\t\t\t\t\t    ||    De Guzman, Josh Edilrey    ||");
	color(0, 1); printf("\n\t\t\t\t\t    ||    Bermas, John Joshua        ||");
	color(0, 1); printf("\n\t\t\t\t\t    ||    Arban, Clien               ||");
	color(0, 1); printf("\n\t\t\t\t\t    ||    Manabat, Edvince Ivan      ||");
	color(0, 1); printf("\n\t\t\t\t\t    ||                               ||");
	color(0, 1); printf("\n\t\t\t\t\t");

	// Displays options for user to start or exit program
	color(0, 1); printf("===========================================");
	color(0, 1); printf("\n\n\t\t\t\t\t\t Press ");
	color(0, 2); printf("[Any Key] ");
	color(0, 2); printf("to Start");
	color(0, 1); printf("\n\n\t\t\t\t\t\t Press ");
	color(0, 4); printf("[Esc Key] ");
	color(0, 4); printf("to Exit");
	color(0, 1); printf("\n\n\t\t\t\t\t========================================");

	// Print current date and time
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	// Get the current time in seconds 
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime); // Convert the raw time into a more readable format
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo); // Readable time into a string
	printf("\n\n\t\t\t\t\tCurrent date & time : %s\n", buffer);

	// Wait for the user to press a key, and exit the program if the key is the escape key 
	if (_getch() == 27) {
		system("cls");
		color(0, 1); printf("\n\n\n\n\n\n\n\n\t\t\t\t\t THANK YOU FOR USING OUR PROGRAM! \n\n\n\n\n\n\n\n\n\n\n\n\n");
		color(0, 15); exit(0);
	}

	char choice;

	// Loop until the user chooses to exit
	do {
		num_accounts = load_accounts(accounts);  // load the accounts from the file

		// if the accounts couldn't be loaded, return an error
		if (num_accounts == -1) {
			return 1;
		}

		system("cls");

		// display the main menu
		system("COLOR 09");
		/*color(0, 2);*/printf("------------------------------------------------\n");
		color(0, 7); printf("\t \t \tMAIN MENU\n");
		color(0, 9); printf("------------------------------------------------\n\n\n");
		color(0, 7); printf("\t [A] Admin Menu\n");
		printf("\t [B] User Menu\n");
		printf("\t [C] Exit\n");
		color(0, 8); printf("\n\nEnter your choice: ");
		scanf_s("%c", &choice, sizeof(choice)); // get the user's choice

		system("cls");

		// check the user's choice and take the appropriate action
		if (choice == 'A' || choice == 'a') {
			do {
				scanf_s("%c", &choice, sizeof(choice));
				//system("cls");
				printf("\n\n\n");
				// display the admin menu
				color(0, 9); printf("------------------------------------------------\n");
				color(0, 9); printf("\t \t \tADMIN MENU\n");
				color(0, 9); printf("------------------------------------------------\n\n\n");
				color(0, 7); printf("\t [A] Create account\n");
				printf("\t [B] Delete account\n");
				printf("\t [C] List all accounts\n");
				printf("\t [D] Exit\n");
				color(0, 8); printf("\n\n Enter your choice: ");
				scanf_s("%c", &choice, sizeof(choice));  // get the user's choice

				system("cls");

				switch (choice)
				{
					// create a new account
				case 'a':
				case 'A':
					printf("\n");
					create_account();
					break;

					// delete an existing account
				case 'b':
				case 'B':
					printf("\n");
					delete_account();
					break;

					// list all the accounts
				case 'c':
				case 'C':
					printf("\n");
					list_accounts();
					break;

					// exit the admin menu
				case 'd':
				case 'D':
					printf("\nExiting...\n");
					save_accounts(accounts, num_accounts, filepath);
					break;
				default:
					printf("\nInvalid choice!\n");
					break;
				}

			} while (choice != 'd' && choice != 'D');

		}
		else if (choice == 'b' || choice == 'B') {
			// get the account number from the user
			char account_no[13];
			int account_index = -1;

			printf("\nPress 'X' to exit or any other key to continue...");
			char c = _getch();
			if (c == 'X' || c == 'x') {
				return; // exit the function and return to admin menu
			}

			system("cls");

			printf("\nEnter account number: ");
			scanf_s("%s", account_no, sizeof(account_no));

			// find the index of the account with the given account number
			for (int i = 0; i < num_accounts; i++) {
				if (strcmp(accounts[i].account_no, account_no) != 0)
					continue;

				// if we found the account, clear the screen and display a welcome message
				system("cls");
				printf("Welcome %s!\n", accounts[i].name);
				account_index = i;
				break;
			}

			system("cls");

			while (account_index == -1) {


				printf("\nEnter account number: ");
				scanf_s("%s", account_no, sizeof(account_no));

				// find the index of the account with the given account number
				for (int i = 0; i < num_accounts; i++) {
					if (strcmp(accounts[i].account_no, account_no) != 0)
						continue;

					// if we found the account, clear the screen and display a welcome message
					system("cls");
					printf("Welcome %s!\n", accounts[i].name);
					account_index = i;
					break;
				}

				// if the system didn't find the account, print an error message and exit the program
				if (account_index == -1) {
					system("cls");
					printf("Account not found!\n");

				}
			}

			// display the user menu and handle the user's choice
			do {
				double amount;
				color(0, 9); printf("------------------------------------------------\n");
				color(0, 9); printf("\t \t \tUSER MENU\n");
				color(0, 9); printf("------------------------------------------------\n\n\n");
				color(0, 7); printf("\t [A] Check Balance\n");
				printf("\t [B] Deposit\n");
				printf("\t [C] Withdraw\n");
				printf("\t [D] Account Details\n");
				printf("\t [E] Exit\n");
				color(0, 8); printf("\n\nEnter your choice: ");

				// get user input
				scanf_s("%c", &choice, sizeof(choice));
				scanf_s("%c", &choice, sizeof(choice)); // get the user's choice (the extra scanf_s is to clear the input buffer)

				// handle the user's choice
				switch (choice)
				{
					system("cls");
				case 'a':
				case 'A':
					check_balance(&accounts[account_index]); // call the function to check the account balance
					break;

				case 'b':
				case 'B':
					system("cls");
					printf("\nEnter amount to deposit: ");
					scanf_s("%lf", &amount); // read the amount to deposit
					deposit(&accounts[account_index], amount); // call the function to deposit money
					break;

				case 'c':
				case 'C':
					printf("\nEnter amount to withdraw: ");
					scanf_s("%lf", &amount); // read the amount to withdraw
					withdraw(&accounts[account_index], amount); // call the function to withdraw money
					break;

				case 'd':
				case 'D':
					printf("\n");
					details(&accounts[account_index]); // call the function to display account details
					break;

				case 'e':
				case 'E':
					printf("\nExiting...\n");
					save_accounts(accounts, num_accounts, filepath); // save the account details and exit the program
					break;

				default:
					printf("\nInvalid choice!\n"); // if the choice isn't valid, display an error message
					break;
				}

				// keep looping until the user chooses to exit
			} while (choice != 'e' && choice != 'E');
		}
		// if user selects option C, exit program and save accounts
		else if (choice == 'c' || choice == 'C') {
			printf("\nExiting...\n");
			save_accounts(accounts, num_accounts, filepath);
		}
		else {
			printf("\nInvalid choice!\n");
		}
	} while (choice != 'c' && choice != 'C');

	// return success
	return 0;
}