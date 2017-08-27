#pragma once
#include <stdint.h>
#include <vector>
#include "E32Std.h"

typedef int64_t TTime;
typedef uint32_t TLinAddr;

struct TVersion {
	uint8_t Major;
	uint8_t Minor;
	uint16_t Build;
};

struct TScreenSize {
	uint32_t Width;
	uint32_t Height;
};

struct TRomHeader {
	uint8_t jump_table[128];
	uint8_t major;
	uint8_t minor;
	uint16_t build;
	int64_t time;
	TLinAddr rom_base_address;
	uint32_t rom_size;
	TLinAddr rom_root_directory_list;
	TLinAddr kern_data_address;
	TLinAddr kern_stack_address;
	TLinAddr primary_file;
	TLinAddr secondary_file;
	uint32_t checksum;
	int64_t language;
	uint32_t hardware;
	uint32_t screen_width;
	uint32_t screen_height;
	int32_t screen_bits_per_pixel;
	TLinAddr rom_section_header;
	int32_t total_sv_data_size;
	TLinAddr variant_file;
	TLinAddr extension_file;
	TLinAddr reloc_info;
	uint32_t trace_mask;
	TLinAddr user_data_address;
	int32_t total_user_data_size;
};

struct TRootDirInfo {
	uint32_t iHardwareVariant;
	TLinAddr iAddressLin;
};

struct TRomRootDirectoryList
{
	int32_t iNumRootDirs;
	TRootDirInfo iRootDir[1];
};

struct TRomSectionHeader{
	TVersion iVersion;
	TTime iTime;
	int64_t iLanguage;
	uint32_t iCheckSum;
};

struct TRomEntry {
public:
	int32_t iSize;
	uint32_t iAddressLin;
	uint8_t iAtt;
	uint8_t iNameLength;
	uint8_t iName[2];
};
const int32_t KRomEntrySize = (sizeof(TRomEntry) - 2);

class TRomDir
{
public:
	int32_t iSize;
	TRomEntry iEntry;
};

//
// Header for extension ROMs
// Equivalent to TRomHeader
//
class TExtensionRomHeader
{
public:
	TVersion iVersion;
	int64_t iTime;
	TLinAddr iRomBase;
	uint32_t iRomSize;
	TLinAddr iRomRootDirectoryList;
	uint32_t iCheckSum;
	//
	TVersion iKernelVersion;
	int64_t iKernelTime;
	uint32_t iKernelCheckSum;
	uint32_t iPad[32 - 11]; // sizeof(TExtensionRomHeader)=128
};


struct TRom {
	std::vector<uint8_t> data;

	TRomHeader header;
};