#pragma once

#include <string>

struct TRomImage;

class TRomImageLoader
{
public:
	bool load(std::string path, TRomImage& image);
private:
	void parseHeader(TRomImage& image);
	void checkHeaderValidity(TRomImage& image);
};