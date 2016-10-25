#include "Utils.h"

uint16_t utils::crc16_ccitt(uint8_t(&values)[6])
{
	uint32_t result = 0;

	for (uint8_t b : values)
	{
		result ^= (b << 8);

		for (uint8_t d = 0; d < 8; d++)
		{
			result = result << 1;

			if (result & 0x10000)
			{
				result ^= 0x1021;
			}

			result &= 0xffff;
		}
	}

	return result;
}