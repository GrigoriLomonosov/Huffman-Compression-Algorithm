#pragma warning(disable:4996)
//Code to check for memory leaks
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <time.h>

#include "Huffman.h"
#include "TestSuite.h"

bool is_empty(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	long long filelen = (long long)ftell(fp);
	if (filelen == 0) {
		return true;
	}
	else {
		return false;
	}
}

int main(int argc, char *argv[]) {
	if (argc == 4) {
		//Check if files can be opened
		FILE* input;
		input = fopen(argv[2], "rb");
		if (!input) {
			printf("ERROR: Could not open file: %s!\n", argv[2]);
			exit(1);
		}
		FILE* output;
		output = fopen(argv[3], "wb");
		if (!output) {
			printf("ERROR: Could not open file: %s!\n", argv[3]);
			exit(1);
		}

		if (!is_empty(input)) {
			//Compression
			if (strcmp("-c", argv[1]) == 0) {
				//Start the timing
				clock_t begin = clock();

				unsigned long long original_size = get_file_size(input);
				printf("Original file size: %lld\n", original_size);
				Huffman* hf = generate_data(input);
				compress(hf, input, output);
				delete_Huffman(hf);
				unsigned long long compressed_size = get_file_size(output);
				printf("Compressed file size: %lld\n", compressed_size);
				printf("Compression-factor: %lf\n", (double)compressed_size / original_size);

				//Calculate the time needed to compress
				clock_t end = clock();
				double cpu_time = (double)(end - begin) / CLOCKS_PER_SEC;
				printf("Used CPU-time for compression: %lf", cpu_time);
			}

			//Decompression
			else if (strcmp("-d", argv[1]) == 0) {
				//Start the timing
				clock_t begin = clock();

				decompress(input, output);

				//Calculate the time needed to decompress
				clock_t end = clock();
				double cpu_time = (double)(end - begin) / CLOCKS_PER_SEC;
				printf("Used CPU-time for decompression: %lf", cpu_time);
			}

			else if (strcmp("-t", argv[1]) == 0) {
				//generate_files();
				//printf("Files generated\n");
				printf("Tests started.");
				test();
				printf("Tests finished\n");
			}
		}

		//Clean up
		fclose(input);
		fclose(output);
	}
	else {
		printf("3 arguments should be given in the following order: [-c or -d] [inputfile] [outputfile]");
	}
	getchar();
	return 0;
	#ifdef _MSC_VER
		_CrtDumpMemoryLeaks();
	#endif
}