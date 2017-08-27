#include <iostream>
#include <memory.h>

#include "TRomLoader.h"
#include "..\Utils.h"
#include "..\TRom.h"

bool TRomLoader::parse(std::string path, TRom& image) {
	auto err = utils::loadData(path, image.data);
	if (!err) {
		return false;
	}

	parseHeader(image);
	//TODO: Check checksum
	parseRomDirectories(image);

	return true;
}

void TRomLoader::parseHeader(TRom& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	TRomHeader& header = image.header;

	memcpy(header.jump_table, image.data.data(), 128); //uint8_t
	header.major = data32[32] & 0xFF; //TVersion
	header.minor = data32[32] & 0xFF00;
	header.build = data32[32] >> 16;
	header.time = ((int64_t)data32[33] << 32) | data32[34]; //int64_t
	header.rom_base_address = data32[35]; //TLinAddr
	header.rom_size = data32[36]; //uint32_t
	header.rom_root_directory_list = data32[37]; //TLinAddr
	header.kern_data_address = data32[38]; //TLinAddr
	header.kern_stack_address = data32[39]; //TLinAddr
	header.primary_file = data32[40]; //TLinAddr
	header.secondary_file = data32[41]; //TLinAddr
	header.checksum = data32[42]; //uint32_t
	header.language = ((int64_t)data32[43] << 32) | data32[44]; //int64_t
	header.hardware = data32[45]; //uint32_t
	header.screen_width = data32[46]; //TScreenSize
	header.screen_height = data32[47];
	header.screen_bits_per_pixel = data32[48]; //int32_t
	header.rom_section_header = data32[49]; //TLinAddr
	header.total_sv_data_size = data32[50]; //int32_t
	header.variant_file = data32[51]; //TLinAddr
	header.extension_file = data32[52]; //TLinAddr
	header.reloc_info = data32[53]; //TLinAddr
	header.trace_mask = data32[54]; //uint32_t
	header.user_data_address = data32[55]; //TLinAddr
	header.total_user_data_size = data32[56]; //int32_t
}

void TRomLoader::parseRomDirectories(TRom& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	TRomRootDirectoryList& directory_list = image.directory_list;
	uint32_t dir_list_address = (image.header.rom_root_directory_list - 0x5000'0000) / 4;

	directory_list.num_root_dir = data32[dir_list_address];
	if (directory_list.num_root_dir != 1) {
		std::cerr << "Insupported number of root directories: " << directory_list.num_root_dir << std::endl;
		return;
	}

	directory_list.root_dir.hardware_variant = data32[dir_list_address + 1];
	directory_list.root_dir.address_lin = data32[dir_list_address + 2];
}
