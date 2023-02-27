#include  <algorithm>
#include "jbt/hjbt.hpp"
#include "jbt/serializer.hpp"
#include "jbt/compression.hpp"

namespace jbt {

    void hjbt_util::create_empty_file(const std::string& path, const std::uint32_t& max_size, const std::uint32_t& block_size) {
        std::ofstream file(path, std::ios_base::binary);
        const auto ser = serializer::instance;

        // write "HJBT
        std::string hjbt_text = "HJBT";
        file.write(hjbt_text.data(), 4);

        // write header
        std::uint32_t default_size = 0;
        ser->write_uint(file, max_size);
        ser->write_uint(file, block_size);
        ser->write_uint(file, default_size);

        file.close();
    }

    hjbt_file::hjbt_file() :
        m_path(""),
        m_in_stream(nullptr),
        m_out_stream(nullptr),
        m_header_size(0),
        m_max_size(),
        m_size(0),
        m_block_size(0),
        m_first_range(nullptr) {

    }

    hjbt_file::hjbt_file(const std::string& path) : hjbt_file() {
        open(path);
        m_path = path;
    }

    std::string hjbt_file::path() const {
        return m_path;
    }

    std::uint32_t hjbt_file::max_size() const {
        return m_max_size;
    }

    std::uint32_t hjbt_file::block_size() const {
        return m_block_size;
    }

    std::uint32_t hjbt_file::size() const {
        return m_size;
    }

    void hjbt_file::free_tag(const std::size_t& tag_id) {
        const auto& [old_offset, old_size] = m_info_map[tag_id];

        // free old memory
        if (m_first_range->offset >= old_offset + old_size) {
            // old memory at the begin
            m_first_range = new hjbt_free_range({ old_offset, old_size, m_first_range });
        }
        else {
            hjbt_free_range* curr = m_first_range;

            while (curr->next->offset < old_offset) {
                curr = curr->next;
            }

            // old memeory is between two free ranges
            if (old_offset == curr->offset + curr->size && old_offset + old_size == curr->next->offset) {
                // merge front range and back range
                curr->size += old_size + curr->next->size;
                hjbt_free_range* next_next = curr->next->next;
                delete curr->next;
                curr->next = next_next;
            }
            else if (old_offset == curr->offset + curr->size) {
                // increase size of front range
                curr->size += old_size;
            }
            else if (old_offset + old_size == curr->next->offset) {
                // increse size of back range
                curr->next->offset -= old_size;
                if (curr->next->size != 0) {
                    curr->next->size += old_size;
                }
            }
            else {
                // create new free range between two ranges
                curr->next = new hjbt_free_range({ old_offset, old_size, curr->next });
            }
        }
    }

    void hjbt_file::open(const std::string& path) {
        const auto ser = serializer::instance;
        m_in_stream = new std::ifstream(path, std::ios_base::binary);

        assert(m_in_stream->is_open() && "Hjbt file not found");

        // read "HJBT"
        std::string hjbt_text;
        hjbt_text.resize(4);
        m_in_stream->read(hjbt_text.data(), 4);

        assert(hjbt_text == "HJBT" && "This file is not a hjbt file");

        // read header
        ser->read_uint(*m_in_stream, m_max_size);
        ser->read_uint(*m_in_stream, m_block_size);
        ser->read_uint(*m_in_stream, m_size);

        assert(m_max_size >= m_size && "Too many tags");
        assert((m_block_size & (m_block_size - 1)) == 0 && "Block size must be a power of two");

        m_header_size = 4 + 4 + 4 + 4 + (12 * m_max_size);
        m_header_size = (m_header_size / m_block_size) + (m_header_size % m_block_size != 0);

        // read offsets and free range list
        std::uint32_t id = 0;
        std::uint32_t offset = 0;
        std::uint32_t size = 0;

        hjbt_free_range* current = new hjbt_free_range({ 0, 0, nullptr });
        std::uint32_t current_offset = 0;

        m_first_range = current;

        for (std::uint32_t i = 0; i < m_size; ++i) {
            ser->read_uint(*m_in_stream, id);
            ser->read_uint(*m_in_stream, offset);
            ser->read_uint(*m_in_stream, size);

            if (offset > current_offset) {
                current->size = offset - current_offset;
                current->next = new hjbt_free_range({ offset + size, 0 , nullptr });;
                current = current->next;
            }
            else {
                current->offset += size;
            }

            current_offset = offset + size;

            m_info_map[id] = { offset, size };
        }
    }

    void hjbt_file::read(const std::uint32_t& tag_id, tag& dst) {
        assert(m_info_map.find(tag_id) != m_info_map.end());

        m_in_stream->seekg((std::uint64_t)(m_header_size + m_info_map[tag_id].first) * m_block_size, std::ios_base::beg);
        compression_util::decompress_tag(*serializer::instance, *m_in_stream, dst);
    }

    void hjbt_file::write(const std::uint32_t& tag_id, const tag& src) {
        assert(tag_id < m_max_size && "Out of bounds");

        omem_stream src_stream;

        const std::uint32_t new_memory_size = compression_util::compress_tag(*serializer::instance, src, src_stream);

        const std::uint32_t new_size = (new_memory_size / m_block_size) + (new_memory_size % m_block_size != 0);

        bool need_to_realloc = false;

        if (m_info_map.find(tag_id) != m_info_map.end()) {
            if (new_size > m_info_map[tag_id].second) {
                free_tag(tag_id);
                need_to_realloc = true;
            }
        }
        else {
            m_size += 1;
            need_to_realloc = true;
        }

        if (need_to_realloc) {
            hjbt_free_range* prev = nullptr;
            hjbt_free_range* curr = m_first_range;

            while (curr->size < new_size && curr->size != 0) {
                hjbt_free_range* temp = curr;
                curr = curr->next;
                prev = temp;
            }

            if (curr) {
                m_info_map[tag_id] = { curr->offset, new_size };

                if (curr->size == new_size) {
                    // remove current free range
                    if (prev) {
                        prev->next = curr->next;
                    }
                    else {
                        m_first_range = curr->next;
                    }
                    delete curr;
                }
                else {
                    curr->offset += new_size;
                    if (curr->size != 0) {
                        curr->size -= new_size;
                    }
                }
            }
        }

        m_out_stream->seekp((std::uint64_t)(m_header_size + m_info_map[tag_id].first) * m_block_size, std::ios_base::beg);
        //m_out_stream->write(src_stream.buffer(), new_memory_size);
        std::uint32_t size = m_block_size;
        for (int i = 0; i < new_memory_size; i += m_block_size) {
            if (new_memory_size - i < m_block_size) {
                size = new_memory_size - i;
            }
            m_out_stream->write(src_stream.buffer() + i, size);
        }
    }

    void hjbt_file::remove(const std::uint32_t& tag_id) {
        assert(m_info_map.find(tag_id) != m_info_map.end());

        free_tag(tag_id);
        m_info_map.erase(tag_id);

        m_size -= 1;
    }

    void hjbt_file::begin_write() {
        assert(m_out_stream == nullptr);
        m_out_stream = new std::ofstream(m_path, std::ios_base::binary | std::ios_base::in);
    }

    void hjbt_file::end_write() {
        assert(m_out_stream != nullptr);
        const auto ser = serializer::instance;

        // update size
        m_out_stream->seekp((std::uint64_t)4 + 4 + 4, std::ios_base::beg);
        ser->write_uint(*m_out_stream, m_size);

        // update info map
        std::vector < std::pair<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>>> info_list;
        for (const auto& [id, info] : m_info_map) {
            const auto& [offset, size] = info;
            info_list.push_back({ offset, { id, size } });
        }

        std::sort(info_list.begin(), info_list.end());

        for (const auto& [offset, info] : info_list) {
            const auto& [id, size] = info;
            ser->write_uint(*m_out_stream, id);
            ser->write_uint(*m_out_stream, offset);
            ser->write_uint(*m_out_stream, size);
        }

        m_out_stream->close();
        delete m_out_stream;
    }

    void hjbt_file::close() {
        delete m_in_stream;
    }

    hjbt_file::~hjbt_file() {

    }
}