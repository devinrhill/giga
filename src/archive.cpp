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

            try {
                tmpPath = std::filesystem::path(directoryName) / std::filesystem::path(filename);
            } catch(const std::exception& e) {
                throw err::FormatException<std::runtime_error>(std::format("Couldn't construct"));
            }

            try {
                bytestream.finalizeFile(tmpPath);
            } catch(const std::exception& e) {
                throw err::FormatException<std::runtime_error>(std::format("Couldn't finalize bytestream '{}'", tmpPath));
            }

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
        
        try {
            bytestream.finalizeFile(tmpPath);
        } catch(const std::exception& e) {
            throw err::FormatException<std::runtime_error>(std::format("Couldn't finalize bytestream '{}'", tmpPath.string()));
        }
    }
}

} // namespace giga
