#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void menu_main_info(void) {
	printf("================================================================\n\n");

	printf("This program converts dice rolls into two types of seed phrases: \n");
	printf("1. BIP39 mnemonic\n");
	printf("2. Electrum mnemonic");
	printf("\n\n");

	printf("By using this program, you acknowledge and accept the following\n");
	printf("terms and requirements:");
	printf("\n\n");

    printf("1. Run this program in a trusted, offline environment.\n\n");

    printf("2. Ensure that the computer is free from malware, spyware,\n");
    printf("   keyloggers and other potentially malicious software.\n\n");

    printf("3. Don't trust, but verify. This program is open source. You are\n");
    printf("   responsible for reviewing and verifying the source code before\n");
    printf("   using it to generate a Bitcoin wallet seed.\n\n");

    printf("4. The author provides this software \"as is\" and accepts no\n");
    printf("   responsibility or liability for any loss, damage or\n");
    printf("   compromise resulting from its use.\n\n");

	printf("5. For more technical details, visit the GitHub repository.\n\n");

    printf("================================================================\n\n");
}