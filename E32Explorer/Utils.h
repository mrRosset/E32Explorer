#pragma once
#include <stdint.h>

namespace utils {
	uint16_t crc16_ccitt(uint8_t(&values)[6]);
}