#include "client/stopwatch.hpp"

using namespace std::chrono;

namespace cybrion
{
    void Stopwatch::reset()
    {
        m_lastTime = high_resolution_clock::now();
    }

    f32 Stopwatch::getDeltaTime() const
    {
        auto time = high_resolution_clock::now();
        return duration_cast<duration<f32>>(time - m_lastTime).count();
    }
}