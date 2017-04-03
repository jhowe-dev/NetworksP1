typedef enum {false, true} bool;

typedef enum 
{
	CHECK,
	DEPOSIT,
	WITHDRAW,
	TRANSFER	
} action;

typedef enum
{
	CHECKING,
	SAVINGS
} account_type;

/*
 * Struct used to store the response from the server
 * */
typedef struct
{
	//place holder, will enumerate later TODO
	int error_type;
	account_type act_type;
	int initial_balance;
	int post_balance;
}server_response;

typedef struct 
{
	action transaction_type;
	int account_type;
	int account_number;
	int amount;	
	int receiver_number;
} transaction;

