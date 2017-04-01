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

#define STRING_SIZE 1024

typedef enum {false, true} bool;

typedef enum 
{
	CHECK,
	DEPOSIT,
	WITHDRAW,
	TRANSFER	
} action;

typedef struct 
{
	action transaction_type;
	int account_type;
	int account_number;
	int amount;	
	int receiver_number;
} transaction;


void print_transaction(transaction* t)
{
	printf("\n----------------------------------------------\n");
	printf("TRANSACTION RECEIPT\n");
	printf("Account Number : %d\n", t->account_number);
	printf("Account Type : %d\n", t->account_type);
	printf("Transaction Type : %d\n", t->transaction_type);
	printf("Amount : %d\n", t->amount);
	printf("Receiver Number : %d\n", t->receiver_number);
	printf("----------------------------------------------\n");
}

void print_separator()
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

int main(void) {

   int sock_client;  /* Socket used by client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char message[STRING_SIZE];  /* send message */
   int x; /* receive message */
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
		//New transaction
		transaction* t = malloc(sizeof(transaction));	
		scanf("%d", &t->account_number);

		printf("Please enter in your account type.\n");	
		printf("0 - Savings, 1 - Checking\n");	
		printf("> ");
		scanf("%d", &t->account_type);

		printf("What would you like to do today?\n");
		printf("0 - Balance Inquiry, 1 - Deposit, 2 - Withdrawl, 3 - Transfer Funds\n");
		printf("> ");

		int response;
		scanf("%d", &response);
		switch(response)
		{
			case 0:
				t->transaction_type = CHECK;
			break;

			case 1:
				t->transaction_type = DEPOSIT;
				printf("How much would you like to deposit?\n");
				printf("> $");
				scanf("%d", &t->amount);
			break;

			case 2:
				t->transaction_type = WITHDRAW;
				do
				{
					printf("How much would you like to withdraw? (Increments of $20)\n");
					printf("> $");
					scanf("%d", &t->amount);
				} while(t->amount % 20 != 0);
			break;

			case 3:
				t->transaction_type = TRANSFER;
				printf("Enter in the account number that will receive funds.\n");
				printf("> ");
				scanf("%d", &t->receiver_number);

				printf("How much would you like to transfer?\n");
				printf("> $");
				scanf("%d", &t->amount);
			break;
		}
		
		printf("\nSubmitting Transaction...\n");
		//DEBUG
		print_transaction(t);

		/* send message */
		bytes_sent = send(sock_client, t, sizeof(t) + 1, 0);
		printf("Number of Bytes Sent : %d bytes\n", sizeof(t));

		/* get response from server */
		bytes_recd = recv(sock_client, &x, 4, 0); 

		printf("\nThe response from server is:\n");
		printf("%d\n", x);
		printf("Number of Bytes Received : %d bytes\n\n", sizeof(x));

		printf("Anything else you need to do today? (Y/N)\n");
		printf("> ");
		char y_n;
		scanf("\n%s", &y_n);
		repeat = y_n == 'Y';

		if(!repeat)
			printf("Goodbye!\n");

		print_separator();
	}

	/* close the socket */
   close (sock_client);
}
