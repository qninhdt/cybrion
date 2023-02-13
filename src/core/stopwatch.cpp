#include "core/stopwatch.hpp"

using namespace std::chrono;

namespace cybrion
{
    void Stopwatch::reset()
    {
        m_lastTime = high_resolution_clock::now();
    }

    u32 Stopwatch::getDeltaTime() const
    {
        auto time = high_resolution_clock::now();
        return duration_cast<milliseconds>(time - m_lastTime).count();
    }

    void Stopwatch::reduceDeltaTime(u32 amount)
    {
        m_lastTime += milliseconds(amount);
    }
}