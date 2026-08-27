
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int parse_main_menu_choice(void) {
	char menu_input[10];
	char *endptr;

	/* Read the entire input from the user */
	if (fgets(menu_input, sizeof(menu_input), stdin) == NULL) {
		return 0;
	}

	/* Convert string into number */
	long value = strtol(menu_input, &endptr, 10);

	/* Check whether input is a number */
	if (endptr == menu_input) {
		return 0;
	}

	/* Check whether user input is outside the range of menu options */
	if (value < 1 || value > 3) {
		return 0;
	}
	return (int)value;
}


int parse_dice_rolls(int rolls[], int count) {
	char dice_rolls_input[1000];
	char *ptr; 
	char *endptr;
	
	/* Read the entire input from the user */
	if (fgets(dice_rolls_input, sizeof(dice_rolls_input), stdin) == NULL) {
		return 0;
	}

	/* Start at the beginning of the input */
	ptr = dice_rolls_input;

	/* Parse each dice roll */
	for (int i = 0; i < count; i++) {
		
		/* Skip spaces and tabs */
		while (*ptr == ' ' || *ptr == '\t') {
			ptr++;
		}

		/* Reset errno before strtol() */
		errno = 0;

		/* Convert the next string into a number */
		long value = strtol(ptr, &endptr, 10);

		/* Check whether strtol actually found a number */
		if (endptr == ptr) {
			return 0;
		}

		/* Check whether conversion led to an overflow */
		if (errno == ERANGE) {
			return 0;
		}

		/* Check whether dice result is a valid number between 1 and 6 */
		if (value  < 1 || value > 6) {
			return 0;
		}

		/* Store the result in the array */
		rolls[i] = (int)value;

		/* Move the pointer to where strtol() stopped */
		ptr = endptr; 
	}

	/* Skip spaces/tabs after the last roll */
	while (*ptr == ' ' || *ptr == '\t') {
		ptr++; 
	}

	/* Checker whether there is something else after the last dice roll */
	if (*ptr != '\n' && *ptr != '\0') {
		return 0;
	}

	return 1;
}


/* Function that parses 2048 words from bip39 words list*/

int parse_bip39_wordlist(
    const char *filename,
    char *wordlist[2048]
	) {
    FILE *file;
    char buffer[100];
	
	/* Open file and check whether it was opened */
    file = fopen(filename, "r");

    if (file == NULL) {
        return 0;
    }

	/* Loop over word lists and read exactly 2048 lines */
    for (int i = 0; i < 2048; i++) {

        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            fclose(file);
            return 0;
        }

        /* Remove newline */
        buffer[strcspn(buffer, "\r\n")] = '\0';
        wordlist[i] = malloc(strlen(buffer) + 1);

        if (wordlist[i] == NULL) {
            fclose(file);
            return 0;
        }

        strcpy(wordlist[i], buffer);
    }

    fclose(file);

    return 1;
}