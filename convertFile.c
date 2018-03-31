#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// function declarations
uint8_t asciiToDecimal(uint8_t amount[]);
void decimalToAscii(uint8_t amount, uint8_t *array);
void writeToType0(FILE* outputStream, uint8_t type, uint8_t amount, uint16_t array[]);
void writeToType1(FILE* outputStream, uint8_t type, uint8_t* amount, int count, uint8_t* numbers);
void type0ToType1(uint8_t* amountArray, uint16_t* numbers, FILE* outputStream, int amount);
void type1ToType0(FILE* outputStream, uint8_t amount, uint8_t* numbers, int count);

int convertFile(int format, FILE* sourcefile, FILE* destfile) {
	// moving the pointer to the end of the file
	fseek(testFile, 0, SEEK_END);
	// gives the offset
	long fileSize = ftell(testFile);
	// moving the pointer back to the start of the file
	fseek(testFile, 0, SEEK_SET);
	// gives the offset which must be 0
	long temp = ftell(testFile);
	
	// printing filesize and temp
	printf("Filesize: %lu\n", fileSize);
	printf("Temp: %lu\n", temp);
	
	// loop until the end of the file
	while (temp < fileSize) {
		// firstByte either 0 or 1
		uint8_t type = fgetc(testFile);
		printf("Type %d", type);
		printf("\t");
		
		// Type 0
		if (type == 0) {
			uint8_t amount = fgetc(testFile);
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
			fread(numbers, 2, amount, testFile);
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
			fread(amount, 1, 3, testFile);
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
			long currOffset = ftell(testFile);
			
			// break until the end of unit is not reached
			while (1) {
				charValue = fgetc(testFile);
				// if amountInDecimal is below 2
				if (amountInDecimal < 2) {
					if (charValue == 0 || charValue == 1) {
						break;
					}
				}
				// if it reaches the end of the file
				// increments count and breaks
				if (ftell(testFile) == fileSize) {
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
			fseek(testFile, currOffset, SEEK_SET);
			// stores the numbers in the array
			fread(numbers, 1, count, testFile);
			
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
			return 0;
		}
		temp = ftell(testFile);
	}
	fclose(testFile);
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
