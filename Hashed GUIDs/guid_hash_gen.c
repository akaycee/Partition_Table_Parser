#include <stdio.h>
#include <stdint.h>

// djb2 Hashing Algorithm
// Copied from: http://www.cse.yorku.ca/~oz/hash.html
uint32_t hash_str(unsigned char *str)
{
	uint32_t hash = 0x1505;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

int main()
{
	char* GUID = "024DEE41-33E7-11D3-9D69-0008C781F39F";
	uint32_t hash = hash_str(GUID);
	printf("GUID: %s\n", GUID);
	printf("Hash: %x\n", hash);
}