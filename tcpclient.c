/* tcp_ client.c */ 
/* Programmed by Adarsh Sethi */
/* February 19, 2017 */     

#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, connect, send, and recv */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */
#include "utility.h"/* for structures shared between client and server*/
#define STRING_SIZE 1024

int main(void) {

   int sock_client;  /* Socket used by client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   int transaction[NUM_VALUES_TRANSACTION];  /* send message */
   int server_response[NUM_VALUES_RESPONSE]; /* receive message */
   unsigned int msg_len;  /* length of message */                      
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
  
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Client: can't open stream socket");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information 
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */
printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   printf("Enter port number for server: ");
   scanf("%hu", &server_port);

   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(server_port);

    /* connect to the server */
 		
   if (connect(sock_client, (struct sockaddr *) &server_addr, 
                                    sizeof (server_addr)) < 0) {
      perror("Client: can't connect to server");
      close(sock_client);
      exit(1);
   }

   /* user interface */
	print_separator();
   printf("Welcome to Big Bill's Big Bank!\n");
	print_separator();
	bool repeat = true;
	while(repeat)
	{
		printf("Please enter in your account number.\n");	
		printf("> ");
		scanf("%d", &transaction[T_ACCOUNT_NUMBER_INDEX]);

		//Get the type of transaction
		printf("What would you like to do today?\n");
		printf("0 - Balance Inquiry, 1 - Deposit, 2 - Withdrawl, 3 - Transfer Funds\n");
		printf("> ");

		int response;
		scanf("%d", &response);
		switch(response)
		{
			case 0:
				transaction[TRANSACTION_TYPE_INDEX] = CHECK;
				transaction[T_AMOUNT_INDEX] = 0;
			break;

			case 1:
				transaction[TRANSACTION_TYPE_INDEX] = DEPOSIT;
				printf("How much would you like to deposit?\n");
				printf("> $");
				scanf("%d", &transaction[T_AMOUNT_INDEX]);
			break;

			case 2:
				transaction[TRANSACTION_TYPE_INDEX] = WITHDRAW;
				do
				{
					printf("How much would you like to withdraw? (Increments of $20)\n");
					printf("> $");
					scanf("%d", &transaction[T_AMOUNT_INDEX]);
				} while(transaction[T_AMOUNT_INDEX] % 20 != 0);
			break;

			case 3:
				transaction[TRANSACTION_TYPE_INDEX] = TRANSFER;
				printf("Enter in the account number that will receive funds.\n");
				printf("> ");
				scanf("%d", &transaction[T_RECEIVER_NUMBER_INDEX]);

				printf("How much would you like to transfer?\n");
				printf("> $");
				scanf("%d", &transaction[T_AMOUNT_INDEX]);
			break;
		}
		
		printf("\nSubmitting Transaction...\n");
		print_transaction(transaction);

		/* send message */
		int transaction_size = NUM_VALUES_TRANSACTION * sizeof(int);
		bytes_sent = send(sock_client, transaction, transaction_size, 0);
		printf("Number of Bytes Sent : %d bytes\n", transaction_size);
		int rec_size = NUM_VALUES_RESPONSE * sizeof(int);
		/* get response from server */
		bytes_recd = recv(sock_client, &server_response, rec_size, 0); 
		printf("\nThe response from server is:\n");
		print_response(server_response);
		printf("Number of Bytes Received : %d bytes\n\n", bytes_recd);

		//Print any errors
		if(server_response[ERROR_TYPE_INDEX] != 0)
			print_error(server_response[ERROR_TYPE_INDEX], transaction);

		printf("Anything else you need to do today? (Y/N)\n");
		printf("> ");
		char y_n;
		scanf("\n%s", &y_n);
		repeat = (y_n == 'Y') || (y_n == 'y');

		if(!repeat)
		{
			printf("Goodbye!\n");
			char goodbye[1];
			goodbye[1] = 'g';
			bytes_sent = send(sock_client, goodbye, sizeof(char), 0);
		}
		/*Set all values to 0's*/
		transaction[0] = 0;
		transaction[1] = 0;
		transaction[2] = 0;
		transaction[3] = 0;
		transaction[4] = 0;
		print_separator();
	}

	/* close the socket */
   close(sock_client);
}
