#ifndef TREE_H
#define TREE_H

#include "Common.h"

typedef struct Node {
	long long frequency;
	//An integer is used as 'character'. This to allow the use of the 2 macro's in Huffman.h next to the 256 characters.
	int character;
	struct Node* left_child;
	struct Node* right_child;
}Node;

Node* init_node(int character, long long frequency);
void delete_node(Node* root);

/*
Creates a new parent node for the two given child_nodes. The parent node is returned.
The parent has EMPTY_CHAR (see Huffman.h) as character and the sum of the frequencies of the children as frequency.
*/
Node* merge_node(Node* left, Node* right);

#endif