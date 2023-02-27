#ifndef JBT_IO_H
#define JBT_IO_H

#include "jbt/internal.hpp"
#include <streambuf>

namespace jbt {

    class imem_streambuf : public std::streambuf {
    public:
        imem_streambuf(char* data, const std::size_t& size);
        ~imem_streambuf();

    private:
        std::streamsize xsgetn(char_type* const s, std::streamsize const count);
    };

    class imem_stream :public std::istream {
    public:
        imem_stream(char* buffer, const std::size_t& size);
        ~imem_stream();
    private:
        imem_streambuf m_buffer;
    };

    class omem_streambuf : public std::streambuf {

    public:

        omem_streambuf();
        omem_streambuf(char* data, const std::size_t& size, const std::size_t& capacity);

        ~omem_streambuf();

        std::size_t capacity() const;
        std::size_t size() const;
        char* buffer() const;

        void reserve(const std::size_t& new_capacity); 
        
    private:
        std::streamsize xsputn(char_type const* s, std::streamsize const count);

        char* m_buffer;
        std::size_t m_capacity;
        std::size_t m_size;
    };

    class omem_stream : public std::ostream {
    public:
        omem_stream();
        std::size_t capacity() const;
        std::size_t size() const;
        char* buffer() const;
        void reserve(const std::size_t& new_capacity);
    private:
        omem_streambuf m_buffer;
    };

}

#endif // JBT_IO_H