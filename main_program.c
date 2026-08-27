#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "modules/menu_header.h"
#include "modules/menu_main_info.h"
#include "modules/functions_parser.h"
#include "modules/create_electrum_seed.h"
#include "modules/create_bip39_seed.h"


int main (void) {

	int menu_choice;
	
	while(1) {
		printf("\033[2J\033[H");
		menu_header();
		menu_main_info();
		printf("----MAIN MENU----\n");
		printf("1: Create Electrum Seed\n");
		printf("2: Create BIP 39 Seed\n");
		printf("3: Exit\n");

		printf("Select menu: ");

		menu_choice = parse_main_menu_choice();
		if (menu_choice) {
			switch (menu_choice) {
				case 1:
					create_electrum_seed();
					break;

				case 2:
					create_bip39_seed();
					break;
				
				case 3:
					printf("Good Bye!\n");
					return 0;
		}
		} else {
			printf("Invalid input\n");
			printf("Select menu: ");
			menu_choice = parse_main_menu_choice();
		}
	}
	return 0;
}