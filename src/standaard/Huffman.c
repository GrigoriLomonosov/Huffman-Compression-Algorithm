#pragma warning(disable:4996)
#include "Huffman.h"

Huffman* init_empty_Huffman() {
	Huffman* huffman = malloc(sizeof(Huffman));
	if (!huffman) {
		printf("ERROR: Allocation failed, insufficient memory for Huffman-struct!\n");
		exit(1);
	}
	
	// Create components
	huffman->number_of_char_in_text = malloc(sizeof(long long));
	*huffman->number_of_char_in_text = 0;
	huffman->used_char = malloc(sizeof(int));
	*huffman->used_char = 0;
	huffman->frequencies = malloc(sizeof(long long)*NUMBER_OF_CHAR);
	for (int i = 0; i < NUMBER_OF_CHAR; i++) {
		huffman->frequencies[i] = 0;
	}
	for (int i = 0; i < NUMBER_OF_CHAR + 1; i++) {
		huffman->prefixes[i][0] = '\0';
	}
	huffman->root = malloc(sizeof(Node));	
	if (!huffman->frequencies || !huffman->prefixes || !huffman->root || !huffman->used_char || !huffman->root){
		printf("ERROR: Allocation failed, insufficient memory for Huffman-struct!\n");
		exit(1);
	}

	return huffman;
}

void delete_Huffman(Huffman* huffman) {
	free(huffman->frequencies);
	free(huffman->number_of_char_in_text);
	free(huffman->used_char);
	delete_node(huffman->root);
	free(huffman);
}

void print_Huffman(Huffman* huffman) {
	printf("number of char in text: %lld\n", *huffman->number_of_char_in_text);
	printf("needed int: %lld\n", *huffman->number_of_char_in_text / 4);
	printf("number of bits in text: %lld\n", *huffman->number_of_char_in_text*8);
	printf("number of chars in alphabet: %d\n", NUMBER_OF_CHAR);
	printf("used characters: %d\n", *huffman->used_char);
	for (int i = 0; i < NUMBER_OF_CHAR+1; i++) {
		if (strlen(huffman->prefixes[i]) > 0) {
			printf("prefix %d: %s \n", i, huffman->prefixes[i]);
		}
	}
}

char* get_prefix(Huffman* huffman, char c) {
	return huffman->prefixes[c];
}

void add_frequency(Huffman* huffman, int index) {
	if (huffman->frequencies[index] < LLONG_MAX) {
		huffman->frequencies[index]++;
		*huffman->number_of_char_in_text = *huffman->number_of_char_in_text + 1;
	}
}

void calc_tree(Huffman* huffman) {
	//Create array of all nodes
	Node** nodes = malloc(sizeof(Node*)*(*huffman->used_char));
	if (!nodes) {
		printf("ERROR: Allocation failed, insufficient memory for Nodes array.");
		exit(1);
	}
	int arr_index = 0;
	for (int i = 0; i < NUMBER_OF_CHAR; i++) {
		if (huffman->frequencies[i] != 0) {
			nodes[arr_index] = init_node(i, huffman->frequencies[i]);
			arr_index++;
		}
	}

	//If k is the number of used characters, do k-1 merge actions
	for (int k = 0; k < (*huffman->used_char) - 1; k++) {
		//Search the 2 nodes with the lowest frequency, merge them and replace the two nodes with the parent node
		long long min[2] = { LLONG_MAX, LLONG_MAX };
		int index[2] = { -1, -1 };
		for (int i = 0; i < (*huffman->used_char); i++) {
			if ((nodes[i] != NULL) && (nodes[i]->frequency < min[0] || nodes[i]->frequency < min[1])) {
				if (min[0] >= min[1]) {
					min[0] = nodes[i]->frequency;
					index[0] = i;
				}
				else {
					min[1] = nodes[i]->frequency;
					index[1] = i;
				}
			}
		}
		Node* parent = merge_node(nodes[index[0]], nodes[index[1]]);
		nodes[index[0]] = parent;
		nodes[index[1]] = NULL;
	}

	//Set the root of the huffman tree in the struct
	huffman->root = nodes[0];
	free(nodes);
}

/*
Helper-function to create prefixes-array
*/
void visit_node(Node* node, char* current_prefix, Huffman* huffman) {
	if (node->left_child != NULL) {
		char copy[NUMBER_OF_CHAR] = "";
		strcpy(copy, current_prefix);
		visit_node(node->left_child, strcat(copy, "0"), huffman);
	}
	if (node->right_child != NULL) {
		char copy[NUMBER_OF_CHAR] = "";
		strcpy(copy, current_prefix);
		visit_node(node->right_child, strcat(copy, "1"), huffman);
	}
	else if (node->left_child == NULL && node->right_child == NULL) {
		strcpy(huffman->prefixes[node->character], current_prefix);
	}
}

/*
Helper-function to create prefixes-array.
*/
void find_longest_prefix(Huffman* hf, Node* node, long long* frequency, Node** result) {
	if (node->left_child == NULL) {
		if (node->frequency < *frequency) {
			*frequency = node->frequency;
			*result = node;
		}
		return;
	}
	find_longest_prefix(hf, node->left_child, frequency, result);
	find_longest_prefix(hf, node->right_child, frequency, result);
}

void create_prefixes_array(Huffman* huffman) {
	char start[NUMBER_OF_CHAR] = "";
	visit_node(huffman->root, start, huffman);

	//Make an extra node holding the separator character and merge it with the Node holding the longest prefix code
	long long* current_frequency = malloc(sizeof(unsigned long long));
	*current_frequency = LLONG_MAX;
	Node** result = malloc(sizeof(Node*));
	find_longest_prefix(huffman, huffman->root, current_frequency, result);
	Node* separator_node = init_node(SEPARATOR, -1);
	(*huffman->used_char)++;
	merge_node(separator_node, *result);

	//Change prefix for current longest and add prefix for separator char
	char copy[NUMBER_OF_CHAR] = "";
	strcpy(copy, huffman->prefixes[(*result)->character]);
	strcpy(huffman->prefixes[(*result)->character], strcat(copy, "0"));
	copy[strlen(copy) - 1] = '1';
	strcpy(huffman->prefixes[NUMBER_OF_CHAR], copy);
	free(current_frequency);
	free(result);
}

/*
Helper function to count the number of needed buffers
*/
long long needed_buffers(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	long long filelen = ftell(fp);
	rewind(fp);
	return (filelen + BUFFER_SIZE - 1) / BUFFER_SIZE;
}

Huffman* generate_data(FILE* fp) {
	Huffman* hf = init_empty_Huffman();
	long long number_of_buffers = needed_buffers(fp);
	unsigned char buffer[BUFFER_SIZE];

	//Count the frequencies
	for (int i = 0; i < number_of_buffers; i++) {
		int read = (fread(buffer, 1, BUFFER_SIZE, fp));
		for (int j = 0; j < read; j++) {
			add_frequency(hf, (int)buffer[j]);
		}
	}
	rewind(fp);

	//Count the number of different characters used.
	for (int i = 0; i < NUMBER_OF_CHAR; i++) {
		if (hf->frequencies[i] != 0) {
			(*hf->used_char)++;
		}
	}
	
	calc_tree(hf);
	create_prefixes_array(hf);
	//print_Huffman(hf);
	return hf;
}

void compress(Huffman* huffman, FILE* input, FILE* output) {
	//Write separator on first line.
	fwrite(huffman->prefixes[NUMBER_OF_CHAR], 1, strlen(huffman->prefixes[NUMBER_OF_CHAR]),output);
	fwrite("\n", 1, 1, output);

	//Variables needed to write Huffman-tree to the file
	int max_buffer_size_in_bits = ((*huffman->used_char - 1) * BYTE_LENGTH) + ((*huffman->used_char) * (*huffman->used_char) * 2);
	int number_of_bytes = (max_buffer_size_in_bits + BYTE_LENGTH - 1) / BYTE_LENGTH;
	unsigned char* buffer = malloc(sizeof(char)*(number_of_bytes+1));
	buffer[number_of_bytes] = '\0';
	unsigned int bit_array_index = 0;

	//Write Huffman-tree to file
	for (int i = 0; i <= NUMBER_OF_CHAR; i++) {
		if ((unsigned int)strlen(huffman->prefixes[i]) > 0) {
			//write char
			if (i != NUMBER_OF_CHAR) {
				for (int j = 1; j <= BYTE_LENGTH; j++) {
					int k = (i >> (BYTE_LENGTH - j)) & 0x01;
					write_bit(buffer, bit_array_index, k);
					bit_array_index++;
				}
			}
			else {
				for (int j = 0; j < BYTE_LENGTH; j++) {
					write_bit(buffer, bit_array_index, 1);
					bit_array_index++;
				}
			}
			//write prefix
			char* prefix = huffman->prefixes[i];
			for (int j = 0; j < (signed int)strlen(prefix); j++) {
				int k = prefix[j] - '0';
				write_bit(buffer, bit_array_index, k);
				bit_array_index++;
			}
			//write separator
			char* separator = huffman->prefixes[NUMBER_OF_CHAR];
			for (int j = 0; j < (signed int)strlen(separator); j++) {
				int k = separator[j] - '0';
				write_bit(buffer, bit_array_index, k);
				bit_array_index++;
			}
		}
	}
	fwrite(buffer, 1,(bit_array_index + BYTE_LENGTH - 1) / BYTE_LENGTH, output);
	free(buffer);

	//Variables needed to write the prefixcode to the file
	long long number_of_buffers = needed_buffers(input);
	unsigned char read_buffer[BUFFER_SIZE];
	unsigned char* write_buffer = malloc(sizeof(char)*BUFFER_SIZE + 1);
	write_buffer[BUFFER_SIZE] = '\0';
	bit_array_index = 0;
	unsigned char c;

	//Write prefixcode to file
	for (int i = 0; i < number_of_buffers; i++) {
		int read = fread(read_buffer, 1, BUFFER_SIZE, input);
		for (int j = 0; j <= read; j++) {	
			char* prefix;			
			//Check if prefix should be equal to separator-prefix
			if (j == read) {
				if (i == (number_of_buffers - 1)) {
					prefix = huffman->prefixes[NUMBER_OF_CHAR];
				}
				else {
					break;
				}
			}
			else {
				c = read_buffer[j];
				prefix = huffman->prefixes[c];
			}
			//Add prefix to write_buffer
			for (int k = 0; k < (signed int)strlen(prefix); k++) {
				int l = prefix[k] - '0';
				write_bit(write_buffer, bit_array_index, l);
				bit_array_index++;
				//if write buffer is full, write buffer to file and reset the index
				if (bit_array_index == BUFFER_SIZE*BYTE_LENGTH) {
					fwrite(write_buffer, 1, (bit_array_index + BYTE_LENGTH - 1) / BYTE_LENGTH, output);
					bit_array_index = 0;
				}
			}
			if (j == read && i == number_of_buffers - 1) {
				fwrite(write_buffer, 1, (bit_array_index + BYTE_LENGTH - 1) / BYTE_LENGTH, output);
			}
		}
	}
	free(write_buffer);
}

/*
Helper-function for decoding the Huffman-tree when decompressing.
*/
void add_to_decode_tree(Node* root, char* str, int c) {
	Node* current = root;
	for (int i = 0; i < (signed int)strlen(str); i++) {
		Node* parent = current;
		if (str[i] == '0') {
			current = current->left_child;
		}
		else if (str[i] == '1') {
			current = current->right_child;
		}
		if (current == NULL) {
			Node* new_left = init_node(EMPTY_CHAR, -1);
			parent->left_child = new_left;
			Node* new_right = init_node(EMPTY_CHAR, -1);
			parent->right_child = new_right;
			if (str[i] == '0') {
				current = parent->left_child;
			}
			else if (str[i] == '1') {
				current = parent->right_child;
			}
		}
	}
	current->character = c;
}

/*
Helper-function for decompression.
*/
Node* descend_in_tree(Node* start, int i) {
	if (i == 0) {
		return start->left_child;
	}
	else if (i == 1) {
		return start->right_child;
	}
	exit(1);
}

/*
Helper-function for decompression.
*/
int start_index_sep(char* total, char* sep) {
	char* start_sep = strstr(total, sep);
	return start_sep - total;
}

void decompress(FILE* input, FILE* output) {
	long long number_of_buffers = needed_buffers(input);
	unsigned char read_buffer[BUFFER_SIZE];
	unsigned char write_buffer[BUFFER_SIZE];

	//Get the separator from the file
	char separator[NUMBER_OF_CHAR];
	fgets(separator, NUMBER_OF_CHAR, input);
	separator[strlen(separator) - 1] = '\0';

	//Construct Huffman tree with one character
	Node* root = init_node(EMPTY_CHAR, -1);
	add_to_decode_tree(root, separator, SEPARATOR);

	//Variables needed for decoding
	bool read_tree_data = false;
	int write_buffer_index = 0;
	int first_prefixcode_index = 0;
	Node* current_tree_node = root;

	//Start decoding
	for (int i = 0; i < number_of_buffers; i++) {
		first_prefixcode_index = 0;
		int read = fread(read_buffer, 1, BUFFER_SIZE, input);
		
		//Decoding the Huffman-tree
		if (!read_tree_data) {
			for (int j = 0; j < read*BYTE_LENGTH;) {
				while (!read_tree_data) {
					//decode character
					unsigned char character = 0;
					for (int k = BYTE_LENGTH - 1; k >= 0; k--) {
						if (test_bit(read_buffer, j)) {
							character = character | (1 << k);
						}
						j++;
					}
					//decode prefix				
					char* temp = malloc((strlen(separator) * 2) + 1);
					temp[strlen(separator) * 2] = '\0';
					for (int k = 0; k < (signed int)(strlen(separator) * 2); k++) {
						temp[k] = test_bit(read_buffer, j) + '0';
						j++;
					}
					int sep_index = start_index_sep(temp, separator);
					char prefix[NUMBER_OF_CHAR] = "";
					memcpy(prefix, temp, sep_index);
					prefix[sep_index] = '\0';
					free(temp);
					//If separator is found, break out of loop to start decoding the total prefixcode of the text
					if (sep_index == 0) {
						read_tree_data = true;
						first_prefixcode_index = (j + BYTE_LENGTH) / BYTE_LENGTH;
						j = INT_MAX;
						break;
					}
					j -= strlen(separator) * 2;
					add_to_decode_tree(root, prefix, character);
					j += (strlen(prefix) + strlen(separator));
				}
			}
		}

		//Decode the prefix of the text
		if(read_tree_data) {
			current_tree_node;
			for (int j = first_prefixcode_index; j < BUFFER_SIZE; j++) {
				unsigned char_to_decode = read_buffer[j];
				for (int k = 0; k < BYTE_LENGTH; k++) {
					current_tree_node = descend_in_tree(current_tree_node, test_bit_in_char(char_to_decode, k));
					if (current_tree_node->character != EMPTY_CHAR) {
						if (current_tree_node->character == SEPARATOR) {
							current_tree_node = root;
							fwrite(write_buffer, 1, write_buffer_index, output);
							j = BUFFER_SIZE;
							break;
						}
						else {
							write_buffer[write_buffer_index] = (char)current_tree_node->character;
							if (write_buffer_index == (BUFFER_SIZE - 1)) {
								fwrite(write_buffer, 1, BUFFER_SIZE, output);
								write_buffer_index = 0;
							}
							else {
								write_buffer_index++;
							}
							current_tree_node = root;
						}
					}
				}
			}	
		}
	}
}