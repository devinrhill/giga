#include <filesystem>
#include <format>
#include <string>
#include <sys/stat.h>
#include "archive.h"
#include "error.h"

namespace giga {

void Archive::inject(const std::string& filename) {
    struct stat st;
    if(!stat(filename.c_str(), &st)) {
        if(st.st_mode & S_IFREG) {
            std::filesystem::path path = std::filesystem::path(filename).relative_path();

            giga::Bytestream bytestream;
            bytestream.openFile(filename);
            bytestream.setFilename(path.lexically_relative(*path.begin()).string());

            this->push_back(bytestream);
        }
    }
}

void Archive::injectDirectory(const std::string& directoryName) {
    for(const auto& directoryEntry: std::filesystem::recursive_directory_iterator(directoryName)) {
        inject(directoryEntry.path());
    }
}

void Archive::extract(const std::string& filename, const std::string& directoryName) {
    std::filesystem::create_directory(directoryName);

    std::string tmpPath;
    for(Bytestream& bytestream: *this) {
        if(bytestream.getFilename() == filename) {
            std::filesystem::create_directory(std::filesystem::path(filename).parent_path());

            tmpPath = std::filesystem::path(directoryName) / std::filesystem::path(filename);

            bytestream.finalizeFile(tmpPath);

            break;
        }
    }
}

void Archive::extractAll(const std::string& directoryName) {
    std::filesystem::create_directory(directoryName);

    std::filesystem::path tmpPath;
    for(Bytestream& bytestream: *this) {
        tmpPath = std::filesystem::path(directoryName) / std::filesystem::path(bytestream.getFilename());

        std::filesystem::create_directory(tmpPath.parent_path());
        
        bytestream.finalizeFile(tmpPath);
    }
}

const std::string& Archive::getFilename() const noexcept {
    return _filename;
}

giga::Bytestream& Archive::getMember(const std::string& filename) {
    for(giga::Bytestream& bytestream: *this) {
        if(bytestream.getFilename() == filename) {
            return bytestream;
        }
    }

    throw giga::err::FormatException<std::runtime_error>(std::format("Couldn't find member '{}'", filename));
}

void Archive::setFilename(const std::string& filename) noexcept {
    _filename = filename;
}

} // namespace giga
