#include "jbt/jbt.hpp"

namespace jbt {

    void init() {
        int num = 1;
        if (*(char*)&num == 1) {
            // little-endian
            serializer::instance = new same_endian_serializer();
        } else {
            // big-endian
            serializer::instance = new diff_endian_serializer();
        }
    }

}