/* 
 * Listing of MBR/EBR Partition Types
 * Source: https://thestarman.pcministry.com/asm/mbr/PartTypes.htm
 */

char* part_type_to_string(int type)
{
	switch(type) {
		case 0x00 : 
			return "Empty Entry";
		case 0x01 : 
			return "12-bit FAT";
		case 0x02 : 
			return "XENIX root";
		case 0x03 : 
			return "XENIX /usr";
		case 0x04 : 
			return "16-bit FAT, partition";
		case 0x05 : 
			return "Extended Partition";
		case 0x06 : 
			return "16-bit FAT, partition";
		case 0x07 : 
			return "Installable file systems: HPFS or NTFS";
		case 0x08 : 
			return "AIX bootable partition";
		case 0x09 : 
			return "AIX data partition, AIX bootable (Linux), Coherent file system, QNX.";
		case 0x0A : 
			return "Coherent swap partition, OPUS or OS/2 Boot Manager";
		case 0x0B : 
			return "32-bit FAT";
		case 0x0C : 
			return "32-bit FAT, using INT 13 Extensions";
		case 0x0E : 
			return "16-bit FAT >= 32 MB, using INT 13 Extensions";
		case 0x0F : 
			return "Extended Partition, using INT 13 Extensions";
		case 0x10 : 
			return "OPUS";
		case 0x11 : 
			return "Hidden 12-bit FAT";
		case 0x12 : 
			return "Compaq diagnostics";
		case 0x14 : 
			return "Hidden 16-bit FAT, partition <32 MB";
		case 0x16 : 
			return "Hidden 16-bit FAT, partition >= 32 MB";
		case 0x17 : 
			return "Hidden IFS (HPFS, NTFS)";
		case 0x18 : 
			return "AST Windows swap file";
		case 0x19 : 
			return "Willowtech Photon coS";
		case 0x1B : 
			return "Hidden 32-bit FAT";
		case 0x1C : 
			return "Hidden 32-bit FAT, Ext INT 13";
		case 0x1E : 
			return "Hidden 16-bit FAT >32 MB, Ext. INT 13 (PowerQuest specific)";
		case 0x20 : 
			return "Willowsoft Overture File System (OFS1)";
		case 0x21 : 
			return "reserved (HP Volume Expansion, SpeedStor variant)";
		case 0x22 : 
			return "Oxygen Extended";
		case 0x23 : 
			return "reserved (HP Volume Expansion, SpeedStor variant?)";
		case 0x24 : 
			return "NEC MS-DOS 3.x";
		case 0x26 : 
			return "reserved (HP Volume Expansion, SpeedStor variant?)";
		case 0x31 : 
			return "reserved (HP Volume Expansion, SpeedStor variant?)";
		case 0x33 : 
			return "reserved (HP Volume Expansion, SpeedStor variant?)";
		case 0x34 : 
			return "reserved (HP Volume Expansion, SpeedStor variant?)";
		case 0x36 : 
			return "reserved (HP Volume Expansion, SpeedStor variant?)";
		case 0x38 : 
			return "Theos PowerQuest Files Partition Format";
		case 0x3D : 
			return "Hidden NetWare";
		case 0x40 : 
			return "VENIX 80286";
		case 0x41 : 
			return "Personal RISC Boot, PowerPC boot partition";
		case 0x42 : 
			return "Secure File System";
		case 0x43 : 
			return "Alternative Linux native file system (EXT2fs)";
		case 0x45 : 
			return "Priam, EUMEL/Elan.";
		case 0x46 : 
			return "EUMEL/Elan";
		case 0x47 : 
			return "EUMEL/Elan";
		case 0x48 : 
			return "EUMEL/Elan";
		case 0x4A : 
			return "ALFS/THIN lightweight filesystem for DOS";
		case 0x4D : 
			return "QNX";
		case 0x4E : 
			return "QNX";
		case 0x4F : 
			return "QNX, Oberon boot/data partition.";
		case 0x50 : 
			return "Ontrack Disk Manager, read-only partition, FAT partition (Logical sector size varies)";
		case 0x51 : 
			return "Ontrack Disk Manager, read/write partition, FAT partition (Logical sector size varies)";
		case 0x52 : 
			return "CP/M, Microport System V/386.";
		case 0x53 : 
			return "Ontrack Disk Manager, write-only";
		case 0x54 : 
			return "Ontrack Disk Manager 6.0 (DDO)";
		case 0x55 : 
			return "EZ-Drive 3.05";
		case 0x56 : 
			return "Golden Bow VFeature";
		case 0x5C : 
			return "Priam EDISK";
		case 0x61 : 
			return "Storage Dimensions SpeedStor";
		case 0x63 : 
			return "GNU HURD, Mach, MtXinu BSD 4.2 on Mach, Unix Sys V/386, 386/ix.";
		case 0x64 : 
			return "Novell NetWare 286, SpeedStore.";
		case 0x65 : 
			return "Novell NetWare (3.11 and 4.1)";
		case 0x66 : 
			return "Novell NetWare 386";
		case 0x67 : 
			return "Novell NetWare";
		case 0x68 : 
			return "Novell NetWare";
		case 0x69 : 
			return "Novell NetWare 5+; Novell Storage Services (NSS)";
		case 0x70 : 
			return "DiskSecure Multi-Boot";
		case 0x75 : 
			return "IBM PC/IX";
		case 0x80 : 
			return "Minix v1.1 - 1.4a, Old MINIX (Linux).";
		case 0x81 : 
			return "Linux/Minix v1.4b+, Mitac Advanced Disk Manager.";
		case 0x82 : 
			return "Linux Swap partition, Prime or Solaris (Unix).";
		case 0x83 : 
			return "Linux native file systems";
		case 0x84 : 
			return "OS/2 hiding type 04h partition;";
		case 0x86 : 
			return "NT Stripe Set, Volume Set";
		case 0x87 : 
			return "NT Stripe Set, Volume Set?, HPFS FT mirrored partition.";
		case 0x93 : 
			return "Amoeba file system, Hidden Linux EXT2 partition (PowerQuest).";
		case 0x94 : 
			return "Amoeba bad block table";
		case 0x99 : 
			return "Mylex EISA SCSI";
		case 0x9F : 
			return "BSDI";
		case 0xA0 : 
			return "Phoenix NoteBios Power Management ";
		case 0xA1 : 
			return "HP Volume Expansion (SpeedStor variant)";
		case 0xA3 : 
			return "HP Volume Expansion (SpeedStor variant)";
		case 0xA4 : 
			return "HP Volume Expansion (SpeedStor variant)";
		case 0xA5 : 
			return "FreeBSD/386";
		case 0xA6 : 
			return "OpenBSD";
		case 0xA7 : 
			return "NextStep Partition";
		case 0xA9 : 
			return "NetBSD";
		case 0xAA : 
			return "Olivetti DOS with FAT12";
		case 0xB0 : 
			return "Bootmanager BootStar by Star-Tools GmbH";
		case 0xB1 : 
			return "HP Volume Expansion (SpeedStor variant)";
		case 0xB3 : 
			return "HP Volume Expansion (SpeedStor variant)";
		case 0xB4 : 
			return "HP Volume Expansion (SpeedStor variant)";
		case 0xB6 : 
			return "HP Volume Expansion (SpeedStor variant)";
		case 0xB7 : 
			return "BSDI file system or secondarily swap";
		case 0xB8 : 
			return "BSDI swap partition or secondarily file system";
		case 0xBB : 
			return "PTS BootWizard (hidden) 4.0";
		case 0xBC : 
			return "May be an Acronis 'Backup' or 'Secure Zone' partition";
		case 0xBE : 
			return "Solaris boot partition";
		case 0xC0 : 
			return "Novell DOS/OpenDOS/DR-OpenDOS/DR-DOS secured";
		case 0xC1 : 
			return "DR-DOS 6.0 LOGIN.EXE-secured 12-bit FAT partition";
		case 0xC2 : 
			return "Reserved for DR-DOS 7+";
		case 0xC3 : 
			return "Reserved for DR-DOS 7+";
		case 0xC4 : 
			return "DR-DOS 6.0 LOGIN.EXE-secured 16-bit FAT partition";
		case 0xC6 : 
			return "DR-DOS 6.0 LOGIN.EXE-secured Huge partition";
		case 0xC7 : 
			return "Syrinx, Cyrnix, HPFS FT disabled mirrored partition";
		case 0xC8 : 
			return "Reserved for DR-DOS 7+";
		case 0xC9 : 
			return "Reserved for DR-DOS 7+";
		case 0xCA : 
			return "Reserved for DR-DOS 7+";
		case 0xCB : 
			return "Reserved for DR-DOS secured FAT32";
		case 0xCC : 
			return "Reserved for DR-DOS secured FAT32X (LBA)";
		case 0xCD : 
			return "Reserved for DR-DOS 7+";
		case 0xCE : 
			return "Reserved for DR-DOS secured FAT16X (LBA)";
		case 0xCF : 
			return "Reserved for DR-DOS secured Extended partition (LBA)";
		case 0xD0 : 
			return "Multiuser DOS secured (FAT12?)";
		case 0xD1 : 
			return "Old Multiuser DOS secured FAT12";
		case 0xD4 : 
			return "Old Multiuser DOS secured FAT16 (<= 32M)";
		case 0xD5 : 
			return "Old Multiuser DOS secured extended partition";
		case 0xD6 : 
			return "Old Multiuser DOS secured FAT16 (BIGDOS > 32 Mb)";
		case 0xD8 : 
			return "CP/M 86";
		case 0xDB : 
			return "CP/M";
		case 0xDE : 
			return "Dell partition. Normally it contains a FAT16 file system of about 32 MB.";
		case 0xDF : 
			return "BootIt EMBRM";
		case 0xE1 : 
			return "SpeedStor 12-bit FAT Extended partition, DOS access (Linux).";
		case 0xE2 : 
			return "DOS read-only (Florian Painke's XFDISK 1.0.4)";
		case 0xE3 : 
			return "SpeedStor (Norton, Linux says DOS R/O)";
		case 0xE4 : 
			return "SpeedStor 16-bit FAT Extended partition";
		case 0xE5 : 
			return "Tandy DOS with logical sectored FAT";
		case 0xE6 : 
			return "Storage Dimensions SpeedStor";
		case 0xEB : 
			return "BeOS file system";
		case 0xED : 
			return "Reserved for Matthias Paul's Spryt*x";
		case 0xEE : 
			return "Indicates a GPT Protective MBR followed by a GPT/EFI Header";
		case 0xEF : 
			return "EFI/UEFI System Partition (or ESP)";
		case 0xF1 : 
			return "SpeedStor Dimensions (Norton,Landis)";
		case 0xF2 : 
			return "DOS 3.3+ second partition, Unisys DOS with logical sectored FAT.";
		case 0xF3 : 
			return "Storage Dimensions SpeedStor";
		case 0xF4 : 
			return "SpeedStor Storage Dimensions (Norton,Landis)";
		case 0xF5 : 
			return "Prologue";
		case 0xF6 : 
			return "Storage Dimensions SpeedStor";
		case 0xFD : 
			return "Reserved for FreeDOS";
		case 0xFE : 
			return "LANstep, IBM PS/2 IML (Initial Microcode Load) partition";
		case 0xFF : 
			return "Xenix bad-block table";
		default:
			return "Unkown Code";
	}
}

int is_extended(int type)
{
	switch (type) {
	case 0xD5 : 
	case 0x05 : 
	case 0x0F : 
	case 0x22 : 
	case 0xCF : 
	case 0xE1 : 
	case 0xE4 :
	       return 1;
	default:
		return 0;
	}		
}
