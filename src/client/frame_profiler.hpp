#pragma once

namespace cybrion
{
    class FrameProfiler
    {
    public:
        static constexpr u32 MAX_DELTA_TIME_QUEUE_SIZE = 1 << 1;

        FrameProfiler();

        void tick();

        f32 getFPS() const;
        f32 getDeltaTime() const;
    private:
        f32 m_fps;
        f32 m_deltaTime;
        f64 m_totalTime;
        std::queue<f32> m_timeQueue;
        std::chrono::steady_clock::time_point m_lastTime, m_time;
    };
}