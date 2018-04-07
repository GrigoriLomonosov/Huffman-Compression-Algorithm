#ifndef TESTSUITE_H
#define TESTSUITE_H

#include "Huffman.h"

#include <time.h>

void test();

void generate_random_numbers(FILE* output, long long size, long long min_jump, long long start);

void generate_files();
#endif
