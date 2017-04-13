#include "Loader.h"
#include "Utils.h"
#include <iostream>
#include <fstream>


bool loadData(std::string path, std::vector<uint8_t>& data) {

	std::ifstream stream(path, std::ios::binary);

	if (!stream)
	{
		std::cerr << "Failed to open image file." << std::endl;
		return false;
	}

	stream.seekg(0, std::ios::end);
	uint64_t length = stream.tellg();
	stream.seekg(0, std::ios::beg);

	data.resize(length);

	for (uint64_t i = 0; i < length; i++)
	{
		data[i] = stream.get();
	}

	stream.close();
	return true;
}

bool loader::load(std::string path, E32Image& image)
{
	auto err = loadData(path, image.data);
	if (!err) {
		return false;
	}
	parseHeader(image);
	checkHeaderValidity(image);

	if (!image.valid_uid_checksum) {
		std::cerr << "Invalid ui checksum. Not a valid Symbian OS app";
		return false;
	}
	if (!image.valid_signature) {
		//std::cerr << "Not an E32Image. TRomImage are not supported";
		return false;
	}
	if (image.header->flags.header_format > 1) {
		std::cerr << "Not a basic header or J-header. V-headers are not supported";
		return false;
	}
	if (image.header->flags.import_format != 0 && image.header->flags.import_format != 2) {
		std::cerr << "ELF import section not supported";
		return false;
	}

	parseIAT(image);
	parseExportDir(image);
	parseImportSection(image);
	parseRelocSections(image);
	checkImportValidity(image);

	if (!image.valid_imports) {
		std::cerr << "Incoherence between the Import Address Table and the Import section" << std::endl;
		return false;
	}

	return true;
}

bool loader::load(std::string path, TRomImage& image) {
	auto err = loadData(path, image.data);
	if (!err) {
		return false;
	}

	parseHeader(image);
	checkHeaderValidity(image);

	return true;
}

void u8_from_32(uint8_t output[4], uint32_t value)
{
	output[0] = (uint8_t)value;
	output[1] = (uint8_t)(value >>= 8);
	output[2] = (uint8_t)(value >>= 8);
	output[3] = (uint8_t)(value >>= 8);
}

void loader::checkHeaderValidity(E32Image& image) {
	std::unique_ptr<E32ImageHeader>& header = image.header;

	// Check the signature
	image.valid_signature = header->signature == 'COPE'; // 'EPOC' backwards for little-endian

	// Check the UID checksum validity
	uint8_t uids[12] = {};
	u8_from_32(uids, image.header->uid1);
	u8_from_32(uids + 4, image.header->uid2);
	u8_from_32(uids + 8, image.header->uid3);

	uint8_t even_bytes[] = { uids[0], uids[2], uids[4], uids[6], uids[8], uids[10] };
	uint8_t odd_bytes[] = { uids[1], uids[3], uids[5], uids[7], uids[9], uids[11] };

	uint32_t uid_crc = ((uint32_t)utils::crc16_ccitt(odd_bytes) << 16) | utils::crc16_ccitt(even_bytes);

	image.valid_uid_checksum = uid_crc == image.header->uid_checksum;
}


void loader::checkHeaderValidity(TRomImage& image) {
	TRomImageHeader& header = image.header;
	// Check the UID checksum validity
	uint8_t uids[12] = {};
	u8_from_32(uids, image.header.uid1);
	u8_from_32(uids + 4, image.header.uid2);
	u8_from_32(uids + 8, image.header.uid3);

	uint8_t even_bytes[] = { uids[0], uids[2], uids[4], uids[6], uids[8], uids[10] };
	uint8_t odd_bytes[] = { uids[1], uids[3], uids[5], uids[7], uids[9], uids[11] };

	uint32_t uid_crc = ((uint32_t)utils::crc16_ccitt(odd_bytes) << 16) | utils::crc16_ccitt(even_bytes);

	image.valid_uid_checksum = uid_crc == image.header.uid_checksum;
}

void loader::parseHeader(E32Image& image)
{
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	
	//Check what header format we are using
	uint32_t flags_raw = data32[11];
	uint8_t header_format = (flags_raw >> 24) & 0xF;

	switch (header_format) {
	case 0: image.header = std::make_unique<E32ImageHeader>(); //Basic format
			break;
	case 1: image.header = std::make_unique<E32ImageHeaderJ>(); //J-format
			break;
	case 2: image.header = std::make_unique<E32ImageHeader>(); //V-format
		    break;
	default: image.header = std::make_unique<E32ImageHeader>();
			 break;
	}

	std::unique_ptr<E32ImageHeader>& header = image.header;
	
	header->uid1 = data32[0];
	header->uid2 = data32[1];
	header->uid3 = data32[2];
	header->uid_checksum = data32[3];
	header->signature = data32[4];
	header->cpu = static_cast<CPUType>(data32[5]);
	header->code_checksum = data32[6];
	header->data_checksum = data32[7];
	header->major = data32[8] & 0xFF;
	header->minor = data32[8] & 0xFF00;
	header->build = data32[8] >> 16;
	header->timestamp = ((uint64_t)data32[9] << 32) | data32[10];
	header->flags_raw = data32[11];
	header->code_size = data32[12];
	header->data_size = data32[13];
	header->heap_minimum_size = data32[14];
	header->heap_maximum_size = data32[15];
	header->stack_size = data32[16];
	header->BSS_size = data32[17];
	header->entry_point_offset = data32[18];
	header->code_base_address = data32[19];
	header->data_base_address = data32[20];
	header->dll_count = data32[21];
	header->export_offset = data32[22];
	header->export_count = data32[23];
	header->text_size = data32[24];
	header->code_offset = data32[25];
	header->data_offset = data32[26];
	header->import_offset = data32[27];
	header->code_relocation_offset = data32[28];
	header->data_relocation_offset = data32[29];
	header->priority = static_cast<ProcessPriority>(data32[30]);

	// Get the flags
	header->flags.executable_type = (header->flags_raw >> 0) & 1;
	header->flags.call_entry_point = (header->flags_raw >> 1) & 1;
	header->flags.fixed_address = (header->flags_raw >> 2) & 1;
	header->flags.abi = (header->flags_raw >> 3) & 0x3;
	header->flags.entry_point_type = (header->flags_raw >> 5) & 7;
	header->flags.header_format = (header->flags_raw >> 24) & 0xF;
	header->flags.import_format = (header->flags_raw >> 28) & 0xF;

	if (header->flags.header_format == 1) {
		//Is is possible to get a uniq_ptr to E32ImageHeaderJ ? Didn't found how
		E32ImageHeaderJ* headerJ = static_cast<E32ImageHeaderJ*>(image.header.get());
		headerJ->compression_type = data32[7];
		if (headerJ->compression_type != 0) {
			headerJ->uncompressed_size = data32[31];
		}
		else {
			/// default value to avoid non - initialized value.Not in any standard.
			headerJ->uncompressed_size = 0;
		}
	}

	// Don't include the header in the data
	//std::vector<decltype(image.data)::value_type>(image.data.begin() + image.header->code_offset, image.data.end()).swap(image.data);
}

void loader::parseHeader(TRomImage& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	TRomImageHeader& header = image.header;

	header.uid1 = data32[0]; //uint32_t
	header.uid2 = data32[1]; //uint32_t
	header.uid3 = data32[2]; //uint32_t
	header.uid_checksum = data32[3]; //uint32_t
	header.entry_point = data32[4]; //uint32_t
	header.code_address = data32[5]; //uint32_t
	header.data_address = data32[6]; //uint32_t
	header.code_size = data32[7]; //int32_t
	header.text_size = data32[8]; //int32_t
	header.data_size = data32[9]; //int32_t
	header.bss_size = data32[10]; //int32_t
	header.heap_minimum_size = data32[11]; //int32_t
	header.heap_maximum_size = data32[12]; //int32_t
	header.stack_size = data32[13]; //int32_t
	header.dll_ref_table_address = data32[14]; //uint32_t
	header.export_dir_count = data32[15]; //int32_t
	header.export_dir_address = data32[16]; //uint32_t
	header.code_checksum = data32[17]; //uint32_t
	header.data_checksum = data32[18]; //uint32_t
	header.major = data32[19]; //uint8_t
	header.minor = data32[20]; //uint8_t
	header.build = data32[21]; //uint16_t
	header.flags_raw = data32[22]; //uint32_t
	header.priority = static_cast<ProcessPriority>(data32[23]); //ProcessPriority
	header.data_bss_linear_base_address = data32[24]; //uint32_t
	header.next_extension_linear_address = data32[25]; //uint32_t
	header.harware_variant = data32[26]; //uint32_t

	//TODO parse flags
	//header.flags = data32[]; //TRomImageFlags
}

void loader::parseIAT(E32Image& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	uint32_t iat_offset = image.header->code_offset + image.header->text_size;
	uint32_t i = 0;

	uint32_t line = data32[iat_offset/4]; //divided by 4 we because we use uin32t (= bytes)
	image.code_section.import_address_table.push_back(line);

	while (line != 0) {
		i++;
		line = data32[(iat_offset/4) + i];
		image.code_section.import_address_table.push_back(line);
	}
}


void loader::parseExportDir(E32Image& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());

	for (int32_t i = 0; i < image.header->export_count; i++) {
		image.code_section.export_directory.push_back(data32[(image.header->export_offset/4) + i]);
	}
}

void loader::parseImportSection(E32Image& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());

	uint32_t offset = image.header->import_offset / 4;
	image.import_section.size = data32[offset++];
	
	for (int32_t i = 0; i < image.header->dll_count; i++) {
		std::unique_ptr<E32ImportBlock> block(new E32ImportBlock);

		block->dll_name_offset = data32[offset++];
		block->number_of_imports = data32[offset++];

		//only standard PE import format has redundancy
		if (image.header->flags.import_format == 0) {
			for (int32_t j = 0; j < block->number_of_imports; j++) {
				block->ordinals.push_back(data32[offset++]);
			}	
		}

		image.import_section.imports.push_back(std::move(block));
	}
}

void loader::parseRelocSections(E32Image& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	
	//we divide by 4 since we are using a uint32 (= 4 bytes) array, so we need to adapt the offset
	if (image.header->code_relocation_offset) {
		image.code_reloc_section.size = data32[image.header->code_relocation_offset/4];
		image.code_reloc_section.number_of_relocs = data32[(image.header->code_relocation_offset / 4) + 1];
	}
	else {
		image.code_reloc_section.size = 0;
		image.code_reloc_section.number_of_relocs = 0;
	}
	if (image.header->data_relocation_offset) {
		image.data_reloc_section.size = data32[image.header->data_relocation_offset / 4];
		image.data_reloc_section.number_of_relocs = data32[(image.header->data_relocation_offset / 4) + 1];
	}
	else {
		image.data_reloc_section.size = 0;
		image.data_reloc_section.number_of_relocs = 0;
	}
}

void loader::checkImportValidity(E32Image& image) {
	//import validity only exist when there is redundancy between iat and import section
	//(so only when import format is standard PE)
	if (image.header->flags.import_format != 0) {
		image.valid_imports = true;
		return;
	}

	//Assumption: Both list should be in the same order.
	//Since we have redundancy, I'm not sure which one the software use.
	//And there is no garuantee someone didn't mess with the one not used
	image.valid_imports = true;
	int iat_index = 0;

	for (std::unique_ptr<E32ImportBlock>& block : image.import_section.imports) {
		for (uint32_t ordinal : block->ordinals) {
			if (image.code_section.import_address_table[iat_index++] != ordinal) {
				image.valid_imports =  false;
			}
		}
	}
}
