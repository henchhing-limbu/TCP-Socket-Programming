#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  our own helper functions  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*  Global constants  */

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)

// function declarations
uint8_t asciiToDecimal(uint8_t amount[]);
void decimalToAscii(uint8_t amount, uint8_t *array);
void writeToType0(FILE* outputStream, uint8_t type, uint8_t amount, uint16_t array[]);
void writeToType1(FILE* outputStream, uint8_t type, uint8_t* amount, int count, uint8_t* numbers);
void type0ToType1(uint8_t* amountArray, uint16_t* numbers, FILE* outputStream, int amount);
void type1ToType0(FILE* outputStream, uint8_t amount, uint8_t* numbers, int count);

int convertFile(int format, FILE* sourcefile, FILE* destfile);

int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */
	unsigned long filesize = 0;
	FILE* file;
	// TODO: need these inputs from client
	// TODO: hard-coded it for now
	FILE* destfile;
	int format = 1;
	
    //  Get port number from the command line, and
    //  set to default port if no arguments were supplied 

    if ( argc == 2 ) {
		port = strtol(argv[1], &endptr, 0);
		if ( *endptr ) {
			fprintf(stderr, "ECHOSERV: Invalid port number.\n");
			exit(EXIT_FAILURE);
		}
    }
    else if ( argc < 2 ) {
		port = ECHO_PORT;
    }
    else {
		fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
		exit(EXIT_FAILURE);
    }

	
    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling bind()\n");
		exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling listen()\n");
		exit(EXIT_FAILURE);
    }

    
    /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {
		/*  Wait for a connection, then accept() it  */
		if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
			fprintf(stderr, "ECHOSERV: Error calling accept()\n");
			exit(EXIT_FAILURE);
		}
		
		// file size reading and writing
		Readline(conn_s, &filesize, sizeof(long));
		printf("Received file size from the client.\n");
		printf("Filesize: %lu\n", filesize);
		Writeline(conn_s, &filesize, sizeof(long));
		printf("Send the file size to the client.\n");
		
		// Retrieve an input line from the connected socket
	    // then simply write it back to the same socket.
		Readline(conn_s, buffer, filesize);
		printf("Received file from client.\n");
		printf("Buffer: %s\n", buffer);
		
		// writing to a file
		file = fopen("receivedFile","wb+");
		printf("Writing the data from buffer to the file.\n");
		fwrite(buffer, 1, filesize, file); 
		
		// translating the file and saving the file to the destination file
		// TODO: change needed here
		// FILE* sourcefile = fopen("practice_project_test_file_1","rb");
		destfile = fopen("outputFile","wb");
		// translating the file
		// checking for error as well
		// Readline(conn_s, &errorMessage, sizeof(int));
		int errorMessage = convertFile(format, file, destfile);
		// sending errormessage to the client
		Writeline(conn_s, &errorMessage, sizeof(int));
		
		// Writeline(conn_s, buffer, filesize);
		printf("Sent response to client.\n");


		/*  Close the connected socket  */

		if ( close(conn_s) < 0 ) {
			fprintf(stderr, "ECHOSERV: Error calling close()\n");
			exit(EXIT_FAILURE);
		}
    }
}

int convertFile(int format, FILE* sourcefile, FILE* outputStream) {
	// moving the pointer to the end of the file
	fseek(sourcefile, 0, SEEK_END);
	// gives the offset
	long fileSize = ftell(sourcefile);
	// moving the pointer back to the start of the file
	fseek(sourcefile, 0, SEEK_SET);
	// gives the offset which must be 0
	long temp = ftell(sourcefile);
	
	// printing filesize and temp
	printf("Filesize: %lu\n", fileSize);
	printf("Temp: %lu\n", temp);
	
	// loop until the end of the file
	while (temp < fileSize) {
		// firstByte either 0 or 1
		uint8_t type = fgetc(sourcefile);
		printf("Type %d", type);
		printf("\t");
		
		// Type 0
		if (type == 0) {
			uint8_t amount = fgetc(sourcefile);
			uint8_t amountArray[3];
			// calls the function to convert the decimal amount 
			// to ASCII characters stored in an array
			decimalToAscii(amount, amountArray);
			// printing the amount
			printf("Amount: ");
			for (int i = 0; i < 3; i++) {
				printf("%c", amountArray[i]);
			}
			printf("\t");
			
			// array to store the numbers
			uint16_t numbers[amount];
			// reading the file and storing the numbers in the array
			fread(numbers, 2, amount, sourcefile);
			// changing the Endianess
			uint16_t x, y;
			
			// print the numbers
			for (int i = 0; i < amount; i++) {
				x = numbers[i] << 8;
				y = numbers[i] >> 8; 
				uint16_t z = x | y;
				printf("%d", z);
				if(i != amount-1)
					printf(",");
			}
			printf("\n");
			
			// checking conditions for different translations
			if (format == 0 || format == 2) {
				writeToType0(outputStream, type, amount, numbers);
			}
			else if (format == 1 || format == 3) {
				type0ToType1(amountArray, numbers, outputStream, amount);
			}
		}
		// Type 1
		else if (type == 1) {
			uint8_t amount[3];
			// getting the value of amount
			fread(amount, 1, 3, sourcefile);
			uint8_t amountInDecimal = asciiToDecimal(amount);
			uint8_t num = amountInDecimal;
			
			// print the amount 
			printf("Amount: ");
			for (int i = 0; i < 3; i++) {
				printf("%c", amount[i]);
			}
			printf("\t");
			// get the position of 1 or 0
			// to calculate the offset for fseek
			int count = 0;
			uint8_t charValue;
			long currOffset = ftell(sourcefile);
			
			// break until the end of unit is not reached
			while (1) {
				charValue = fgetc(sourcefile);
				// if amountInDecimal is below 2
				if (amountInDecimal < 2) {
					if (charValue == 0 || charValue == 1) {
						break;
					}
				}
				// if it reaches the end of the file
				// increments count and breaks
				if (ftell(sourcefile) == fileSize) {
						count++;
						break;
				}
				// if comma, subtract 1 from amountInDecimal
				if (charValue == ',') {
					// printf("Found , ");
					amountInDecimal -= 1;
				}
				count++;
			}
			
			// array to store the numbers
			uint8_t numbers[count];
			// changing the position of the pointer
			fseek(sourcefile, currOffset, SEEK_SET);
			// stores the numbers in the array
			fread(numbers, 1, count, sourcefile);
			
			// prints the numbers
			for (int i = 0; i < count; i++) {
				printf("%c", numbers[i]);
			}
			printf("\n");
			
			// checking conditions for different translations
			if (format == 0 || format == 1) {
				writeToType1(outputStream, type, amount, count, numbers);
			}
			if (format == 2 || format == 3) {
				type1ToType0(outputStream, num, numbers, count);
			}
		}
		else {
			printf("Error with the format of the file.\n");
			return -1;
		}
		temp = ftell(sourcefile);
	}
	fclose(sourcefile);
	fclose(outputStream);
	return 0;
}

// helper function to conver ascii to decimal numbers
uint8_t asciiToDecimal(uint8_t amount[]) {
	uint8_t temp = atoi(amount);;
	return temp;
}

// helper function to convert decimal to ascii characters
void decimalToAscii(uint8_t amount, uint8_t *array) {
	int remainder;
	int i = 2;
	// population the array with corresponding ascii characters
	while(i >= 0) {
		remainder = amount % 10;
		amount = amount / 10;
		array[i] = remainder + '0';
		i -= 1;
	}
}

// writes the type 0 unit to  an output file
void writeToType0(FILE* outputStream, uint8_t type, uint8_t amount, uint16_t* array) {
	int unitLength = amount * 2 + 1 + 1;
	
	// array to store all the data of the unit
	uint8_t unitData[unitLength];
	// copying the first byte into unitData
	memcpy(unitData, &type, 1);
	// copying the second byte (amount) to unitData
	memcpy(unitData + 1, &amount, 1);
	// copyting the numbers array to unitData
	memcpy(unitData + 2, array, amount * 2);
	
	// printf("%c\n", unitData[unitLength-1]);
	
	
	// writing to a *outStream
	fwrite(unitData, 1, unitLength, outputStream);
}

// writes the type 1 unit to an output file 
void writeToType1(FILE* outputStream, uint8_t type, uint8_t* amount, int count, uint8_t* numbers) {
	int unitLength = count + 1+ 3;
	uint8_t unitData[unitLength];
	memcpy(unitData, &type, 1);
	memcpy(unitData + 1, amount, 3);
	memcpy(unitData + 4, numbers, count);
	fwrite(unitData, 1, unitLength, outputStream);
}

// translates to type1 format
void type0ToType1(uint8_t* amountArray, uint16_t* numbers, FILE* outputStream, int amount) {
	// change the first byte
	// TODO: Assume for now these values are passed
	uint8_t type = 1;
	fwrite(&type, 1, 1, outputStream);
	
	fwrite(amountArray, 1, 3, outputStream); 
	int count = 0;
	char comma = ',';
	for (int i = 0; i < amount; i++) {
		char buffer[5];
		int num = numbers[i];
		count = snprintf(buffer, 5, "%d", num);
		// depending on the value of count add the char from buffer into file
		fwrite(buffer, 1, count, outputStream);
		if (i != amount-1) 
			fwrite(&comma, 1, 1, outputStream);
	}
}

void type1ToType0(FILE* outputStream, uint8_t amount, uint8_t* numbers, int count) {
	// TODO: 
	// changing the first byte
	uint8_t type = 0;
	fwrite(&type, 1, 1, outputStream);
	
	// writing the decimal amount to the file
	// TODO: Fix this
	// Convert to a byte/ maybe more than a byte
	fwrite(&amount, 1, 1, outputStream);
	int start = 0;
	int end = 0;
	int x = 0;
	// writing the decimal values to the file
	for (int i = 0; i < count; i++ ) {
		// if char is ','
		if ((numbers[i] == ',') || (i == count - 1)) {
			end = i;
			if (i == count - 1)
				end++;
			// creating a sliced array			
			// contains the chars to convert to 2 byte numbers
			uint8_t slicedNumbers[end-start];
			for (int j = start; j < end; j++) {
				slicedNumbers[x] = numbers[j];
				x++;
			}
			x = 0;
			// converting char to decimal value
			uint16_t num = atoi(slicedNumbers);
			/*
			// TODO: Find better way
			// putting num as two bytes in an array
			uint8_t numArray[2];
			numArray[1] = (uint8_t) num;
			num = num >> 8;
			numArray[0] = (uint8_t) num;
			*/
			// finally wrting the 2 bytes into the file
			fwrite(&num, 1, 2, outputStream);
			start = end + 1;
		}
	} 
}
