#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdint.h>
#include <openssl/sha.h>

#include "./menu_header.h"
#include "./menu_main_info.h"
#include "./functions_parser.h"


void convert_dice_to_base6_digits(
	const int rolls[], 
	int count, 
	int base6_rolls[]
	);


int convert_base6_digits_to_largeint(
	const int base6_rolls[],
	int count,
	uint8_t number[]
	);


int convert_binary_integer_to_sha256(
	const uint8_t number[],
	size_t number_length, 
	uint8_t hash[32] 
	);


void convert_hash_to_bip39_indexes (
	const uint8_t entropy[32],
	uint16_t indexes[24]
	);


void convert_indexes_to_bip39_words(
	const uint16_t indexes[24],
	char *wordlist[2048]
	);


int create_bip39_seed(void) {
	int rolls[100];
	int count = 100;
	int base6_rolls[100];
	char continue_button[100];
	uint8_t number[33] = {0};
	uint8_t hash[32];
	uint16_t indexes[24];
	char *wordlist[2048];

	printf("\033[2J\033[H");
	menu_header();
	printf("================================================================\n");
	printf("                          DICE ENTROPY\n");
	printf("================================================================\n\n");
	printf("1. Roll a six-sided dice 100 times. You can speed up the process\n");
	printf("   by using more than one dice at the same time.\n");
	printf("2. Each result must be a number between 1 and 6.\n");
	printf("3. Enter the results separated by spaces.\n\n");

	/* Check whether the input from dice rolls is correct, if not try again*/
	while (1) {
		printf("Enter the results below: \n");
		if (parse_dice_rolls(rolls, count) == 1) {
			printf("All dice rolls are valid!\n");
			break;
		}
		printf("Invalid dice input. Please try again.\n");
	}

	/* Convert dice rolls into base-6 digits */
	convert_dice_to_base6_digits(rolls, count, base6_rolls);

	printf("\n\n");
	printf("Base6 digits output sequence:\n");

	for (int i = 0; i < 100; i++) {
		printf("%d ", base6_rolls[i]);
	}
	printf("\n\n\n");

	/* Convert base-6 digits into a large binary integer */
	if (convert_base6_digits_to_largeint(base6_rolls, count, number) == 0) {
		printf("Error: number is too large.\n\n");
		return 0;
	}

	/* Print and check binary integer */
	printf("Binary integer (hex):\n");

	for (int i = 0; i < 33; i++) {
		printf("%02x", number[i]);
	}
	printf("\n\n\n");

	/* Convert the binary integer to SHA256 hash */
	convert_binary_integer_to_sha256(number, 33, hash);
	printf("\n\n\n");

	/* Convert entropy + checksum into BIP39 indexes */
	convert_hash_to_bip39_indexes(hash, indexes);	
	printf("BIP39 word indexes:\n");

	for (int i = 0; i < 24; i++) {
		printf("%d ", indexes[i]);
	}

	printf("\n\n\n");

	if (parse_bip39_wordlist("Words/bip39_word_list.txt", wordlist) == 0) {
		printf("Could not load BIP39 wordlist.\n\n");
		return 0;
	}

	convert_indexes_to_bip39_words(indexes, wordlist);

	printf("\n\n\n");

	printf("Press 'Enter' to continue and create seed and seed phrase\n");
	fgets(continue_button, sizeof(continue_button), stdin);

}


/* Function that converts dice rolls number into base6 digits 1->0, 2->1, ...*/

void convert_dice_to_base6_digits(
	const int rolls[], 
	int count, 
	int base6_rolls[]
) {
	for (int i = 0; i < count; i++) {
		base6_rolls[i] = rolls[i] - 1;
	}
}


/* Convert base-6 digits into a 33 byte binary integer */

int convert_base6_digits_to_largeint(
	const int base6_rolls[],
	int count,
	uint8_t number[]
) {
	for (int i = 0; i < count; i++) {
		int digit = base6_rolls[i];
		int carry = digit; 

		for (int j = 32; j >= 0; j--) {
			int value = number[j] * 6 + carry;
			number[j] = value & 0xFF;
			carry = value >> 8;
		}

		/* Check whether the number overflowed */
		if (carry != 0) {
			return 0;
		}	
	}
	return 1;
} 


int convert_binary_integer_to_sha256(
	const uint8_t number[],
	size_t number_length, 
	uint8_t hash[32] 
) {
	SHA256(number, 33, hash);
	
	printf("Hash of binary integer:\n");
	
	for (int i = 0; i < 32; i++) {
		printf("%02x", hash[i]);
	}
}


void convert_hash_to_bip39_indexes (
	const uint8_t entropy[32],
	uint16_t indexes[24]
) {
	uint8_t checksum_hash[32];
	uint8_t bits[33];

	/* Calculate SHA-256 hash of the entrpy */
	SHA256(entropy, 32, checksum_hash);

	/* Copy the 256 bit entropy */
	memcpy(bits, entropy, 32);

	/* Add the first 8 bits of the checksum */
	bits[32] = checksum_hash[0];

	/* Extract 24 groups of 11 bits */
	for (int i = 0; i < 24; i++) {
		uint16_t index = 0;

		for (int j = 0; j < 11; j++) {
			int bit_position = i * 11 + j;
			int byte_position = bit_position / 8;
			int bit_offset = 7 - (bit_position % 8);
			uint8_t bit = (bits[byte_position] >> bit_offset) & 1;
			index = (index << 1) | bit; 
		}
		 
		indexes[i] = index;
	}
}


void convert_indexes_to_bip39_words(
	const uint16_t indexes[24],
	char *wordlist[2048]
) {
	printf("Seed phrase:\n");
    for (int i = 0; i < 24; i++) {
        printf("%s ", wordlist[indexes[i]]);
    }
}