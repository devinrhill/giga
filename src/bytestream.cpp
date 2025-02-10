#include <algorithm>
#include <cstddef>
#include <format>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "bytestream.h"
#include "error.h"

namespace giga {

// TODO: Review code and improve
// TODO: Catch ALL exceptions (slow)

Bytestream& Bytestream::operator=(Bytestream& other) {
    this->reset();

    this->setBuf(const_cast<const std::uint8_t*>(other.getBuf()), other.getSize());
    _pos = other.getPos();
    _filename = other.getFilename();
    _endianness = other.getEndianness();

    return *this;
}

void Bytestream::openFile(const std::string& filename) {
	if(filename.empty()) {
        throw err::FormatException<std::invalid_argument>("Filename is empty");
    }

    std::ifstream file;
    file.exceptions(std::ifstream::badbit | std::ifstream::eofbit | std::ifstream::failbit);

    try {
        file.open(filename, std::ifstream::binary | std::ifstream::ate);
    } catch(const std::ifstream::failure& e) {
        throw err::FormatException<std::runtime_error>(std::format("Couldn't open file '{}' for reading", filename));
    }

    std::size_t fileSize = 0;
    try {
        fileSize = static_cast<std::size_t>(file.tellg());
    } catch(const std::ifstream::failure& e) {
        throw err::FormatException<std::runtime_error>(std::format("Couldn't get file input stream position of file '{}'", filename));
    }

    if(fileSize < 1) {
        throw err::FormatException<std::runtime_error>(std::format("File '{}' is empty", filename));
    }

    try {
        _buf.resize(fileSize);
    } catch(const std::length_error& e) {
        throw err::FormatException<std::runtime_error>(std::format("Couldn't resize internal character buffer to size {}", fileSize));
    }

    try {
        file.seekg(0, std::ifstream::beg);
    } catch(const std::ifstream::failure& e) {
        throw err::FormatException<std::runtime_error>(std::format("Couldn't seek to file input stream position 0 of file '{}'", filename));
    }

    try {
        file.read(reinterpret_cast<char*>(_buf.data()), fileSize);
    } catch(const std::ifstream::failure& e) {
        throw err::FormatException<std::runtime_error>(std::format("Couldn't read {} characters from file input stream of file '{}'", fileSize, filename));
    }

    _filename = filename;
}

void Bytestream::finalizeFile(const std::string& filename) {
	if(filename.empty()) {
        throw err::FormatException<std::invalid_argument>("Filename is empty");
    }

    std::ofstream file;
    file.exceptions(std::ofstream::badbit | std::ofstream::eofbit | std::ofstream::failbit);

    try {
        file.open(filename, std::ofstream::binary);
    } catch(const std::ofstream::failure& e) {
        throw err::FormatException<std::runtime_error>(std::format("Couldn't open file '{}' for reading", filename));
    }

    try {
        file.write(reinterpret_cast<const char*>(_buf.data()), _buf.size());
    } catch(const std::ofstream::failure& e) {
        throw err::FormatException<std::runtime_error>(std::format("Couldn't write {} characters to file output stream of file '{}'", _buf.size(), filename));
    }
}

std::size_t Bytestream::seek(std::size_t pos, SeekMode mode) {
	std::size_t tmpPos = _pos;

    switch(mode) {
        case SeekMode::Beg:
            tmpPos = pos;
            break;
        case SeekMode::Cur:
            tmpPos += pos;
            break;
        case SeekMode::End:
            tmpPos = _buf.size() - pos;
            break;
        default:
            throw err::FormatException<std::runtime_error>(std::format("Got invalid internal character buffer seek mode {}", static_cast<std::size_t>(mode)));
    }

    if(tmpPos > _buf.size()) {
        throw err::FormatException<std::out_of_range>(std::format("New internal character buffer position {} exceeds buffer size {}", tmpPos, _buf.size()));
    }

    _pos = tmpPos;

    return _pos;
}

std::size_t Bytestream::read(std::uint8_t* buf, std::size_t size, int offset) {
    if(!buf) {
        throw err::FormatException<std::invalid_argument>("Buffer is null");
    }
    if(size < 1) {
        throw err::FormatException<std::invalid_argument>("Buffer size is too small");
    }

    std::size_t tmpOffset;
    if(offset == -1) {
        tmpOffset = _pos;
    } else {
        tmpOffset = offset;
    }

    std::size_t availableCount = _buf.size() - tmpOffset;
    if(availableCount < size) {
        throw err::FormatException<std::runtime_error>("Not enough characters available in the internal character buffer to read");
    }

    std::size_t readCount = std::min(availableCount, size);

    std::copy(_buf.data() + tmpOffset, _buf.data() + tmpOffset + readCount, buf);
    if(offset == -1) {
        _pos += readCount;
    }

    return readCount;
}

std::size_t Bytestream::write(const std::uint8_t* buf, std::size_t size, int offset) {
	if(!buf) {
        throw err::FormatException<std::invalid_argument>("Buffer is null");
    }
    if(size < 1) {
        throw err::FormatException<std::invalid_argument>("Buffer size is too small");
    }

    std::size_t tmpOffset;
    if(offset == -1) {
        tmpOffset = _pos;
    } else {
        tmpOffset = offset;
    }

    if(tmpOffset + size > _buf.size()) {
        try {
            _buf.resize(tmpOffset + size);
        } catch(const std::length_error& e) {
            throw err::FormatException<std::length_error>(std::format("Couldn't resize internal character buffer to size {}", tmpOffset + size));
        }
    }

    std::copy(buf, buf + size, _buf.begin() + tmpOffset);
    if(offset == -1) {
        _pos += size;
    }

    return size;
}

void Bytestream::writePadding(std::uint8_t pad, std::size_t padCount, int offset) {
	if(padCount < 1) {
        return;
    }

	for(std::size_t i = 0; i < padCount; i++) {
		try {
            this->write(&pad, 1, offset);
        } catch(const std::exception& e) {
            throw err::FormatException<std::runtime_error>("Couldn't write padding byte to internal character buffer");
        }
	}
}

void Bytestream::writeString(const std::string& str, std::uint8_t pad, std::size_t len) {
    if(str.empty()) {
		throw err::FormatException<std::invalid_argument>("String is empty");
	}
	if(len < 1) {
        return;
	}

    for(std::size_t i = 0; i < len; i++) {
        if(i < str.size()) {
            try {
                this->write(reinterpret_cast<const std::uint8_t*>(str.c_str() + i), 1);
            } catch(const std::exception& e) {
                throw err::FormatException<std::runtime_error>(std::format("Couldn't write character {} of string 0x{:x} to internal character buffer", i, str[i]));
            }
        } else {
            try {
                this->writeScalar<char>(pad);
            } catch(const std::exception& e) {
                throw err::FormatException<std::runtime_error>(std::format("Couldn't write padding character 0{:x} to internal character buffer", pad));
            }
        }
    }
}

bool Bytestream::isEmpty() const noexcept {
	return _buf.empty();
}


std::string Bytestream::getFilename() const noexcept {
    return _filename;
}

std::uint8_t* Bytestream::getBuf() noexcept {
	return _buf.data();
}

std::size_t Bytestream::getSize() const noexcept {
	return _buf.size();
}

std::size_t Bytestream::getPos() const noexcept {
	return _pos;
}


endian::Endianness Bytestream::getEndianness() const noexcept {
	return _endianness;
}

void Bytestream::reset() noexcept {
	_buf.clear();
	_pos = 0;
	_endianness = endian::NATIVE;
    _filename = "";
}

void Bytestream::setFilename(const std::string& filename) noexcept {
    _filename = filename;
}

void Bytestream::setBuf(const std::uint8_t* buf, std::size_t size) {
    _buf.clear();
    _buf.resize(size);
    std::copy(buf, buf + size, _buf.data());
}

void Bytestream::setEndianness(endian::Endianness endianness) noexcept {
	_endianness = endianness;
}

} // namespace giga
