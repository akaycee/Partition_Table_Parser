#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define GUID_LENGTH 36

bool is_hex(char character)
{
	if (character >= '0' && character <= '9')
		return true;
	else if (character >= 'A' && character <= 'F')
		return true;
	else if (character >= 'a' && character <= 'f')
		return true;
	else
		return false;
}

// djb2 Hashing Algorithm
// Copied from: http://www.cse.yorku.ca/~oz/hash.html
uint32_t hash_str(char *str)
{
	uint32_t hash = 0x1505;
	int c;

	while (c = *str++) {
		// Ignore any non-hex characters
		if (is_hex(c))
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}

int hash_str_file(char* input_path, char* output_path)
{
	FILE* guid_file;
	FILE* hash_file;
	char* guid = malloc(GUID_LENGTH + 2);
	uint32_t hash;

	guid_file = fopen(input_path, "r");

	if (guid_file == NULL) {
		printf("Error Opening %s: %d\n", input_path, errno);
		return -1;
	}

	hash_file = fopen(output_path, "w");

	if (hash_file == NULL) {
		printf("Error Opening %s: %d\n", output_path, errno);
		return -1;
	}

	while (fgets(guid, GUID_LENGTH + 2, guid_file) != NULL) {
		// Skip if only a newline was read
		if (guid[0] == '\n')
			continue;

		hash = hash_str(guid);
		fprintf(hash_file, "%x\n", hash);
	}

	fclose(guid_file);
	fclose(hash_file);

	return 0;
}

int main(int argc, char ** argv)
{
	if (argc < 3) {
		printf("Usage: %s <path_to_guid_list> <path_to_hash_output>\n", argv[0]);
		return -1;
	}

	int result = hash_str_file(argv[1], argv[2]);

	if (result == 0)
		printf("Successfully hashed %s\n", argv[1]);

	return result;
}