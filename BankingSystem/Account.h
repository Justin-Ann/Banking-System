// Include necessary header files
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef ACCOUNT_H
#define ACCOUNT_H
#define ACCOUNT_NO_LENGTH 12

// Define a struct for Account
typedef struct Account {
	char account_no[ACCOUNT_NO_LENGTH + 1];
	char name[50];
	char gender[7];
	char contact_no[12];
	char address[150];
	double balance;
} Account;

// Function prototypes
void generate_account_no(char* account_no);
void withdraw(Account* account, double amount);
void deposit(Account* account, double amount);
void check_balance(Account* account);
void details(Account* account);


#endif