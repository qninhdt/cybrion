#include "core/pool.hpp"

BS::thread_pool pool;

BS::thread_pool& cybrion::GetPool()
{
    return pool;
}
