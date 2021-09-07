#define GPT_SIGNATURE_LI 0x5452415020494645	//Little endian signiture

bool is_gpt_disk(int signiture)
{
	if (signiture == GPT_SIGNATURE_LI)
		return true;

	return false;
}