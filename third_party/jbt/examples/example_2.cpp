#include <zlib.h>
#include <lz4.h>
#include <fstream>
#include <iostream>

char* gen_data(int size) {
    char* data = new char[size];
    
    for (int i = 0; i < size; ++i) {
        if (i % 3 == 0) {
            data[i] = sin(i) * 255 + i;
        } else {
            data[i] = i%2;
        }
    }

    return data;
}

void zlib_write(char* data, int size) {
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int max_size = compressBound(size);
    char* output = new char[max_size];

    defstream.avail_in = size;
    defstream.next_in = (Bytef*) data;
    defstream.avail_out = max_size;
    defstream.next_out = (Bytef*) output;

    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    int output_size = defstream.total_out;

    std::ofstream file("D:/github/jbt/zlib.dat", std::ios_base::binary);
    file.write(output, output_size);
    file.close();
}

void lz4_write(char* data, int size) {
    int max_size = LZ4_COMPRESSBOUND(size);
    char* output = new char[max_size];
    int output_size = LZ4_compress_default(data, output, size, max_size);

    std::ofstream file("D:/github/jbt/lz4.dat", std::ios_base::binary);
    file.write(output, output_size);
    file.close();
}

int main() {
    clock_t begin_time;
    const int size = 32 * 32 * 32 * 8;
    char* data = gen_data(size);

    std::cout << "Compress and write data to file:\n";

    {
        // LZ4
        begin_time = clock();
    
        lz4_write(data, size);
    
        std::cout << "- lz4  : " << double(clock() - begin_time) / CLOCKS_PER_SEC << "s\n";
    }

    {
        // ZLIB
        begin_time = clock();

        zlib_write(data, size);

        std::cout << "- zlib : " << double(clock() - begin_time) / CLOCKS_PER_SEC << "s\n";
    }
}