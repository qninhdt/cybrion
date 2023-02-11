#pragma once

namespace cybrion
{
    class Stopwatch
    {
    public:
        void reset();
        f32 getDeltaTime() const;
    private:
        std::chrono::steady_clock::time_point m_lastTime;
    };
}