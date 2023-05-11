#include "Account.h"

void generate_account_no(char* account_no) {
    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Generate a random 10-digit number
    int rand_num = rand() % 10000000000; // Generate a random 10-digit number

    // Format the account number as a string with leading zeros
    sprintf_s(account_no, ACCOUNT_NO_LENGTH + 1, "%0*d", ACCOUNT_NO_LENGTH, rand_num);
}

// Checks the balance of the given account
void check_balance(Account* account) {
    system("cls");
    printf("Account balance: %.2f\n", account->balance);
}

// Adds the given amount to the balance of the given account
void deposit(Account* account, double amount) {
    account->balance += amount;
    system("cls");
    printf("Success!\n");
}

/* Subtracts the given amount from the balance of the given account,
   provided that the account has enough funds */ 
void withdraw(Account* account, double amount) {
    if (account->balance < amount) {
        system("cls");
        printf("Error: Insufficient funds.\n");
        return;
    }

    account->balance -= amount;
    system("cls");
    printf("Success!\n");
}

// Displays the details of the given account
void details(Account* account) {
    system("cls");
    printf("Account No. %s\n\n", account->account_no);
    printf("Name: %s\n", account->name);
    printf("Gender: %s\n", account->gender);
    printf("Address: %s\n", account->address);
    printf("Contact number: %s\n", account->contact_no);
}