#ifndef GIGA_PLATFORM_H
#define GIGA_PLATFORM_H

#ifdef __LP64__
#define IS_64_BIT
#else
#define IS_32_BIT
#endif

#if (defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
#define IS_WINDOWS
#endif

namespace giga {
namespace system {

enum class Platform {
	Unknown = 0,
	Unix = 1,
	Posix = 2,
	Linux = 3,
	Windows = 4,
	Mac = 5,
	iPhone = 6
};

#if __unix__
constexpr Platform PLATFORM = Platform::Unix;
#elif defined(_POSIX_VERSION)
constexpr Platform PLATFORM = Platform::Posix;
#elif __linux__
constexpr Platform PLATFORM = Platform::Linux;
#elif IS_WINDOWS
constexpr Platform PLATFORM = Platform::Windows;
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
constexpr Platform PLATFORM = Platform::Mac;
#elif TARGET_OS_IPHONE
constexpr Platform PLATFORM = Platform::iPhone;
#endif
#else
constexpr Platform PLATFORM = Platform::Unknown;
#endif

const char* getPlatformName(Platform _platform = Platform::Unknown);

#if IS_WINDOWS
constexpr const char* DIRECTORY_SEPARATOR = "\\";
#else
constexpr const char* DIRECTORY_SEPARATOR = "/";
#endif

} // namespace system
} // namespace giga

#endif // GIGA_PLATFORM_H
