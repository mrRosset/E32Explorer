#include "Loader.h"
#include "Utils.h"
#include <iostream>
#include <fstream>

bool loader::load(std::string path, E32Image& image)
{
	std::ifstream stream(path, std::ios::binary);

	if (!stream)
	{
		std::cerr << "Failed to open image file." << std::endl;
		return false;
	}

	stream.seekg(0, std::ios::end);
	uint64_t length = stream.tellg();
	stream.seekg(0, std::ios::beg);

	image.data.resize(length);

	for (uint64_t i = 0; i < length; i++)
	{
		image.data[i] = stream.get();
	}

	stream.close();
	parseHeader(image);
	parseIAT(image);
	parseExportDir(image);
	parseRelocSections(image);

	return true;
}

void loader::u8_from_32(uint8_t output[4], uint32_t value)
{
	output[0] = (uint8_t)value;
	output[1] = (uint8_t)(value >>= 8);
	output[2] = (uint8_t)(value >>= 8);
	output[3] = (uint8_t)(value >>= 8);
}

bool loader::parseHeader(E32Image& image)
{
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	E32ImageHeader& header = image.header;

	header.uid1 = data32[0];
	header.uid2 = data32[1];
	header.uid3 = data32[2];
	header.uid_checksum = data32[3];
	header.signature = data32[4];
	header.cpu = static_cast<CPUType>(data32[5]);
	header.code_checksum = data32[6];
	header.data_checksum = data32[7];
	header.major = data32[8] & 0xFF;
	header.minor = data32[8] & 0xFF00;
	header.build = data32[8] >> 16;
	header.timestamp = ((uint64_t)data32[9] << 32) | data32[10];
	header.flags_raw = data32[11];
	header.code_size = data32[12];
	header.data_size = data32[13];
	header.heap_minimum_size = data32[14];
	header.heap_maximum_size = data32[15];
	header.stack_size = data32[16];
	header.BSS_size = data32[17];
	header.entry_point_offset = data32[18];
	header.code_base_address = data32[19];
	header.data_base_address = data32[20];
	header.dll_count = data32[21];
	header.export_offset = data32[22];
	header.export_count = data32[23];
	header.text_size = data32[24];
	header.code_offset = data32[25];
	header.data_offset = data32[26];
	header.import_offset = data32[27];
	header.code_relocation_offset = data32[28];
	header.data_relocation_offset = data32[29];
	header.priority = static_cast<ProcessPriority>(data32[30]);

	// The entry point address
	image.entry_point = header.code_offset;

	// Get the flags
	header.flags.executable_type = (header.flags_raw >> 0) & 1;
	header.flags.call_entry_point = (header.flags_raw >> 1) & 1;
	header.flags.fixed_address = (header.flags_raw >> 2) & 1;
	header.flags.abi = (header.flags_raw >> 3) & 0x3;
	header.flags.entry_point_type = (header.flags_raw >> 5) & 7;
	header.flags.header_format = (header.flags_raw >> 24) & 0xF;
	header.flags.import_format = (header.flags_raw >> 28) & 0xF;

	// TODO: Data relocation support
	if (header.data_relocation_offset != 0)
	{
		std::cerr << "Data relocation not supported" << std::endl;
		return false;
	}

	// Check the signature
	if (header.signature != 'COPE') // 'EPOC' backwards for little-endian
	{
		std::cerr << "Invalid E32Image signature." << std::endl;
		return false;
	}

	// Check the UID checksum validity
	uint8_t uids[12] = {};
	u8_from_32(uids, image.header.uid1);
	u8_from_32(uids + 4, image.header.uid2);
	u8_from_32(uids + 8, image.header.uid3);

	uint8_t even_bytes[] = { uids[0], uids[2], uids[4], uids[6], uids[8], uids[10] };
	uint8_t odd_bytes[] = { uids[1], uids[3], uids[5], uids[7], uids[9], uids[11] };

	uint32_t uid_crc = ((uint32_t)utils::crc16_ccitt(odd_bytes) << 16) | utils::crc16_ccitt(even_bytes);

	if (uid_crc != image.header.uid_checksum)
	{
		std::cerr << "UID checksum mismatch." << std::endl;
		return false;
	}


	/*// Calculate import count
	uint32_t* imports = reinterpret_cast<uint32_t*>(image.data.data() + header.code_offset + header.text_size);
	image.import_count = 0;

	while (*imports++)
	{
		image.import_count++;
	}

	if (image.header.dll_count == 0)
	{
		std::cerr << "DLL count is 0, please report this. " << image.import_count << std::endl;
		return false;
	}

	// The number of exports should always be at least 1
	if (header.export_count != 1)
	{
		std::cerr << "Export count is not 1. Report this. " <<  image.header.export_count << std::endl;
		return false;
	}

	// Don't include the header in the data
	std::vector<decltype(image.data)::value_type>(image.data.begin() + image.header.code_offset, image.data.end()).swap(image.data);
	*/
	return true;
}

bool loader::parseIAT(E32Image& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	uint32_t iat_offset = image.header.code_offset + image.header.text_size;
	uint32_t i = 0;

	uint32_t line = data32[iat_offset/4]; //divided by 4 we because we use uin32t (= bytes)
	image.code_section.import_address_table.push_back(line);

	while (line != 0) {
		i++;
		line = data32[(iat_offset/4) + i];
		image.code_section.import_address_table.push_back(line);
	}

	return true;
}


bool loader::parseExportDir(E32Image& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());

	for (uint32_t i = 0; i < image.header.export_count; i++) {
		image.code_section.export_directory.push_back(data32[(image.header.export_offset/4) + i]);
	}

	return true;
}


bool loader::parseRelocSections(E32Image& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	
	//we divide by 4 since we are using a uint32 (= 4 bytes) array, so we need to adapt the offset
	if (image.header.code_relocation_offset) {
		image.code_reloc_section.size = data32[image.header.code_relocation_offset/4];
		image.code_reloc_section.number_of_relocs = data32[(image.header.code_relocation_offset / 4) + 1];
	}
	else {
		image.code_reloc_section.size = 0;
		image.code_reloc_section.number_of_relocs = 0;
	}
	if (image.header.data_relocation_offset) {
		image.data_reloc_section.size = data32[image.header.data_relocation_offset / 4];
		image.data_reloc_section.number_of_relocs = data32[(image.header.data_relocation_offset / 4) + 1];
	}
	else {
		image.data_reloc_section.size = 0;
		image.data_reloc_section.number_of_relocs = 0;
	}

	return true;
}
