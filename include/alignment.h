#ifndef GIGA_ALIGNMENT_H
#define GIGA_ALIGNMENT_H

#include "platform.h"
#include "types.h"

namespace giga {

#ifdef IS_64_BIT
constexpr uint ALIGNMENT = 8;
#else
constexpr uint ALIGNMENT = 4;
#endif

inline uint align(uint size) {
	return (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT- 1));
}

} // namespace giga

#endif // GIGA_ALIGNMENT_H
