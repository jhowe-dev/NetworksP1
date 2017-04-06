/* tcpserver.c */
/* Programmed by Adarsh Sethi */
/* February 19, 2017 */    

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */
#include "utility.h"/* for structures to be shared between client and server*/

#define STRING_SIZE 1024   

/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 65002
/*
 * Struct: user_account
 * Fields: Account Number->Integer number representing the account identifier
 *		   Balance->Integer number representing the whole-number-amount in the account
 *		   account_type->Enumeration of account types.  Either Checking or savings
 * */
typedef struct
{
	int account_number;
	int balance;
	account_type type;
}user_account;

int main(void) {

   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */
   bool sock_flag = 0;//whether or not the client is sticking around
   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   int transaction[NUM_VALUES_TRANSACTION]; /* expected message */
   int response_message[NUM_VALUES_RESPONSE]; /*message to send back*/
   int transaction_size = NUM_VALUES_TRANSACTION * sizeof(int); /*Size of the message expected*/

   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   /*Create 2 accounts initialized to $0.  #'s 1,2*/

   //checking account
   user_account check_act;
   check_act.account_number=1;
   check_act.balance=0;
   check_act.type=CHECKING;
   //savings account
   user_account save_act;
   save_act.account_number=2;
   save_act.balance=0;
   save_act.type = SAVINGS;
   
   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Server: can't open stream socket");
      exit(1);                                                
   }

   /* initialize server address information */
    
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */ 
   server_port = SERV_TCP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address");
      close(sock_server);
      exit(1);
   }                     

   /* listen for incoming requests from clients */

   if (listen(sock_server, 50) < 0) {    /* 50 is the max number of pending */
      perror("Server: error on listen"); /* requests that will be queued */
      close(sock_server);
      exit(1);
   }
   printf("I am here to listen ... on port %hu\n\n", server_port);
  
   client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */

   for (;;) {
	  if(sock_flag == 0){
      sock_connection = accept(sock_server, (struct sockaddr *) &client_addr, 
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
	  }//if allows for persistent connection if already connected to client
      if (sock_connection < 0) {
         perror("Server: accept() error\n"); 
         close(sock_server);
         exit(1);
      }
 
      /* receive the message */
	  bytes_recd = recv(sock_connection, transaction, transaction_size, 0);
	  if(bytes_recd == 1)
	  {
		print_separator();
		printf("Client is disconnecting!\n");
		sock_flag = 0;
		close(sock_connection);
		printf("Goodbye!\n");
		print_separator();
		continue;
	  } 
	  else{
		sock_flag = 1;
	  }
	  /*prepare values to go in server response*/

	  //Which error has been encountered (0 being no error)
	  int error_code = 0;
	  //Account type that was accessed
	  int response_account_type=SAVINGS;
	  //Initial balance before any transaction is completed
	  int response_initial_balance = 0;
	  //Post balance -> balance after the transaction is complete (with or without error)
	  int response_post_balance = 0;
      
	  if (bytes_recd > 0)
		{

		 print_transaction(transaction);			
       printf("Transaction received of size: %d bytes.\n", transaction_size);
		 print_separator();
		 /*Pull out relevant info from client message*/
		 //what action to perform
		 int action_type = transaction[TRANSACTION_TYPE_INDEX];
		 //account number to perform on
		 int account_number = transaction[T_ACCOUNT_NUMBER_INDEX];
		 //receiver in the event of transfer
		 int receiver_account_number = transaction[T_RECEIVER_NUMBER_INDEX];
		 //amount to deposit/withdraw/transfer
		 int amount = transaction[T_AMOUNT_INDEX];
		 
		 //Decide action based on action type!
		 switch (action_type)
		 {
			//Balance Inquiry
			case 0:
				printf("Balance Inquiry received for account: %d\n",
						account_number);
				if(account_number == 1)
				{
					response_account_type = CHECKING;
					printf("Accessing Account 1\n");
					//for readability in stdout
					printf("Account 1 has $%d remaining in their account.\n", check_act.balance);
					//for readability in stdout
					print_separator();
				}//if accessing account 1
				else if(account_number == 2)
				{
					response_account_type = SAVINGS;
					
					printf("Accessing Account 2 \n");
					printf("Account 2 has $%d remaining in their account.\n", save_act.balance);
					print_separator();
				}//else if accessing account 2
				else
				{
					//No such account error
					error_code = ERR_ACCOUNT_NONEXIST;
				}//else save error code, no such account
				break;
			//Deposit
			case 1:
				printf("Deposit request received for Account: %d\n",
						account_number);
				if(account_number == 1)
				{
					//save values for server response
					response_account_type = CHECKING;
					//initial value
					response_initial_balance = check_act.balance;
					
					printf("Accessing Account 1\n");
					print_separator();
					
					check_act.balance += amount;//Add the amount specified in the transaction to the account
				    //after depositing, save off the post balance for response	
					response_post_balance = check_act.balance;
					//notify user
					printf("Remaining Balance: $%d\n", check_act.balance);
					print_separator();
				}//if accessing account 1
				
				else if(account_number == 2)
				{
					//save off values for server response
					response_account_type = SAVINGS;
					//initial value
					response_initial_balance = save_act.balance;
					printf("Accessing Account 2\n");
					print_separator();
					save_act.balance += amount;//Add the amount specified in the transaction to the account
					//post value
					response_post_balance = save_act.balance;
					//notfiy user
					printf("Remaining Balance: $%d\n", save_act.balance);
					print_separator();
				}//else if accessing account 2
				
				else
				{
					//no such account error
					error_code = ERR_ACCOUNT_NONEXIST;
				}//else save error code, no such account
				
				break;
			//Withdraw
			case 2:
				printf("Withdraw request received for account: %d\n",
					  account_number);
				if(account_number == 1)
				{
					//save off response values
					response_account_type = CHECKING;
					//initial value
					response_initial_balance = check_act.balance;
					printf("Attempting to withdraw from account 1\n");
					if(check_act.balance - amount < 0)
					{
						//insufficient funds error
						error_code = ERR_INSUFFICIENT_FUNDS;
					}//if insufficient funds error 3
					else
					{
						printf("Withdrawing $%d from Account %d.\n", amount, account_number);
						check_act.balance -= amount;
						//save post value
						response_post_balance = check_act.balance;
						//notfiy user
						printf("New balance is $%d\n", check_act.balance);
					}//else withdraw amount	
				}//if accessing account 1

				else if(account_number == 2)
				{
					//save off response values
					response_account_type = SAVINGS;
					//set error, no withdraw from savings
					error_code = ERR_WITHDRAW_SAVINGS;
				}
				else
				{
					error_code = ERR_ACCOUNT_NONEXIST;
				}//else Error! no such account
				
				break;
			//Transfer
			case 3:
				printf("Transfer request received from Account %d to Account %d\n",
					  account_number, receiver_account_number);	
				
				if(account_number == 1)
				{
					//save response values
					//only worry about account receiving money
					response_account_type = SAVINGS;
					response_initial_balance = save_act.balance;
					if(check_act.balance - amount < 0)
					{
						error_code = ERR_INSUFFICIENT_FUNDS;
					}//if insufficient funds
					else if(receiver_account_number != 2)
					{
						error_code = ERR_RECEIVER_NONEXIST;
					}//else if receiver does not exist
					else
					{
						printf("Transferring $%d to Account %d\n", amount, receiver_account_number);
						//take money from one account
						check_act.balance -= amount;
						//add money to other account
						save_act.balance += amount;
						//value after transferring
						printf("New balances are Sender: $%d, Receiver: $%d\n", check_act.balance, save_act.balance);
						response_post_balance = save_act.balance;
					}//else transfer funds!
				}//if transfer from account 1
				else if(account_number == 2){
				//save response values
				//only worry about account receiving money
					response_account_type = CHECKING;
					response_initial_balance = check_act.balance;
					if(save_act.balance - amount < 0)
					{
						error_code = ERR_INSUFFICIENT_FUNDS;
					}//if insufficient funds
					else if(receiver_account_number != 1)
					{
						error_code = ERR_RECEIVER_NONEXIST;
					}//else if receiver does not exist
					else
					{
						printf("Transferring $%d to Account %d\n", amount, receiver_account_number);
						//take money from one account
						save_act.balance -= amount;
						//add money to other account
						check_act.balance += amount;
						//value after transferring
						response_post_balance = check_act.balance;
						printf("New balances are Sender:$%d , Receiver:$%d\n", save_act.balance, check_act.balance);
					}//else transfer funds!
				}//else if transfer from account 2
				else
				{
					error_code = ERR_ACCOUNT_NONEXIST;
				}
				break;
			//Error if you got this far..
			default:
				error_code = ERR_INVALID_TRANSACTION;
				break;
		 }

			/* prepare the message to send */
			msg_len = NUM_VALUES_RESPONSE*sizeof(int);
			/*consturct message*/
			//add error code
			response_message[ERROR_TYPE_INDEX] = error_code;
			//add account_type
			response_message[R_ACCOUNT_TYPE_INDEX] = response_account_type;
			//add initial balance
			response_message[R_INITIAL_BALANCE_INDEX] = response_initial_balance;
			//add post balance
			response_message[R_POST_BALANCE_INDEX] = response_post_balance;					
	
			/* send message */
			bytes_sent = send(sock_connection, response_message, msg_len, 0);
			printf("Sent response to client of size %d bytes.\n", bytes_sent);
			print_response(response_message);
      }
   } 
}
