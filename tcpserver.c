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

#define SERV_TCP_PORT 65001
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

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
	int transaction[NUM_VALUES_TRANSACTION]; /* expected message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   /*Create 2 accounts initialized to $0.  #'s 1,2*/
   user_account check_act;
   check_act.account_number=1;
   check_act.balance=0;
   check_act.type=CHECKING;
   
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

      sock_connection = accept(sock_server, (struct sockaddr *) &client_addr, 
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
      if (sock_connection < 0) {
         perror("Server: accept() error\n"); 
         close(sock_server);
         exit(1);
      }
 
      /* receive the message */
	  int transaction_size = (NUM_VALUES_TRANSACTION * sizeof(int)) + 1;
      bytes_recd = recv(sock_connection, transaction, transaction_size, 0);
	  int error_code = 0;
      if (bytes_recd > 0)
		{

         printf("Transaction Received of size: %d\n", transaction_size);
		 print_transaction(transaction);			
		 
		 
		 /*TODO Implement Account Logic*/
		 int action_type = transaction[TRANSACTION_TYPE_INDEX];
		 int account_number = transaction[T_ACCOUNT_NUMBER_INDEX];
		 int rec_number = transaction[T_RECEIVER_NUMBER_INDEX];
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
					printf("Accessing Account 1\n");
					print_separator();
					printf("You have: %d remaining in your account\n", check_act.balance);
					print_separator();
				}//if accessing account 1
				else if(account_number == 2)
				{
					printf("Accessing Account 2 \n");
					print_separator();
					printf("You have: %d remaining in your account\n", save_act.balance);
					print_separator();
				}//else if accessing account 2
				else
				{
					printf("Error code 1! Account :%d for Balance inquiry does not exist.\n", account_number);		
					error_code = 1;
				}//else save error code, no such account
				break;
			//Deposit
			case 1:
				printf("Deposit request received for account: %d\n",
						account_number);
				if(account_number == 1)
				{
					printf("Accessing Account 1\n");
					print_separator();
					
					check_act.balance += amount;//Add the amount specified in the transaction to the account
					
					printf("You now have: %d remaining in your account\n", check_act.balance);
					print_separator();
				}//if accessing account 1
				
				else if(account_number == 2)
				{
					printf("Accessing Account 2 \n");
					print_separator();
					save_act.balance += amount;//Add the amount specified in the transaction to the account
					printf("You have: %d remaining in your account\n", save_act.balance);
					print_separator();
				}//else if accessing account 2
				
				else
				{
					printf("Error code 1! Account :%d for deposit request does not exist.\n", account_number);		
					error_code = 1;
				}//else save error code, no such account
				
				break;
			//Withdraw
			case 2:
				printf("Withdraw request received for account: %d\n",
					  account_number);
				if(account_number == 1)
				{
					printf("Attempting to withdraw from account 1\n");
					if(check_act.balance - amount < 0)
					{
						printf("Error code 3! I'm sorry, there are not enough funds in account %d to complete the withdrawal of %d\n",
								account_number, amount);
						error_code = 3;
					}//if insufficient funds error 3
					else
					{
						printf("Withdrawing %d from account %d", amount, account_number);
						check_act.balance -= amount;
						printf("New balance is %d", check_act.balance);
					}//else withdraw amount	
				}//if accessing account 1

				else if(account_number == 2)
				{
					if(save_act.balance - amount < 0)
					{
						printf("Error code 3! I'm sorry, there are not enough funds in account %d to complete the withdrawal of %d\n",
								account_number, amount);
						error_code = 3;
					}
					else
					{
						printf("Withdrawing %d from account %d", amount, account_number);
						save_act.balance -= amount;
						printf("New balance is %d", save_act.balance);
					}
					printf("Attempting to withdraw from account 2");
				}//else if accessing account 2	
				
				else{
					printf("Error code 1! Account :%d for withdraw request does not exist.\n", account_number);		
					error_code = 1;
				}//else Error! no such account
				
				break;
			//Transfer
			case 3:
				printf("Transfer request received from account: %d to account %d\n",
					  account_number, rec_number);	
				break;
			//Error if you got this far..
			default:
				printf("Error code 2! I didn't understand your request, please try again with a valid request.\n");
				error_code = 2;
				break;
		 }

			/* prepare the message to send */
			msg_len = bytes_recd;

			int j = 1;
			/* send message */
			bytes_sent = send(sock_connection, &j, sizeof(j), 0);
      }

      /* close the socket */
      close(sock_connection);
   } 
}
