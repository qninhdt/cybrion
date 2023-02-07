#include "client/frame_profiler.hpp"

using namespace std::chrono;

namespace cybrion
{
    FrameProfiler::FrameProfiler():
        m_fps(0),
        m_deltaTime(0),
        m_totalTime(0)
    {
        m_time = high_resolution_clock::now();
        m_lastTime = high_resolution_clock::now();
    }

    void FrameProfiler::tick()
    {
        m_lastTime = m_time;
        m_time = high_resolution_clock::now();
        f32 deltaTime = duration_cast<duration<f32>>(m_time - m_lastTime).count();

        m_timeQueue.push(deltaTime);
        m_totalTime += deltaTime;
        if (m_timeQueue.size() > MAX_DELTA_TIME_QUEUE_SIZE)
        {
            m_totalTime -= m_timeQueue.front();
            m_timeQueue.pop();
        }

        m_deltaTime = m_totalTime / m_timeQueue.size();
        m_fps = 1/ m_deltaTime;
    }

    f32 FrameProfiler::getFPS() const
    {
        return m_fps;
    }

    f32 FrameProfiler::getDeltaTime() const
    {
        return m_deltaTime;
    }
}