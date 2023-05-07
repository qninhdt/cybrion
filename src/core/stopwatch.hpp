#pragma once

namespace cybrion
{
    class Stopwatch
    {
    public:
        void reset();
        u32 getDeltaTime() const;
        void reduceDeltaTime(u32 amount);

    private:
        std::chrono::steady_clock::time_point m_lastTime;
    };
}