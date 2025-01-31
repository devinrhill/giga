#include "endianness.h"

namespace giga {
namespace endian {

const char* getEndiannessName(Endianness endianness) {
	if(endianness == Endianness::Unknown) {
		endianness = native;
	}

	switch(endianness) {
	case Endianness::Little:
		return "Little";
	case Endianness::Big:
		return "Big";
	default:
		goto unknown_endianness;
	}

unknown_endianness:
	return "Unknown";
}

} // namespace endian
} // namespace giga
