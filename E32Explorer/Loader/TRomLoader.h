#pragma once

#include <string>

struct TRom;

namespace TRomLoader
{
	bool parse(std::string path, TRom& image);
	void parseHeader(TRom& image);

};