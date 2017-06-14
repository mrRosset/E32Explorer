#pragma once

#include <string>

struct E32Image;

class E32ImageLoader
{
public:
	bool load(std::string path, E32Image& image);
private:
	void parseHeader(E32Image& image);
	void parseIAT(E32Image& image);
	void parseExportDir(E32Image& image);
	void parseImportSection(E32Image& image);
	void parseRelocSections(E32Image& image);
	void checkHeaderValidity(E32Image& image);
	void checkImportValidity(E32Image& image);
};