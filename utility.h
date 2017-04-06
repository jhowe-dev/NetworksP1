#include <stdio.h>          /* for standard I/O functions */
/*
 * Header File
 * Written by: John Howe, Alex Szostek
 *
 * Contains information about structures that need to be shared between server and client and utility functions
 * */

/*
 * Enumeration: bool
 * Values: false, true
 * Use: Used to represent boolean values
 * */
typedef enum {false, true} bool;

/*
 * Enumeration: action
 * Values: Check, Deposit, Withdraw, Transfer
 * Use: Used to represent the possible actions that can be taken by a client
 * */
typedef enum 
{
	CHECK,
	DEPOSIT,
	WITHDRAW,
	TRANSFER	
} action;

/*
 * Enumeration: account_type
 * Values: Checking, Savings
 * Use: Used to represent possible account types on the server
 * */
typedef enum
{
	CHECKING,
	SAVINGS
} account_type;

//Utility Functions

void print_separator()
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

/*
 * Int Array: transaction
 * Fields: 
 *			transaction_type-> Uses the action enumeration, denotes the type of transaction
 *		   account_number-> Integer representation of the unique account id to be accessed
 *		   amount-> Integer amount(multiples of 20) that will be transferred, deposited, or withdrawn
 *		   receiver_number-> Integer representation of the unique account id of the account that will 
 *							 be transferred into (only in transfer actions)
 * */
#define NUM_VALUES_TRANSACTION 4
#define TRANSACTION_TYPE_INDEX 0
#define T_ACCOUNT_NUMBER_INDEX 1
#define T_AMOUNT_INDEX 2
#define T_RECEIVER_NUMBER_INDEX 3

void print_transaction(int transaction[NUM_VALUES_TRANSACTION])
{
	printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("Transaction Type : %d\n", transaction[TRANSACTION_TYPE_INDEX]);	
	printf("Account Number : %d\n", transaction[T_ACCOUNT_NUMBER_INDEX]);	
	printf("Amount : %d\n", transaction[T_AMOUNT_INDEX]);	
	printf("Receiver Account Number : %d\n", transaction[T_RECEIVER_NUMBER_INDEX]);	
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}



/*
 * Int Array: server_response
 * Fields: 
 *			error_type->Integer representation of an error code. Will be made an enumeration later
 *		   act_type-> Uses the account_type enumeration, denotes whether the account is checking or savings
 *		   initial_balance-> The balance of the account before the transaction was completed
 *		   post_balance-> The balance of the account after the transaction is completed
 * */
#define NUM_VALUES_RESPONSE 4
#define ERROR_TYPE_INDEX 0
#define R_ACCOUNT_TYPE_INDEX 1
#define R_INITIAL_BALANCE_INDEX 2
#define R_POST_BALANCE_INDEX 3

//used to print the response sent by the server
void print_response(int server_response[NUM_VALUES_RESPONSE])
{
	print_separator();
	printf("Error code: %d \n", server_response[ERROR_TYPE_INDEX]);
	printf("Account Type: %d \n", server_response[R_ACCOUNT_TYPE_INDEX]);
	printf("Initial Balance: %d \n", server_response[R_INITIAL_BALANCE_INDEX]);
	printf("Post Transaction Balance: %d \n", server_response[R_POST_BALANCE_INDEX]);
	print_separator();
}


/*Error codes:
 * 1: Account does not exist -> specified number is not an existing account
 * 2: Invalid request -> Specified request is not one of 0(inquiry) 1(deposit) 2(withdraw) 3(transfer)
 * 3: Insufficient funds -> During a withdrawal or transfer, the account losing money does not have enough to complete transaction
 * 4: Receiver doesn't exist -> Account number specified for the receiver of a transfer does not exist
 * */
