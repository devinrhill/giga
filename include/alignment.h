#ifndef GIGA_ALIGNMENT_H
#define GIGA_ALIGNMENT_H

#include "platform.h"

namespace giga {
namespace system {

#ifdef IS_64_BIT
constexpr unsigned int alignment = 8;
#else
constexpr unsigned int alignment = 4;
#endif

inline unsigned align(const unsigned size) {
	return (((size) + (alignment - 1)) & ~(alignment - 1));
}

} // namespace system
} // namespace giga

#endif // GIGA_ALIGNMENT_H
