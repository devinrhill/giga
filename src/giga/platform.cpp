#include "giga/platform.h"

namespace giga {
namespace system {

const char* getPlatformName(Platform _platform) {
	if(_platform == Platform::Unknown) {
		_platform = platform;
	}

	switch(_platform) {
	case Platform::Unknown:
		goto unknown;
	case Platform::Unix:
		return "Unix system";
	case Platform::Posix:
		return "POSIX system";
	case Platform::Linux:
		return "Linux system";
	case Platform::Windows:
		return "Microsoft Windows system";
	case Platform::Mac:
		return "Apple Mac system";
	case Platform::iPhone:
		return "Apple iPhone system";
	}

unknown:
	return "Unknown platform";
}

} // namespace system
} // namespace giga
