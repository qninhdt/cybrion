#ifndef JBT_COMPRESSION_H
#define JBT_COMPRESSION_H

#include "jbt/internal.hpp"
#include "jbt/io.hpp"

namespace jbt {

    class compression_util {
    public:
        static std::uint32_t compress_tag(serializer& ser, const tag& src, omem_stream& dst);
        static std::uint32_t compress_tag(serializer& ser, const tag& src, std::ostream& dst);
        static void decompress_tag(serializer& ser, std::istream& src, tag& dst);
    };
}

#endif // !JBT_COMPRESSION_H