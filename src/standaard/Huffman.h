#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "Common.h"
#include "Tree.h"

typedef struct Huffman {
	//number of used characters
	int* used_char;
	//an array of the size of the charset to hold the frequencies of the characters. The index in the array of frequencies == index of the character in the charset.
	long long* frequencies;
	//the root of the tree to calculate the prefixcodes.
	Node* root;
	//array of strings holding the prefixcodes. The +1 is to hold the prefix for the separator.
	unsigned char prefixes[NUMBER_OF_CHAR + 1][NUMBER_OF_CHAR];
	long long* number_of_char_in_text;
} Huffman;

Huffman* init_empty_Huffman();
void delete_Huffman(Huffman* huffman);
void print_Huffman(Huffman* huffman);

/*
When a filepointer is given, this function reads the whole file and generates all the necessary data for Huffman-encoding.
A pointer to a struct Huffman, with all the data, is returned.
*/
Huffman* generate_data(FILE* fp);

/*
Compresses the data in the file pointed to by the given filepointer 'input' using Huffman-encoding 
and writes the results to 'output'.
*/
void compress(Huffman* huffman, FILE* input, FILE* output);

/*
Decompresses a file 'input' encoded with Huffman. The result is written to 'output'.
*/
void decompress(FILE* input, FILE* output);

/*
Adds 1 to the frequency of the character at the given index in charset.
Also adds 1 to number_of_char_in_text. When the frequency reaches LLONG_MAX the frequency is no longer updated.
*/
void add_frequency(Huffman* huffman, int index);

/*
Calculates a tree that can be used to calculate the prefixcodes of the characters in the charset. 
*/
void calc_tree(Huffman* huffman);

/*
Creates a matrix holding the prefixcodes for the characters in the charset. When a character was not used in a text, its row remains empty.
The row number equals the index of the character in the charset.
*/
void create_prefixes_array(Huffman* huffman);

/*
Returns the prefixcode for a given character.
*/
char* get_prefix(Huffman* huffman, char c);

/*
When a tree is given, this functions finds out if the tree already contains a substring of str. The part of str that is not in the tree is than
added. This happens by adding two children to the latest visited node and going down the path which equals the following char in str. This continues until
the complete str is in the tree and the character c is added as a leave. The string str contains only 1's and 0's.
*/
void add_to_decode_tree(Node* root, char* str, int c); 

/*
Returns the left child of a given Node when 0 is entered, the right child when 1 is entered.
*/
Node* descend_in_tree(Node* start, int i);

#endif