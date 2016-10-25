#pragma once

#include <string>
#include "E32Image.h"

namespace loader
{
	bool load(std::string path, E32Image& image);
	void parseHeader(E32Image& image);
	void parseIAT(E32Image& image);
	void parseExportDir(E32Image& image);
	void parseRelocSections(E32Image& image);
	void checkHeaderValidity(E32Image& image);

	// For checksums
	void u8_from_32(uint8_t output[4], uint32_t value);
}