#ifndef JBT_FILE_H

#include "jbt/internal.hpp"

namespace jbt {

    void save_tag(const tag& src, const std::string& path);
    void save_tag(const tag& src, std::ofstream& dst);
    void open_tag(tag& dst, const std::string& path);
    void open_tag(tag& dst, std::ifstream& src);
}

#endif // !JBT_FILE_H
