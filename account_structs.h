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



