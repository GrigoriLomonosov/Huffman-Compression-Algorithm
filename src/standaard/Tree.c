#include "Tree.h"

Node* init_node(int character, long long frequency) {
	Node* node = malloc(sizeof(Node));
	if (!node) {
		printf("ERROR: Allocation failed, insufficient memory for Node-struct!\n");
		exit(1);
	}
	node->character = character;
	node->frequency = frequency;
	node->left_child = NULL;
	node->right_child = NULL;
	return node;
}

void delete_node(Node* root) {
	if (root != NULL) {
		delete_node(root->left_child);
		delete_node(root->right_child);
		free(root);
	}
}

Node* merge_node(Node* left, Node* right) {
	Node* parent = init_node(EMPTY_CHAR, left->frequency + right->frequency);
	parent->left_child = malloc(sizeof(Node));
	parent->left_child = left;
	parent->right_child = malloc(sizeof(Node));
	parent->right_child = right;
	return parent;
}