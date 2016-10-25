#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

// Sources:
// For a short overview: https://web.archive.org/web/20120813204404/http://www.antonypranata.com/node/10
// For a more detailed explanation, see "Symbian OS Internals" chapter 10 and Appendix 2 and 3

enum class CPUType : uint32_t {
	x86 = 0x1000,
	Arm = 0x2000,
	Mcore = 0x4000
};

enum class ProcessPriority
{
	Low = 150,
	Background = 250,
	Foreground = 350,
	High = 450,
	WindowServer = 650,
	FileServer = 750,
	RealTimeServer = 850,
	Supervisor = 950
};

struct E32CodeSection
{
	//Text section
	//Should the data be copied here ? Or just let the offset from the header.

	//Assumption: We don't have negative import ordinal.
	//Otherwise use int32_t.
	std::vector<uint32_t> import_address_table;
	
	std::vector<uint32_t> export_directory;
};

struct E32ImportBlock
{
	uint32_t dll_name_offset; //relative to the import section // offset of name of dll importing from
	int32_t number_of_imports;		// number of imports from this dll
	std::vector<uint32_t> ordinals;	// TUint32 iImport[iNumberOfImports];
};

struct E32ImportSection
{
	int32_t size;			    // size of this section
	std::vector<std::unique_ptr<E32ImportBlock>> imports;	// E32ImportBlock[iDllRefTableCount];
};

struct E32RelocSection
{
	int32_t size;
	int32_t number_of_relocs;
};


//Structure created to contains the flags in more easily understandble way
//Not representative of what is in the on-disk file.
struct E32Flags
{
	bool executable_type;  // false = executable, true = DLL
	bool call_entry_point; // false = call, true = don't call
	bool fixed_address;    // 0 = not fixed address, 1 = fixed address
	uint8_t abi;                // 0 = GCC98r2, 1 = EABI
	uint8_t entry_point_type;   // 0 = EKA1, 1 = EKA2
	uint8_t header_format;      // 0 = Basic, 1 = J-format, 2 = V-format
	uint8_t import_format;      // 0 = Standard PE format, 1 = ELF format, 2 = PE format with some other custom shit
};

struct E32ImageHeader {
	uint32_t uid1;
	uint32_t uid2;
	uint32_t uid3;
	uint32_t uid_checksum;
	uint32_t signature;
	CPUType cpu;
	uint32_t code_checksum;
	uint32_t data_checksum;
	uint8_t major;
	uint8_t minor;
	uint16_t build;
	int64_t timestamp;
	uint32_t flags_raw;
	E32Flags flags;
	int32_t code_size;
	int32_t data_size;
	int32_t heap_minimum_size;
	int32_t heap_maximum_size;
	int32_t stack_size;
	int32_t BSS_size;
	uint32_t entry_point_offset;
	uint32_t code_base_address;
	uint32_t data_base_address;
	int32_t dll_count;
	uint32_t export_offset;
	int32_t export_count;
	int32_t text_size;
	uint32_t code_offset;
	uint32_t data_offset;
	uint32_t import_offset;
	uint32_t code_relocation_offset;
	uint32_t data_relocation_offset;
	ProcessPriority priority;
};

struct E32Image {
	std::vector<uint8_t> data;
	bool valid_uid_checksum = false;
	bool valid_signature = false;

	//uint32_t import_count;

	E32ImageHeader header;
	E32CodeSection code_section; //Contains text section, import address table and export directory
	//BSS section <- usually empty
	//Data section  <- usually empty
	E32ImportSection import_section;
	E32RelocSection code_reloc_section;
	E32RelocSection data_reloc_section;
};