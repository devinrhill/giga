#ifndef GIGA_BYTESTREAM_H
#define GIGA_BYTESTREAM_H

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include "endianness.h"

// TODO: Add documentation

namespace giga {

class Bytestream {
public:
    enum class SeekMode {
        Beg = 1,
        Cur = 2,
        End = 3
    };

    void openFile(const std::string& filename);
	void finalizeFile(const std::string& filename);
    std::size_t seek(std::size_t pos, SeekMode mode = SeekMode::Beg);
    std::size_t read(std::uint8_t* buf, std::size_t size, int offset = -1);
    std::size_t write(const std::uint8_t* buf, std::size_t size, int offset = -1);
    void writePadding(std::uint8_t pad, std::size_t padCount, int offset = -1);
    void writeString(const std::string& str, std::uint8_t pad = 0, std::size_t len = -1);

    bool isEmpty() const noexcept;
    std::string getFilename() const noexcept;
    const std::uint8_t* getBuf() const noexcept;
    std::size_t getSize() const noexcept;
    std::size_t getPos() const noexcept;
    endian::Endianness getEndianness() const noexcept;

    void reset() noexcept;
    void setFilename(const std::string& filename) noexcept;
    void setEndianness(endian::Endianness _endianness) noexcept;
	
	template<typename T>
    T readScalar(int size = -1) {
        static_assert(std::is_scalar<T>::value, "Type T is non-scalar");

        std::size_t tmpSize;
        if(size == -1) {
            tmpSize = sizeof(T);
        } else {
            tmpSize = size;
        }

        T num;
        try {
            this->read(reinterpret_cast<std::uint8_t*>(&num), tmpSize);
        } catch(const std::exception& e) {
            throw std::runtime_error("Couldn't read from buffer: " + std::string(e.what()));
        }

        num = endian::byteswapEndianness<T>(num, _endianness, tmpSize);

        return num;
    }


    template<typename T>
    void writeScalar(const T num, int size = -1) {
        static_assert(std::is_scalar<T>::value, "type T is non-scalar");

        std::size_t tmpSize;
        if(size == -1) {
            tmpSize = sizeof(T);
        } else {
            tmpSize = size;
        }

        // swap bytes if needed
        T tmpNum = endian::byteswapEndianness<T>(num, _endianness, tmpSize);
        try {
            this->write(reinterpret_cast<const std::uint8_t*>(&tmpNum), tmpSize);
        } catch(const std::exception& e){
            throw std::runtime_error("couldn't write to buffer: " + std::string(e.what()));
        }
    }

private:
    std::string _filename;
    std::vector<std::uint8_t> _buf;
    std::size_t _pos = 0;
    endian::Endianness _endianness = endian::native;
};

} // namespace giga

#endif // GIGA_BYTESTREAM_H
