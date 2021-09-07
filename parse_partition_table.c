#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "mbr_part_ids.h"
#include "gpt_part_guids.h"

/* Master Boot Record Structure:
 * https://en.wikipedia.org/wiki/Master_boot_record */

#define BLK_SIZE		512	/* Default value */
#define MAX_partition_table_RECORDS	4	/* Max partition table entries */
#define MAX_ETABLE_RECORDS	2	/* Max extended table entries */

typedef struct mbr_timestamp {
	uint16_t reserve;	/* 0x0000 */
	uint8_t opd;		/* Original physical drive (0x80–0xFF) */
	uint8_t sec;		/* Seconds (0–59) */
	uint8_t min;		/* Minutes (0–59) */
	uint8_t hours;		/* Hours (0–23) */
} mbr_timestamp;

typedef struct mbr_entry {
	uint8_t status;		/* Status or physical drive */
	uint8_t chs_1[3];	/* CHS address of first absolute sector in partition */
	uint8_t part_type;	/* specifies the file system the partition contains */
	uint8_t chs_2[3];	/* CHS address of last absolute sector in partition */
	uint32_t lba_as;	/* LBA of first absolute sector in the partition */
	uint32_t no_sectors;	/* Number of sectors in partition */
} mbr_entry;

#pragma pack(2)			/* Allign the members of the structures in 1 Byte fields*/
typedef struct mbr_bootstrap {
	uint8_t bca1[218];			/* Bootstrap code area (part 1) */
	struct mbr_timestamp timestamp;		/* Optional */
	uint8_t bca2[216];			/* Bootstrap code area (part 2) (TODO: can be 222, not sure when) */
	uint32_t dsign;				/* 32-bit disk signature */
	uint16_t protected;			/* 0x0000 (0x5A5A if copy-protected) */
} mbr_bootstrap;

typedef struct mbr {
	struct mbr_bootstrap bc;		/* Bootstrap Code area */
	struct mbr_entry partition_table[4];	/* Partition table (for primary partitions) */
	uint16_t b_sign;			/* Boot Signature (0x55AA) */
} mbr;
#pragma pack()

// Format defined by:
// https://en.wikipedia.org/wiki/GUID_Partition_Table#Partition_table_header_(LBA_1)
typedef struct gpt_entry {
	guid part_guid;
	guid unique_guid;
	uint64_t first_lba;
	uint64_t last_lba;
	uint64_t flags;
	uint8_t part_name[72];
} gpt_entry;

typedef struct gpt_header {
	uint64_t signature;
	uint32_t revision;
	uint32_t header_size;
	uint32_t header_crc32;
	uint32_t reserved1;
	uint64_t current_lba;
	uint64_t backup_lba;
	uint64_t first_usable_lba;
	uint64_t last_usable_lba;
	guid disk_guid;
	uint64_t part_start_lba;
	uint32_t num_parts;
	uint32_t part_size;
	uint32_t part_arr_crc32;
	uint8_t reserved2[420];
} gpt_header;

typedef struct gpt {
	struct gpt_header header;
	struct gpt_entry entries[128];
} gpt;

/* sh_bytes - Converts bytes to Short Hand Notation 
 * 
 * Parameters:
 * 1) long long t_bytes	<INPUT>	 : Total number of bytes
 * 2) char *bytes	<OUTPUT> : Bytes in shorthand notation
 * 
 * Return: void
 */

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

/* open_file - Opens a file from the filename
 *
 * Parameters:
 * 1) char *filename	<INPUT>  : The name of the file
 * 
 * Return: NULL if there is an error, a pointer to the opened file otherwise.
 */

FILE* open_file(char *filename)
{
	FILE* file;
	file = fopen(filename, "r");

	if (file == NULL) {
		printf("Error Opening %s: %d\n", filename, errno);
		return NULL;
	}

	return file;
}

/* read_mbr - Get the master boot record from the disk image file
 *
 * Parameters:
 * 1) char *filename	<INPUT>  : The Name of the disk image file
 * 2) FILE *img		<INPUT>  : A pointer to the img file
 * 3) mbr *record	<OUTPUT> : The struct to read the MBR into
 * 
 * Return: -1 if there is an error, 0 otherwise.
 */

int read_mbr(char *filename, FILE *img, mbr *record)
{
	unsigned int size = 0;

	if ((size = fread(record, 1, sizeof(mbr), img)) < 1) {
		printf("Error reading from %s! [%d] size = %lu\n", filename, size, sizeof(mbr));
		return -1;
	}

	return 0;
}

/* print_mbr_overview - Prints the information of an MBR disk
 *
 * Parameters:
 * 1) mbr *record 	<INPUT> : Master Boot Record Structure
 * 2) char *filename	<INPUT> : Name of the disk image file
 * 
 * Return: void
 */

void print_mbr_overview (mbr *record, char *filename)
{
	long total_sectors = 0;
	long long total_bytes = 0;
	char size_in_bytes[64];
	int i = 0;

	total_sectors = record->partition_table[0].lba_as;				/* Initialise total_sector with the first absolute sector */
	for (i = 0; i < 4; ++i) total_sectors +=  record->partition_table[i].no_sectors;
	total_bytes = total_sectors * BLK_SIZE;
	sh_bytes(total_bytes, size_in_bytes);

	printf("Disk %s: %s, %llu bytes, %lu sectors\n", filename, size_in_bytes, total_bytes, total_sectors);
	printf("Disk identifier: 0x%0x\n", record->bc.dsign);
}

/* print_ebr_table - Prints the partition info of one EBR
 *
 * Parameters:
 * 1) mbr *record 		<INPUT>  : Master Boot Record structure
 * 2) char *filename		<INPUT>  : Name of the disk image file
 * 3) long long *ebr_address	<OUTPUT> : The relative address of the next EBR if it exists, 0 otherwise
 * 4) int partition_number	<INPUT> : The number of the next partition
 * 
 * Return: void
 */

void print_ebr_table(mbr *record, char *filename, long long *ebr_offset, int partition_number)
{
	char size_in_bytes[64];
	long total_sectors = 0, start = 0, end = 0, sectors = 0;
	int type = 0;

	sectors = record->partition_table[0].no_sectors;
	
	// Print the info of the logical partition
	if (0 != sectors)
	{
		start = record->partition_table[0].lba_as;
		end = start + sectors - 1;
		total_sectors += sectors;
		type = record->partition_table[0].part_type;

		sh_bytes((sectors * BLK_SIZE), size_in_bytes);

		printf("%s%d  ", filename, partition_number);
		printf("%c\t", record->partition_table[0].status? '*':' ' );
		printf("%lu\t%lu\t%lu\t\t", start, end, sectors);
		printf("%s\t", size_in_bytes);
		printf("%0x\t%s\n", type, part_type_to_string(type));
	}

	// Store the offset of the next EBR
	if (record->partition_table[1].no_sectors == 0)
		*ebr_offset = 0;
	else
		*ebr_offset = record->partition_table[1].lba_as * BLK_SIZE;
}

/* print_extended_partitions - Prints the partition info all EBR partitions on the disk
 *
 * Parameters:
 * 1) mbr *record 		<INPUT> : Master Boot Record structure
 * 2) char *filename		<INPUT> : Name of the disk image file
 * 3) FILE *img			<INPUT> : A pointer to the img file
 * 4) long long ebr_address	<INPUT> : The absolute address of the first EBR table
 * 5) int partition_number	<INPUT> : The number of the next partition
 * 
 * Return: void
 */

void print_extended_partitions(mbr *record, char *filename, FILE* img, long long ebr_address, int partition_number)
{
	unsigned int size = 0;
	long long ebr_offset = 0;

	do
	{
		if ((size = fseek(img, ebr_address + ebr_offset, SEEK_SET)) < 0) {
			printf("Error seeking file %s! [%d] size = %lu\n", filename, size, sizeof(mbr));
			return;
		} else if ((size = fread(record, 1, sizeof(struct mbr), img)) < 1) {
			printf("Error reading from %s! [%d] size = %lu\n", filename, size, sizeof(mbr));
			return;
		}

		print_ebr_table(record, filename, &ebr_offset, partition_number);
		partition_number += 1;
	} while (ebr_offset);
}


/* print_mbr_table - Prints the partition info of an MBR entry
 *
 * Parameters:
 * 1) mbr_entry *entry 		<INPUT>  : MBR partition entry struct
 * 2) char *filename		<INPUT>  : Name of the disk image file
 * 3) int part_num		<INPUT>  : The number of the partition
 * 
 * Return: EBR address if it exists, 0 otherwise
 */

int  print_mbr_entry(mbr_entry *entry, char *filename, int part_num)
{
	long long ebr_address = 0;
	char size_in_bytes[64];
	long total_sectors = 0, start = 0, end = 0, sectors = 0;
	int type = 0;

	sectors = entry->no_sectors;
	
	if (0 == sectors)
		return 0;
	
	start = entry->lba_as;
	end = start + sectors - 1;
	total_sectors += sectors;
	type = entry->part_type;
	
	if (is_extended(type))
		ebr_address = (start * BLK_SIZE);

	sh_bytes((sectors * BLK_SIZE), size_in_bytes);

	printf("%s%d  ", filename, part_num);
	printf("%c\t", entry->status ? '*':' ' );
	printf("%lu\t%lu\t%lu\t\t", start, end, sectors);
	printf("%s\t", size_in_bytes);
	printf("%0x\t%s\n", type, part_type_to_string(type));

	return ebr_address;
}

/* print_mbr_table - Prints the partition info of an MBR
 *
 * Parameters:
 * 1) mbr *record 		<INPUT>  : Master Boot Record struct
 * 2) char *filename		<INPUT>  : Name of the disk image file
 * 3) FILE *img			<INPUT>  : A pointer to the img file
 * 
 * Return: void
 */

void print_mbr_table(mbr *record, char *filename, FILE* img)
{
	int i = 0;
	long long ebr_address = 0;

	print_mbr_overview(record, filename);
	printf("\nDevice%*s\tStart\tEnd\tSectors\t\tSize\tID\tType\n", (int) strlen(filename), "Boot");
	
	for (i = 0; i < 4; ++i) {
		mbr_entry *entry = &(record->partition_table[i]);
		long long result = print_mbr_entry(entry, filename, i + 1);

		if (result != 0)
			ebr_address = result;
	}

	if (ebr_address != 0)
		print_extended_partitions(record, filename, img, ebr_address, i + 1);
}

/* read_gpt - Get the GUID partition table from the disk image file
 *
 * Parameters:
 * 1) char *filename	<INPUT>  : The Name of the disk image file
 * 2) FILE *img		<INPUT>  : A pointer to the img file
 * 3) gpt* table	<OUTPUT> : The struct to read the GPT into
 * 
 * Return: -1 if there is an error, 0 otherwise.
 */

int read_gpt(char *filename, FILE *img, gpt *table)
{
	unsigned int size = 0;

	// Seek to the Partition Table Header (LBA 1)
	if ((size = fseek(img, BLK_SIZE, SEEK_SET)) < 0) {
		printf("Error seeking file %s! [%d] size = %lu\n", filename, size, sizeof(gpt));
		return -1;
	}

	// Read the header
	if ((size = fread(table, 1, sizeof(gpt), img)) < 1) {
		printf("Error reading from %s! [%d] size = %lu\n", filename, size, sizeof(gpt));
		return -1;
	}

	return 0;
}

/* print_gpt_overview - Prints the information of a GPT disk
 *
 * Parameters:
 * 1) gpt *table 	<INPUT> : GUID Partition Table Structure
 * 2) char *filename	<INPUT> : Name of the disk image file
 * 
 * Return: void
 */

void print_gpt_overview(gpt *table, char *filename)
{
	long total_sectors = 0;
	long long total_bytes = 0;
	char size_in_bytes[64];
	gpt_header *header = &table->header;

	// Initialise total_sector with the starting sector of the first partition
	total_sectors = header->part_start_lba + ((header->num_parts * header->part_size) / BLK_SIZE);

	for (int i = 0; i < header->num_parts; ++i)
	{
		long num_sectors = table->entries[i].last_lba - table->entries[i].first_lba;
		total_sectors += num_sectors;
	}

	total_bytes = total_sectors * BLK_SIZE;
	sh_bytes(total_bytes, size_in_bytes);

	printf("Disk %s: %s, %llu bytes, %lu sectors\n", filename, size_in_bytes, total_bytes, total_sectors);
	//Print GUID
}

/* print_gpt_table - Prints the partition info of a GPT
 *
 * Parameters:
 * 1) gpt *table 		<INPUT>  : Master Boot Record structure
 * 2) char *filename		<INPUT>  : Name of the disk image file
 * 
 * Return: void
 */

void print_gpt_table(gpt *table, char *filename)
{
	print_gpt_overview(table, filename);
	printf("\nDevice%*s\tStart\tEnd\tSectors\t\tSize\tID\tType\n", (int) strlen(filename), "Boot");
	
}

/* print_partitions - Prints the partitions of MBR and GPT disks
 *
 * Parameters:
 * 1) char *filename	<INPUT> : The Name of the disk image file
 *
 * Return: -1 if there is an error, 0 otherwise.
 */

int print_partitions(char *filename)
{
	mbr m1;
	uint8_t first_part_type;
	FILE *img = open_file(filename);

	if (img == NULL)
		return -1;

	if (read_mbr(filename, img, &m1))
		return -1;

	first_part_type = m1.partition_table[0].part_type;

	// MBR disk
	if (!is_protective_gpt(first_part_type)) {
		print_mbr_table(&m1, filename, img);
	}
	// GPT disk
	else {
		gpt g1;
		read_gpt(filename, img, &g1);
		print_gpt_table(&g1, filename);
	}

	return 0;
}

int main(int argc, char ** argv)
{
	if (argc < 2) {
		printf("Usage: %s <path_to_os_img>\n", argv[0]);
		return -1;
	}

	return print_partitions(argv[1]);
}
