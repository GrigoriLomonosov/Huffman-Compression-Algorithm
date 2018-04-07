#include "Common.h"

void set_bit(unsigned char* arr, int k) {
	arr[k / BYTE_LENGTH] = arr[k / BYTE_LENGTH] | (0x01 << (BYTE_LENGTH - 1 - (k%BYTE_LENGTH)));
}

void clear_bit(unsigned char* arr, int k) {	
	arr[k / BYTE_LENGTH] = arr[k / BYTE_LENGTH] & ~(0x01 << (BYTE_LENGTH - 1 - (k%BYTE_LENGTH)));
}

int test_bit(unsigned char* arr, int k) {
	return (arr[k / BYTE_LENGTH] & (0x01 << (BYTE_LENGTH - 1 - (k%BYTE_LENGTH)))) != 0;
}

int test_bit_in_char(unsigned char c, int k) {
	return (c & (0x01 << (BYTE_LENGTH - 1 - k))) != 0;
}

void write_bit(unsigned char* buffer, int index, int binary_number) {
	if (binary_number == 1) {
		set_bit(buffer, index);
	}
	else {
		clear_bit(buffer, index);
	}
}

long long get_file_size(FILE* file) {
	fseek(file, 0L, SEEK_END);
	long long size = ftell(file);
	rewind(file);
	return size;
}