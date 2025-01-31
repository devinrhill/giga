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
constexpr Platform platform = Platform::Unix;
#elif defined(_POSIX_VERSION)
constexpr Platform platform = Platform::Posix;
#elif __linux__
constexpr Platform platform = Platform::Linux;
#elif IS_WINDOWS
constexpr Platform platform = Platform::Windows;
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
constexpr Platform platform = Platform::Mac;
#elif TARGET_OS_IPHONE
constexpr Platform platform = Platform::iPhone;
#endif
#else
constexpr Platform platform = Platform::Unknown;
#endif

const char* getPlatformName(Platform _platform = Platform::Unknown);

#if IS_WINDOWS
constexpr const char* directorySeparator = "\\";
#else
constexpr const char* directorySeparator = "/";
#endif

} // namespace system
} // namespace giga

#endif // GIGA_PLATFORM_H
