#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int asciiToDecimal(uint8_t amount[]);
void decimalToAscii(uint8_t amount, uint8_t *array);

int main() {
	FILE *testFile = fopen("practice_project_test_file_1","rb");
	//FILE *testFile = fopen("practice_project_test_file_2","rb");
	
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
		printf("Type %d\t", type);
		
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
				// sll by 8
				x = numbers[i] << 8;
				// srl by 8
				y = numbers[i] >> 8;
				// 
				uint16_t z = x | y;
				printf("%d", z);
				if(i != amount-1)
					printf(",");
			}
			printf("\n");
		}
		
		// Type 1
		else if (type == 1) {
			uint8_t amount[3];
			// getting the value of amount
			fread(amount, 1, 3, testFile);
			int amountInDecimal = asciiToDecimal(amount);
			
			// print the amount 
			printf("Amount: ");
			for (int i = 0; i < 3; i++) {
				printf("%c", amount[i]);
			}
			printf("\t");
			// printf(" In decimal %d ", amountInDecimal);
			printf(" ");
			
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
		}
		else {
			printf("Error.\n");
		}
		temp = ftell(testFile);
	}
	fclose(testFile);
	return 0;
}

// helper function to conver ascii to decimal numbers
int asciiToDecimal(uint8_t amount[]) {
	int temp = atoi(amount);;
	/*
	for (int i =0; i < 2; i++) {
		temp += amount[i] - '0';
		// printf(" Temp value: %d ", temp); 
		temp = temp << 1;
	}
	temp += amount[2] - '0';
	*/
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