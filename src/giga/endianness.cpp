#include "giga/endianness.h"

namespace giga {
namespace endian {

const char* getEndiannessName(Endianness endianness) {
	if(endianness == Endianness::Unknown) {
		endianness = native;
	}

	switch(endianness) {
	case Endianness::Little:
		return "Little endian";
	case Endianness::Big:
		return "Big endian";
	default:
		goto unknown_endianness;
	}

unknown_endianness:
	return "Unknown endianness";
}

} // namespace endian
} // namespace giga
