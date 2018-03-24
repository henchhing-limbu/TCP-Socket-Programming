#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
	FILE *testFile = fopen("practice_project_test_file_1","rb");
	for (int m = 0; m < 2; m++) {
	// printf("Entered loop");
	// fgetc
	// firstByte
		uint8_t type = fgetc(testFile);
		printf("Type %d ", type);
		if (type == 0) {
			uint8_t amount = fgetc(testFile);
			printf("Amount %d ", amount);
			// array to store the numbers
			uint16_t numbers[amount];
			// reading the file and storing the numbers in the array
			fread(numbers, 2, amount, testFile);
			// changing the Endianess
			uint16_t x, y;
			for (int i = 0; i < amount; i++) {
				// sll by 8
				x = numbers[i] << 8;
				// srl by 8
				y = numbers[i] >> 8;
				// 
				uint16_t z = x | y;
				printf("%d ", z);
			}
		}
		else if (type == 1) {
			uint8_t amount[3];
			// getting the value of amount
			fread(amount, 1, 3, testFile);
			for (int i = 0; i < 3; i++) {
				printf("%c", amount[i]);
			}
			// get the position of 1 or 0
			// to calculate the offset for fssek
			int count = 0;
			uint8_t charValue;
			long currOffset = ftell(testFile);
			while (1) {
				charValue = fgetc(testFile);
				if (charValue == 0 || charValue == 1) {
					break;
				}
				count += 1;
			}
			printf("Count %d ", count);
			uint8_t numbers[count];
			// changing the position of the pointer
			fseek(testFile, currOffset, SEEK_SET);
			// stores the numbers in the array
			fread(numbers, 1, count, testFile);
			for (int i = 0; i < count; i++) {
				printf("%c", numbers[i]);
			}
		}
		else {
			printf("Error.\n");
		}
	}
	fclose(testFile);
	return 0;
}