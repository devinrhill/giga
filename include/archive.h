#ifndef GIGA_ARCHIVE_H
#define GIGA_ARCHIVE_H

#include <vector>
#include "bytestream.h"

namespace giga {

class Archive: public std::vector<giga::Bytestream> {
public:
    // input
    void inject(const std::string& filename);
    void injectDirectory(const std::string& directoryName = ".");

    // output
    void extract(const std::string& filename, const std::string& directoryName = ".");
    void extractAll(const std::string& directoryName = ".");
};

} // namespace giga

#endif // GIGA_ARCHIVE_H
