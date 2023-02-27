#include "core/pool.hpp"

BS::thread_pool pool(std::thread::hardware_concurrency() - 3);

BS::thread_pool& cybrion::GetPool()
{
    return pool;
}
