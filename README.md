# Dice2SeedGenerator

A program written in C that generates Bitcoin seed phrases from physical dice rolls.

The program converts **100 six-sided dice rolls** into two types of Bitcoin wallet seed phrases:

1. **BIP39 mnemonic**
2. **Electrum mnemonic**

The purpose of this project is to provide a transparent, open-source method for generating wallet seed phrases using physical randomness rather than relying exclusively on a computer's random number generator.

**Important:** Please read this README.md before you are using the program to generate a seed phrase for real Bitcoin funds.

## What the Program Does

The high-level process of the seed generation process consists of the following steps:

```text
100 physical dice rolls between 1 and 6
        │
        ▼
Parse numbers and validate input
        │
        ▼
Convert sequence into base6 digits (1–6 → 0–5)
        │
        ▼
Interpret sequence as a base-6 number
        │
        ▼
Convert number to binary representation
        │
        ▼
Convert binary representation into SHA-256 hash
        │
        ├───────────────┐
        ▼               ▼
     BIP39           Electrum
   mnemonic          mnemonic
```

The final step differs between BIP39 and Electrum because the two wallet standards use different mnemonic-generation mechanisms.


# BIP39 Generation

## Step 1 - Generate physical entropy

The user rolls a six-sided die **100 times**.

Each roll must result in a number between 1 and 6:

The results are entered into the program separated by spaces:

```text
4 2 6 1 5 3 6 2 4 1 ...
```

The input requires exactly **100 rolls**. 

More than one physical die can be used simultaneously to make the process faster. However, each individual die result must still be recorded and the total number of recorded results must be exactly 100.

For example, five dice can be rolled 20 times:

```text
5 dice × 20 rolls = 100 results
```

The security of this step depends on the dice being sufficiently unpredictable and the user recording the results correctly.


## Step 2 - Parse input and validate the dice input

The program reads the complete input line and parses the individual numbers.

For every number, it checks that:

* A number was actually entered.
* The number can be converted successfully.
* No integer conversion overflow occurred.
* The value is between **1 and 6**.
* Exactly **100 values** were entered.
* There is no unexpected additional input after the 100th value.

Invalid input is rejected and the user is asked to enter the sequence again.


## Step 3 - Convert dice values to base-6 digits

In the next step we convert the results into base-6 digits, which range from 0 to 5. The dice values are converted from:

```text
1 → 0
2 → 1
3 → 2
4 → 3
5 → 4
6 → 5
```

into a sequence of 100 base-6 digits.

For example:

```text
Dice:
4 2 6 1 5

Base-6:
3 1 5 0 4
```

The conversion itself does not create or remove randomness. It simply changes the representation.


## Step 4 - Convert the base-6 sequence into a large integer

In step 4 the 100 base-6 digits are interpreted as one large integer. Conceptually, the sequence:

```text
d₀ d₁ d₂ ... d₉₉
```

represents:

```text
d₀ × 6⁹⁹ + d₁ × 6⁹⁸ + ... + d₉₉
```

Because this number is far too large for standard C integer types such as `uint64_t`, the program implements the integer using an array of bytes.

The program processes the base-6 digits sequentially using repeated multiplication by 6 and addition.

The resulting number is stored as a binary integer.


## Step 5 - Convert the large integer to SHA-256 hash

The resulting large integer is represented as a fixed-length binary byte sequence.

This binary representation is then supplied to SHA-256 that creates a hash.

SHA-256 produces exactly:

```text
256 bits
```

or:

```text
32 bytes
```

The resulting 32-byte value becomes the 256-bit entropy used for BIP39 mnemonic generation.

Using the SHA-256 algorithm is a convenient way to compress the large integer. The compression is necessary because the dice input contains slightly more than 256 bits of theoretical information, while SHA-256 produces a fixed 256-bit output.


## Step 6 - Calculate the BIP39 checksum

BIP39 does not simply convert 256 bits directly into 24 words. For 256-bit entropy, BIP39 requires an additional **8-bit checksum**.

The program calculates:

```text
SHA-256(entropy)
```

and takes the first 8 bits of the resulting hash.

The data is therefore:

```text
256-bit entropy + 8-bit checksum
```

giving:

```text
264 bits
```


## Step 7 - Divide the 264 bits into 24 groups

BIP39 uses a word list containing **2048 words**.

Since:

```text
2048 = 2¹¹
```

each word corresponds to an 11-bit number.

The 264 bits are therefore divided into:

```text
264 / 11 = 24
```

groups.

Conceptually:

```text
256 bits entropy
       +
8 bits checksum
       │
       ▼
264 bits
       │
       ▼
24 × 11-bit indexes
```

Each index has a value from 0 to 2047.


## Step 8 - Convert the indexes into BIP39 words

The program loads the official BIP39 English word list, which contains 2048 words. Each 11-bit index is used as an index into the word list.

For example:

```text
index 0    → abandon
index 1    → ability
index 2    → able
...
index 2047 → zoo
```

The 24 resulting words form the BIP39 mnemonic.


# Important security considerations

## Physical randomness

The security of the generated seed phrase, to a large extend, depends on the randomness of the dice rolls.

The user should comply with the following requirements:

* Use dice rolls as real physical randomness rather than a random number generator that creates a computer-generated sequence.
* Use properly manufactured dice that don't generate biased results.
* Roll the dice in a way that does not intentionally or unintentionally bias the results.
* Record every result accurately.
* Ensure exactly 100 independent rolls are used.
* Run the program in a trusted, offline environment.
* Ensure that the computer is free from malware, spyware, keyloggers and other potentially malicious software.
* A dice sequence used to generate a wallet should be treated as sensitive secret material.
* Do not publish or reuse the sequence for another wallet.
* Anyone who obtains the complete dice sequence and knows the exact generation procedure may be able to reproduce the resulting wallet.
* The program cannot compensate for predictable or manipulated physical input. It is your responsibility to make sure that the dice rolls are truly random.


# Electrum seed generation

Electrum uses a different mnemonic-generation mechanism from BIP39. Therefore, the BIP39 procedure described above **must not be assumed to apply to Electrum seeds**.

The Electrum implementation is currently under development and will be documented separately once the implementation and its test procedure have been finalized.


# Project status

**Currently under development.**

The BIP39 generation pipeline is implemented and has been tested by importing a generated mnemonic into Sparrow Wallet.

Further testing against official test vectors, independent implementations and the complete wallet derivation process is required before the software should be considered production-ready.


# Warning

**This project is still experimental.**

Do not use Dice2SeedGenerator to generate a wallet containing real Bitcoin until the implementation has been thoroughly tested and independently reviewed. I will further test the program in the following days, report the test results and update the project status. 

Bitcoin seed generation is security-critical software. An implementation error can result in permanent loss of funds.

The author makes no guarantee that the generated seed phrases are secure or suitable for storing Bitcoin or other cryptocurrencies. Don't trust, but verify. Please, review the code indepentendly and provide constructive feedback, if you find any bugs. 





