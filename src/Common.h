#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#define NUMBER_OF_CHAR 256
#define SEPARATOR -2
#define EMPTY_CHAR -3
//The buffer size is the amount of characters that will be held in memory.
#define BUFFER_SIZE 8192
#define BYTE_LENGTH 8

/*
Sets the k-th bit in array arr to 1.
*/
void set_bit(unsigned char* arr, int k);

/*
Sets the k-th bit in array arr to 0.
*/
void clear_bit(unsigned char* arr, int k);

/*
Returns the value of the bit at position k in the array.
*/
int test_bit(unsigned char* arr, int k);

/*
Returns the value of the bit at position k in the character.
*/
int test_bit_in_char(unsigned char c, int k);

/*
Sets the bit at position index in the array to the given binary_number (1 or 0).
*/
void write_bit(unsigned char* buffer, int index, int binary_number);

/*
Returns an integer that represents the number of bytes in a given file.
*/
long long get_file_size(FILE* file);

#endif 