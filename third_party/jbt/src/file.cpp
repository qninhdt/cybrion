#include "jbt/file.hpp"
#include "jbt/tag.hpp"
#include "jbt/compression.hpp"
#include "jbt/serializer.hpp"

namespace jbt {

    void save_tag(const tag& src, const std::string& path) {
        std::ofstream output(path, std::ios_base::binary);
        std::string jbt_text = "JBT";
        output.write(jbt_text.data(), 3);
        save_tag(src, output);
        output.close();
    }

    void save_tag(const tag& src, std::ofstream& dst) {
        assert(serializer::instance != nullptr && "Jbt is not init");
        compression_util::compress_tag(
            *serializer::instance,
            src,
            dst
        );
    }

    void open_tag(tag& dst, const std::string& path) {
        std::ifstream input(path, std::ios_base::binary);
        std::string jbt_text = "   ";
        input.read(jbt_text.data(), 3);

        assert(jbt_text == "JBT" && "This file is not a jbt file");

        open_tag(dst, input);
        input.close();
    }

    void open_tag(tag& dst, std::ifstream& src) {
        assert(serializer::instance != nullptr && "Jbt is not init");
        compression_util::decompress_tag(
            *serializer::instance,
            src,
            dst
        );
    }
}