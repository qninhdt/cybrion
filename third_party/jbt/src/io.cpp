#include "jbt/io.hpp"

namespace jbt {

    imem_streambuf::imem_streambuf(char* data, const std::size_t& size) {
        setg(data, data, data + size);
    }

    imem_streambuf::~imem_streambuf() {
        
    }

    std::streamsize imem_streambuf::xsgetn(char_type* const s, std::streamsize const count) {
        std::memcpy(s, gptr(), count);
        gbump(count);

        return count;
    }

    imem_stream::imem_stream(char* buffer, const std::size_t& size):
        m_buffer(buffer, size),
        std::istream(&m_buffer) {
    }

    imem_stream::~imem_stream() {
    }

    omem_streambuf::omem_streambuf(): m_size(0), m_capacity(64) {
        m_buffer = new char[m_capacity];
        setp(m_buffer, m_buffer + m_size);
    }

    std::streamsize omem_streambuf::xsputn(char_type const* s, std::streamsize const count) {
        if (count + m_size > m_capacity) {
            reserve(count + m_size);
        }
        m_size += count;
        std::memcpy(pptr(), s, count);
        pbump(count);
        return count;
    }

    void omem_streambuf::reserve(const std::size_t& new_capacity) {
        while (m_capacity < new_capacity) {
            m_capacity *= 2;
        }
        char* new_buffer = new char[m_capacity];
        std::memcpy(new_buffer, m_buffer, m_size);
        delete[] m_buffer;
        m_buffer = new_buffer;

        const auto current = pptr() - pbase();
        setp(new_buffer, new_buffer + m_size);
        pbump(current);
    }

    omem_streambuf::~omem_streambuf() {
        delete[] m_buffer;
    }

    std::size_t omem_streambuf::size() const {
        return m_size;
    }

    std::size_t omem_streambuf::capacity() const {
        return m_capacity;
    }

    char* omem_streambuf::buffer() const {
        return m_buffer;
    }
    
    omem_stream::omem_stream() : std::ostream(&m_buffer) {

    }

    std::size_t omem_stream::capacity() const {
        return m_buffer.capacity();
    }

    std::size_t omem_stream::size() const {
        return m_buffer.size();
    }

    char* omem_stream::buffer() const {
        return m_buffer.buffer();
    }

    void omem_stream::reserve(const std::size_t& new_capacity) {
        m_buffer.reserve(new_capacity);
    }
}