#ifndef JBT_HJBT
#define JBT_HJBT

#include "jbt/internal.hpp"
#include <unordered_map>

namespace jbt {

    class hjbt_util {
    public:
        static void create_empty_file(const std::string& path, const std::uint32_t& max_size, const std::uint32_t& block_size);
    };

    struct hjbt_free_range {
        std::uint32_t offset;
        std::uint32_t size;
        hjbt_free_range* next;
    };

    class hjbt_file {
    
    public:

        hjbt_file();
        hjbt_file(const std::string& path);

        ~hjbt_file();

        void open(const std::string& path);
        void close();

        bool has(const std::uint32_t& tag_id);
        void read(const std::uint32_t& tag_id, tag& dst);
        void write(const std::uint32_t& tag_id, const tag& src);
        void remove(const std::uint32_t& tag_id);

        void begin_write();
        void end_write();

        std::uint32_t size() const;
        std::uint32_t max_size() const;
        std::uint32_t block_size() const;
        std::string path() const;

        bool is_writing;

    private:
        void free_tag(const std::size_t& tag_id);

        std::string m_path;
        std::uint32_t m_header_size;
        std::uint32_t m_max_size;
        std::uint32_t m_size;
        std::uint32_t m_block_size;
        hjbt_free_range* m_first_range;
        std::ifstream* m_in_stream;
        std::ofstream* m_out_stream;
        std::unordered_map<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> m_info_map;
    };
}

#endif // !JBT_HJBT
