#define GPT_SIGNATURE_LI 0x5452415020494645	//Little endian signiture

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

char* guid_to_string(char* guid)
{
	uint32_t hash = hash_str(guid);

	switch (hash)
	{
		case 0x2fe8f05 : 
			return "Unused entry";
		case 0x614f2df8 : 
			return "MBR partition scheme";
		case 0xbc9af79c : 
			return "EFI System partition";
		case 0x7aa4f5bb : 
			return "BIOS boot partition";
		case 0xe75532a4 : 
			return "Intel Fast Flash (iFFS) partition (for Intel Rapid Start technology)";
		case 0xa4d3651d : 
			return "Sony boot partition";
		case 0x482e9032 : 
			return "Lenovo boot partition";
		case 0xa76623d1 : 
			return "Microsoft Reserved Partition (MSR)";
		case 0x393054fc : 
			return "Basic data partition";
		case 0xb02e9c2c : 
			return "Logical Disk Manager (LDM) metadata partition";
		case 0xf43dacb5 : 
			return "Logical Disk Manager data partition";
		case 0x82459384 : 
			return "Windows Recovery Environment";
		case 0x59c2a0a1 : 
			return "IBM General Parallel File System (GPFS) partition";
		case 0xecd56645 : 
			return "Storage Spaces partition";
		case 0x22de7d10 : 
			return "Storage Replica partition";
		case 0x5538c0bd : 
			return "Data partition";
		case 0xb5fa1dfb : 
			return "Service partition";
		case 0x1453fecb : 
			return "Linux filesystem data";
		case 0xe4989372 : 
			return "RAID partition";
		case 0xa9cad14c : 
			return "Root partition (x86)";
		case 0x60fb371d : 
			return "Root partition (x86-64)";
		case 0x69043203 : 
			return "Root partition (32-bit ARM)";
		case 0x43fa07e3 : 
			return "Root partition (64-bit ARM/AArch64)";
		case 0x90e6e9d8 : 
			return "/boot partition";
		case 0xcbebdc6b : 
			return "Swap partition";
		case 0x56c27c7f : 
			return "Logical Volume Manager (LVM) partition";
		case 0x3643f35e : 
			return "/home partition";
		case 0x84c33ad2 : 
			return "/srv (server data) partition";
		case 0xf100beff : 
			return "Plain dm-crypt partition";
		case 0x28dc866 : 
			return "LUKS partition";
		case 0x59812357 : 
			return "Reserved";
		case 0x6d03d285 : 
			return "Boot partition";
		case 0x329a2cf : 
			return "BSD disklabel partition";
		case 0xaf41f5d0 : 
			return "Swap partition";
		case 0x5b5a48d1 : 
			return "Unix File System (UFS) partition";
		case 0xb38aeed3 : 
			return "Vinum volume manager partition";
		case 0xc065d9dc : 
			return "ZFS partition";
		case 0x2ccfed9d : 
			return "nandfs partition";
		case 0x901ff20c : 
			return "Hierarchical File System Plus (HFS+) partition";
		case 0x3cb43526 : 
			return "Apple APFS container | APFS FileVault volume container";
		case 0x8f304aaa : 
			return "Apple UFS container";
		case 0xd08be9cc : 
			return "ZFS";
		case 0x597ce68f : 
			return "Apple RAID partition";
		case 0x5a768f24 : 
			return "Apple RAID partition, offline";
		case 0x47616df7 : 
			return "Apple Boot partition (Recovery HD)";
		case 0xfd1c4898 : 
			return "Apple Label";
		case 0x8c6a985d : 
			return "Apple TV Recovery partition";
		case 0x360a2d7a : 
			return "Apple Core Storage Container | HFS+ FileVault volume container";
		case 0x142b8242 : 
			return "Boot partition";
		case 0xf9d07ed8 : 
			return "Root partition";
		case 0xfdfabf8c : 
			return "Swap partition";
		case 0x2255cda2 : 
			return "Backup partition";
		case 0xd08be9cc : 
			return "/usr partition";
		case 0x1a2fb81d : 
			return "/var partition";
		case 0xecb68922 : 
			return "/home partition";
		case 0x51bba096 : 
			return "Alternate sector";
		case 0xb0152a42 : 
			return "Reserved partition";
		case 0xbfbad951 : 
			return "Reserved partition";
		case 0xe11a7b2f : 
			return "Reserved partition";
		case 0x30c5d61b : 
			return "Reserved partition";
		case 0x7d862993 : 
			return "Reserved partition";
		case 0x3703bfd : 
			return "Swap partition";
		case 0x77227f4e : 
			return "FFS partition";
		case 0xef1dbba2 : 
			return "LFS partition";
		case 0xacc2e4da : 
			return "RAID partition";
		case 0xa7ceeda4 : 
			return "Concatenated partition";
		case 0x1b8130f5 : 
			return "Encrypted partition";
		case 0x36ee1e05 : 
			return "Chrome OS kernel";
		case 0x5f5e496f : 
			return "Chrome OS rootfs";
		case 0x88fbd7fe : 
			return "Chrome OS future use";
		case 0xbfe901f0 : 
			return "/usr partition (coreos-usr)";
		case 0xb1062058 : 
			return "Resizable rootfs (coreos-resize)";
		case 0xc2964f66 : 
			return "OEM customizations (coreos-reserved)";
		case 0xcfb16f22 : 
			return "Root filesystem on RAID (coreos-root-raid)";
		case 0x6a915686 : 
			return "Haiku BFS";
		case 0xc1f3f720 : 
			return "Boot partition";
		case 0x1192ab1c : 
			return "Data partition";
		case 0xbdaafe1d : 
			return "Swap partition";
		case 0x177df51e : 
			return "Unix File System (UFS) partition";
		case 0x69c3511e : 
			return "Vinum volume manager partition";
		case 0x15dba41f : 
			return "ZFS partition";
		case 0xb99c0a82 : 
			return "Journal";
		case 0x788f5eff : 
			return "dm-crypt journal";
		case 0x18d77652 : 
			return "OSD";
		case 0x7d7e2824 : 
			return "dm-crypt OSD";
		case 0x9b7c3d59 : 
			return "Disk in creation";
		case 0xaa3416a8 : 
			return "dm-crypt disk in creation";
		case 0x8f225018 : 
			return "Block";
		case 0x2c15d7b4 : 
			return "Block DB";
		case 0xd5d19ccf : 
			return "Block write-ahead log";
		case 0x34c97a1a : 
			return "Lockbox for dm-crypt keys";
		case 0xf05f2b0f : 
			return "Multipath OSD";
		case 0xd9556510 : 
			return "Multipath journal";
		case 0xaedbaaa6 : 
			return "Multipath block";
		case 0xd179004a : 
			return "Multipath block";
		case 0x7964e978 : 
			return "Multipath block DB";
		case 0x46d303c5 : 
			return "Multipath block write-ahead log";
		case 0x4e15a495 : 
			return "dm-crypt block";
		case 0xaf0596b2 : 
			return "dm-crypt block DB";
		case 0x3507f6d4 : 
			return "dm-crypt block write-ahead log";
		case 0xec9ce22d : 
			return "dm-crypt LUKS journal";
		case 0xc22327c3 : 
			return "dm-crypt LUKS block";
		case 0x963289ef : 
			return "dm-crypt LUKS block DB";
		case 0x16e6ce88 : 
			return "dm-crypt LUKS block write-ahead log";
		case 0xf18bab52 : 
			return "dm-crypt LUKS OSD";
		case 0x3edd2e53 : 
			return "Data partition";
		case 0x9bb494b3 : 
			return "Power-safe (QNX6) file system";
		case 0xd547cdf6 : 
			return "Plan 9 partition";
		case 0xd3192a93 : 
			return "vmkcore (coredump partition)";
		case 0x247f09b : 
			return "VMFS filesystem partition";
		case 0x87a8176f : 
			return "VMware Reserved";
		case 0x734d8f06 : 
			return "Bootloader";
		case 0x7489c5f8 : 
			return "Bootloader2";
		case 0xdd9e5f4 : 
			return "Boot";
		case 0x10fdeb67 : 
			return "Recovery";
		case 0xa9e6304a : 
			return "Misc";
		case 0x2b04f83c : 
			return "Metadata";
		case 0x53d23e27 : 
			return "System";
		case 0x93f9e7f7 : 
			return "Cache";
		case 0x6bac8871 : 
			return "Data";
		case 0x8b4073a9 : 
			return "Persistent";
		case 0xca779d17 : 
			return "Vendor";
		case 0x9dd2711f : 
			return "Config";
		case 0x3bb7e6f4 : 
			return "Factory";
		case 0x58efb25b : 
			return "Factory (alt)";
		case 0x52d84ba7 : 
			return "Fastboot / Tertiary";
		case 0x378769a8 : 
			return "OEM";
		case 0x64ca06dc : 
			return "Android Meta";
		case 0xa7d39e96 : 
			return "Android EXT";
		case 0x7fccfa4f : 
			return "Boot";
		case 0xce20117f : 
			return "Config";
		case 0xb33a7c1e : 
			return "PReP boot";
		case 0x90e6e9d8 : 
			return "Shared boot loader configuration";
		case 0x9b8b17fe : 
			return "Basic data partition (GEM, BGM, F32)";
		case 0x888903b1 : 
			return "Encrypted data partition";
		case 0xb5f557d5 : 
			return "ArcaOS Type 1";
		case 0x79c79f02 : 
			return "SPDK block device";
		case 0x30522da7 : 
			return "barebox-state";
		case 0x447df9f4 : 
			return "SoftRAID_Status";
		case 0x400c4f9a : 
			return "SoftRAID_Scratch";
		case 0xb666923d : 
			return "SoftRAID_Volume";
		case 0xd582d82f : 
			return "SoftRAID_Cache";
		default:
			return "Unknown Partition GUID";
	}
}

bool is_gpt_disk(uint64_t signiture)
{
	if (signiture == GPT_SIGNATURE_LI)
		return true;

	return false;
}