#ifndef FUNCTIONS_PARSER_H
#define FUNCTIONS_PARSER_H 

int parse_main_menu_choice(void);

int parse_dice_rolls(int rolls[], int count);

int parse_bip39_wordlist(const char *filename, char *wordlist[2048]);

#endif
