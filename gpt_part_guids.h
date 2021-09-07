#define GPT_SIGNATURE_LI 0x5452415020494645	//Little endian signiture

int is_gpt_disk(int signiture)
{
	if (signiture == GPT_SIGNATURE_LI)
		return 1;

	return 0;
}