#ifndef CREATE_BIP39_SEED_H
#define CREATE_BIP39_SEED_H 

void create_bip39_seed(void);

void convert_dice_to_base6_digits(
	const int rolls[], 
	int count, 
	int base6_rolls[]
	);

#endif 