#include "jbt/jbt.hpp"
#include <lz4.h>

namespace jbt {

    std::uint32_t compression_util::compress_tag(serializer& ser, const tag& src, omem_stream& dst) {
        omem_stream src_stream;
        ser.write_tag(src_stream, src);

        const std::uint32_t src_size = src_stream.size();
        const std::uint32_t max_compressed_size = LZ4_COMPRESSBOUND(src_size);

        dst.reserve(max_compressed_size + 8);
        const std::int32_t _compressed_size = LZ4_compress_default(src_stream.buffer(),
            dst.buffer() + 8, src_size, max_compressed_size);

        assert(_compressed_size > 0 && "Compression is failed");

        /// TODO: compress tag into dst stream directly without temp var
        const std::uint32_t compressed_size = _compressed_size;
        
        dst.write((char*)&compressed_size, sizeof(std::uint32_t));
        dst.write((char*)&src_size, sizeof(std::uint32_t));

        return sizeof(std::uint32_t) * 2 + compressed_size;
    }

    std::uint32_t compression_util::compress_tag(serializer& ser, const tag& src, std::ostream& dst) {
        omem_stream temp_stream;
        const std::uint32_t size = compress_tag(ser, src, temp_stream);
        dst.write(temp_stream.buffer(), size);

        return size;
    }

    void compression_util::decompress_tag(serializer& ser, std::istream& src, tag& dst) {
        std::uint32_t src_size = 0;
        std::uint32_t dst_size = 0;
        src.read((char*)&src_size, sizeof(std::uint32_t));
        src.read((char*)&dst_size, sizeof(std::uint32_t));

        char* compressed_data = new char[src_size];
        char* decompressed_data = new char[dst_size];

        src.read(compressed_data, src_size);
        const std::uint32_t decompressed_size = LZ4_decompress_safe(compressed_data,
            decompressed_data, src_size, dst_size);
        
        imem_stream dst_stream(decompressed_data, decompressed_size);
        ser.read_tag(dst_stream, dst);

        delete[] compressed_data;
        delete[] decompressed_data;
    }
}