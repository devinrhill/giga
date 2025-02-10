#include "platform.h"

namespace giga {
namespace system {

const char* getPlatformName(Platform _platform) {
	if(_platform == Platform::Unknown) {
		_platform = PLATFORM;
	}

	switch(_platform) {
	case Platform::Unknown:
		goto unknown;
	case Platform::Unix:
		return "Unix";
	case Platform::Posix:
		return "POSIX";
	case Platform::Linux:
		return "Linux";
	case Platform::Windows:
		return "Microsoft Windows";
	case Platform::Mac:
		return "Apple iMac";
	case Platform::iPhone:
		return "Apple iPhone";
	}

unknown:
	return "Unknown";
}

} // namespace system
} // namespace giga
