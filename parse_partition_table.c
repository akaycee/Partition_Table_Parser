#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "part_types.h"

/* Master Boot Record Structure:
 * https://en.wikipedia.org/wiki/Master_boot_record */

#define BLK_SIZE		512	/* Default value */
#define MAX_partition_table_RECORDS	4	/* Max partition table entries */
#define MAX_ETABLE_RECORDS	2	/* Max extended table entries */

struct disk_tstamp {
	uint16_t reserve;	/* 0x0000 */
	uint8_t opd;		/* Original physical drive (0x80–0xFF) */
	uint8_t sec;		/* Seconds (0–59) */
	uint8_t min;		/* Minutes (0–59) */
	uint8_t hours;		/* Hours (0–23) */
};

struct partition_entry {
	uint8_t status;		/* Status or physical drive */
	uint8_t chs_1[3];	/* CHS address of first absolute sector in partition */
	uint8_t part_type;	/* specifies the file system the partition contains */
	uint8_t chs_2[3];	/* CHS address of last absolute sector in partition */
	uint32_t lba_as;	/* LBA of first absolute sector in the partition */
	uint32_t no_sectors;	/* Number of sectors in partition */
};

#pragma pack(2)			/* Allign the members of the structures in 1 Byte fields*/
struct bootstrap_code_area {
	uint8_t bca1[218];			/* Bootstrap code area (part 1) */
	struct disk_tstamp timestamp;		/* Optional */
	uint8_t bca2[216];			/* Bootstrap code area (part 2) (TODO: can be 222, not sure when) */
	uint32_t dsign;				/* 32-bit disk signature */
	uint16_t protected;			/* 0x0000 (0x5A5A if copy-protected) */
};

struct mbr {
	struct bootstrap_code_area bc;		/* Bootstrap Code area */
	struct partition_entry partition_table[4];	/* Partition table (for primary partitions) */
	uint16_t b_sign;			/* Boot Signature (0x55AA) */
};
#pragma pack()

/* sh_bytes - Converts bytes to Short Hand Notation 
 * 
 * Parameters:
 * 1) long long t_bytes	<INPUT>	 : Total number of bytes
 * 2) char *bytes	<OUTPUT> : Bytes in shorthand notation
 * 
 * Return: void
 */

FILE *img = NULL;

void sh_bytes (long long t_bytes, char *bytes)
{
	double total_bytes = t_bytes;
	uint8_t flag = 0;
	char size = ' ';
	
	while (total_bytes > 1024) {
		total_bytes /= 1024;
		flag++;
	}

	if (flag == 1) {
		size = 'K';
	} else if (flag == 2) {
		size = 'M';
	} else if (flag == 3) {
		size = 'G';
	} else if (flag == 4) {
		size = 'T';
	}

	sprintf(bytes, "%.3g%cB", total_bytes, size);
}

/* read_mbr - Get the master boot record from the img file
 *
 * Parameters:
 * 1) char *filename	<INPUT> : The name of the img file
 * 2) struct mbr record	<INPUT>	: The master boot record
 * 
 * Return: -1 if there is an error, 0 otherwise.
 */

int read_mbr(char *filename, struct mbr *record)
{
	unsigned int size = 0;
	
	if (NULL == (img = fopen(filename, "r"))) {
		printf("Error Opening %s: %d", filename, errno);
		return -1;
	}

	if ((size = fread(record, 1, sizeof(struct mbr), img)) < 1) {
		printf("Error reading from %s! [%d] size = %lu\n", filename, size, sizeof(struct mbr));
		return -1;
	}
	return 0;

}

int is_gpt_disk(struct mbr record)
{
	int part_type = record.partition_table[0].part_type;

	if (is_gpt(part_type))
		return 1;

	return 0;
}

/* print_overview - Prints the number of sectors and total no. of bytes
 *
 * Parameters:
 * 1) struct mbr 	<INPUT> : Master Boot Record Structure
 * 2) char *filename	<INPUT> : Name of the img file
 * 
 * Return: void
 */

void print_overview (struct mbr record, char *filename)
{
	long total_sectors = 0;
	long long total_bytes = 0;
	char size_in_bytes[64];
	int i = 0;

	total_sectors = record.partition_table[0].lba_as;				/* Initialise total_sector with the first absolute sector */
	for (i = 0; i < 4; ++i) total_sectors +=  record.partition_table[i].no_sectors;
	total_bytes = total_sectors * BLK_SIZE;
	sh_bytes(total_bytes, size_in_bytes);

	printf("Disk %s: %s, %llu bytes, %lu sectors\n", filename, size_in_bytes, total_bytes, total_sectors);
	printf("Disk identifier: 0x%0x\n", record.bc.dsign);
}

/* print_ebr_table - Prints the partition info of one EBR
 *
 * Parameters:
 * 1) struct mbr 		<INPUT>  : Master Boot Record structure
 * 2) char *filename		<INPUT>  : Name of the img file
 * 3) long long *ebr_address	<OUTPUT> : The relative address of the next EBR if it exists, 0 otherwise
 * 
 * Return: void
 */

void print_ebr_table(struct mbr record, char *filename, long long *ebr_offset)
{
	char size_in_bytes[64];
	long total_sectors = 0, start = 0, end = 0, sectors = 0;
	int type = 0;

	sectors = record.partition_table[0].no_sectors;
	
	// Print the info of the logical partition
	if (0 != sectors)
	{
		start = record.partition_table[0].lba_as;
		end = start + sectors - 1;
		total_sectors += sectors;
		type = record.partition_table[0].part_type;

		sh_bytes((sectors * BLK_SIZE), size_in_bytes);

		printf("%s%d  ", filename, 1);
		printf("%c\t", record.partition_table[0].status? '*':' ' );
		printf("%lu\t%lu\t%lu\t\t", start, end, sectors);
		printf("%s\t", size_in_bytes);
		printf("%0x\t%s\n", type, part_type_to_string(type));
	}

	// Store the offset of the next EBR
	if(record.partition_table[1].no_sectors == 0)
	{
		*ebr_offset = 0;
	}
	else
	{
		*ebr_offset = record.partition_table[1].lba_as * BLK_SIZE;
	}
}

/* print_extended_partitions - Prints the partition info all EBR partitions on the disk
 *
 * Parameters:
 * 1) struct mbr 		<INPUT> : Master Boot Record structure
 * 2) char *filename		<INPUT> : Name of the img file
 * 3) long long ebr_address	<INPUT> : The absolute address of the first EBR table
 * 
 * Return: void
 */

void print_extended_partitions(struct mbr record, char *filename, long long ebr_address)
{
	unsigned int size = 0;
	long long ebr_offset = 0;

	do
	{
		if ((size = fseek(img, ebr_address + ebr_offset, SEEK_SET)) < 0) {
			printf("Error seeking file %s! [%d] size = %lu\n", filename, size, sizeof(struct mbr));
			return;
		} else if ((size = fread(&record, 1, sizeof(struct mbr), img)) < 1) {
			printf("Error reading from %s! [%d] size = %lu\n", filename, size, sizeof(struct mbr));
			return;
		}

		print_ebr_table(record, filename, &ebr_offset);
	} while (ebr_offset);
}

/* print_mbr_table - Prints the partition info of an MBR
 *
 * Parameters:
 * 1) struct mbr 		<INPUT>  : Master Boot Record structure
 * 2) char *filename		<INPUT>  : Name of the img file
 * 
 * Return: void
 */

void print_mbr_table(struct mbr record, char *filename)
{
	char size_in_bytes[64];
	long total_sectors = 0, start = 0, end = 0, sectors = 0;
	int type = 0;
	long long ebr_address = 0;

	printf("\nDevice%*s\tStart\tEnd\tSectors\t\tSize\tID\tType\n", (int) strlen(filename), "Boot");
	
	for (int i = 0; i < 4; ++i) {
		sectors = record.partition_table[i].no_sectors;
		
		if (0 == sectors)
			continue;
		
		start = record.partition_table[i].lba_as;
		end = start + sectors - 1;
		total_sectors += sectors;
		type = record.partition_table[i].part_type;
		
		if (is_extended(type))
			ebr_address = (start * BLK_SIZE);

		sh_bytes((sectors * BLK_SIZE), size_in_bytes);

		printf("%s%d  ", filename, i + 1);
		printf("%c\t", record.partition_table[i].status? '*':' ' );
		printf("%lu\t%lu\t%lu\t\t", start, end, sectors);
		printf("%s\t", size_in_bytes);
		printf("%0x\t%s\n", type, part_type_to_string(type));
	}

	if (ebr_address != 0)
		print_extended_partitions(record, filename, ebr_address);
}

/* print_partitions - Prints the partitions on the disk
 *
 * Parameters:
 * 1) struct mbr record	<INPUT>	: The master boot record with the partition table
 * 2) char *filename	<INPUT> : The name of the img file
 *
 * Return: void
 */

void print_partitions(struct mbr record, char *filename)
{
	if (!is_gpt_disk(record))
		print_mbr_table(record, filename);
	else
		printf("\nThe GPT format is not supported yet\n");
}

int main(int argc, char ** argv)
{
	struct mbr m1;

	if (argc < 2) {
		printf("Usage: %s <path_to_os_img>\n", argv[0]);
		return -1;
	}

	if (read_mbr(argv[1], &m1)) {
		return -1;
	}

	print_overview(m1, argv[1]);
	print_partitions(m1, argv[1]);

	return 0;
}
