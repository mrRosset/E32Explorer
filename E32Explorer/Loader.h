#pragma once

#include <string>
#include "E32Image.h"
#include "TRomImage.h"

namespace loader
{
	bool load(std::string path, E32Image& image);
	bool load(std::string path, TRomImage& image);
	void parseHeader(E32Image& image);
	void parseHeader(TRomImage& image);
	void parseIAT(E32Image& image);
	void parseExportDir(E32Image& image);
	void parseImportSection(E32Image& image);
	void parseRelocSections(E32Image& image);
	void checkHeaderValidity(E32Image& image);
	void checkImportValidity(E32Image& image);
}