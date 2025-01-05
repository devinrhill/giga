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
    void seek(std::size_t pos, SeekMode mode = SeekMode::Beg);
    std::size_t read(std::uint8_t* buf, std::size_t size);
    std::size_t write(const std::uint8_t* buf, std::size_t size);
    void writePadding(std::uint8_t pad, std::size_t padCount);
    void writeString(const std::string& str, std::uint8_t pad = 0, std::size_t len = -1);

    bool isEmpty() const noexcept;
    std::size_t getSize() const noexcept;
    std::size_t getPos() const noexcept;
    const std::uint8_t* getBuf() const noexcept;
    endian::Endianness getEndianness() const noexcept;

    void reset() noexcept;
    void setEndianness(endian::Endianness _endianness) noexcept;
	
	template<typename T>
    T readScalar() {
        static_assert(std::is_scalar<T>::value, "Type T is non-scalar");

        T num;
        try {
            this->read(reinterpret_cast<std::uint8_t*>(&num), sizeof(T));
        } catch(const std::exception& e) {
            throw std::runtime_error("Couldn't read from buffer: " + std::string(e.what()));
        }

        // swap bytes if needed
        num = endian::byteswapEndianness<T>(num, _endianness);

        return num;
    }


    template<typename T>
    void writeScalar(const T num) {
        static_assert(std::is_scalar<T>::value, "type T is non-scalar");

        // swap bytes if needed
        T tmpNum = endian::byteswapEndianness<T>(num, _endianness);
        try {
            this->write(reinterpret_cast<const std::uint8_t*>(&tmpNum), sizeof(T));
        } catch(const std::exception& e){
            throw std::runtime_error("couldn't write to buffer: " + std::string(e.what()));
        }
    }

private:
    std::vector<std::uint8_t> _buf;
    std::size_t _pos = 0;
    endian::Endianness _endianness = endian::native;
};

} // namespace giga

#endif // GIGA_BYTESTREAM_H
