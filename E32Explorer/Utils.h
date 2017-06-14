#pragma once
#include <stdint.h>
#include <vector>

namespace utils {
	uint16_t crc16_ccitt(uint8_t(&values)[6]);
	bool loadData(std::string path, std::vector<uint8_t>& data);
	void u8_from_32(uint8_t output[4], uint32_t value);
}