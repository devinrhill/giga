#ifndef GIGA_ENDIANNESS_H
#define GIGA_ENDIANNESS_H

#include <cstdint>

namespace giga {
namespace endian {
enum class Endianness {
	Unknown = 0,
	Little = 1234,
	Big = 4321
};

#if (defined(__x86_64__) || defined(__i386__))
constexpr Endianness native = Endianness::Little;
#else
constexpr Endianness native = Endianness::Big;
#endif

inline uint16_t byteswap16(uint16_t num) {
    return ( (((num) >> 8) & 0x00ff) | (((num) << 8) & 0xff00) );
}

inline uint32_t byteswap32(uint32_t num) {
    return ( (((num) >> 24) & 0x000000ff) | (((num) >>  8) & 0x0000ff00) |
             (((num) <<  8) & 0x00ff0000) | (((num) << 24) & 0xff000000) );
}

inline uint64_t byteswap64(uint64_t num) {
    return ( (((num) >> 56) & 0x00000000000000ff) | (((num) >> 40) & 0x000000000000ff00) |
             (((num) >> 24) & 0x0000000000ff0000) | (((num) >>  8) & 0x00000000ff000000) |
             (((num) <<  8) & 0x000000ff00000000) | (((num) << 24) & 0x0000ff0000000000) |
             (((num) << 40) & 0x00ff000000000000) | (((num) << 56) & 0xff00000000000000) );
}

const char* getEndiannessName(Endianness endianness = Endianness::Unknown);
template<typename T>
T byteswapEndianness(T num, Endianness endianness = native) {
	if(endianness != native) {
		switch(sizeof(T)) {
		case 1:
			goto do_nothing;
		case 2:
			return byteswap16(num);
		case 4:
			return byteswap32(num);
		case 8:
			return byteswap64(num);
		default:
			T tmpNum = num;

			for(std::size_t i = 0; i < sizeof(T); i++) {
				reinterpret_cast<char*>(&tmpNum)[i] = reinterpret_cast<char*>(&num)[sizeof(T) - i - 1];
			}

			return tmpNum;
		}
	}

do_nothing:
	return num;
}

} // namespace endian
} // namespace giga

#endif // GIGA_ENDIANNESS_H
