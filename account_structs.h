/*
 * Header File
 * Written by: John Howe, Alex Szosteck
 *
 * Contains structures that need to be shared between server and client
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

/*
 * Struct: server_response
 * Fields: error_type->Integer representation of an error code. Will be made an enumeration later
 *		   act_type-> Uses the account_type enumeration, denotes whether the account is checking or savings
 *		   initial_balance-> The balance of the account before the transaction was completed
 *		   post_balance-> The balance of the account after the transaction is completed
 * */
typedef struct
{
	//place holder, will enumerate later TODO
	int error_type;
	account_type act_type;
	int initial_balance;
	int post_balance;
}server_response;

/*
 * Struct: transaction
 * Fields: transaction_type-> Uses the action enumeration, denotes the type of transaction
 *		   account_number-> Integer representation of the unique account id to be accessed
 *		   amount-> Integer amount(multiples of 20) that will be transferred, deposited, or withdrawn
 *		   receiver_number-> Integer representation of the unique account id of the account that will 
 *							 be transferred into (only in transfer actions)
 * */
typedef struct 
{
	action transaction_type;
	int account_type;
	int account_number;
	int amount;	
	int receiver_number;
} transaction;

