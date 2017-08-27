#include <iostream>
#include <memory.h>

#include "TRomLoader.h"
#include "..\Utils.h"
#include "..\TRom.h"

bool TRomLoader::parse(std::string& path, TRom& image) {
	auto err = utils::loadData(path, image.data);
	if (!err) {
		return false;
	}

	parseHeader(image);

	return true;
}

void TRomLoader::parseHeader(TRom& image) {
	uint32_t* data32 = reinterpret_cast<uint32_t*>(image.data.data());
	TRomHeader& header = image.header;

	memcpy(header.iJump, image.data.data(), 128); //uint8_t
	header.major = data32[32] & 0xFF; //TVersion
	header.minor = data32[32] & 0xFF00;
	header.build = data32[32] >> 16;
	header.iTime = ((int64_t)data32[33] << 32) | data32[34]; //int64_t
	header.iRomBase = data32[35]; //TLinAddr
	header.iRomSize = data32[36]; //uint32_t
	header.iRomRootDirectoryList = data32[37]; //TLinAddr
	header.iKernDataAddress = data32[38]; //TLinAddr
	header.iKernStackAddress = data32[39]; //TLinAddr
	header.iPrimaryFile = data32[40]; //TLinAddr
	header.iSecondaryFile = data32[41]; //TLinAddr
	header.iCheckSum = data32[42]; //uint32_t
	header.iLanguage = ((int64_t)data32[43] << 32) | data32[44]; //int64_t
	header.iHardware = data32[45]; //uint32_t
	header.screen_width = data32[46]; //TScreenSize
	header.screen_height = data32[47];
	header.iScreenBitsPerPixel = data32[48]; //int32_t
	header.iRomSectionHeader = data32[49]; //TLinAddr
	header.iTotalSvDataSize = data32[50]; //int32_t
	header.iVariantFile = data32[51]; //TLinAddr
	header.iExtensionFile = data32[52]; //TLinAddr
	header.iRelocInfo = data32[53]; //TLinAddr
	header.iTraceMask = data32[54]; //uint32_t
	header.iUserDataAddress = data32[55]; //TLinAddr
	header.iTotalUserDataSize = data32[56]; //int32_t
}
