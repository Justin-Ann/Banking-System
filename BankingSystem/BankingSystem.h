#include "Account.h" /// To access the Account struct and related functions

#ifndef BANKING_SYSTEM // Header guard to prevent multiple includes of this file
#define BANKING_SYSTEM

// Function prototypes for functions defined in BankingSystem.c
void save_accounts(Account accounts[], int num_accounts, const char* filename);
int load_accounts(Account accounts[]);
void create_account();
void delete_account();
void list_accounts();

#endif // !BANKING_SYSTEM