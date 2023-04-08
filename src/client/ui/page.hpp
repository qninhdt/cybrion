#pragma once

namespace cybrion::ui
{
    class Page
    {
    public:
        Page();
        virtual void onOpen() = 0;
        virtual void onRender() = 0;
        virtual void onClose() = 0;
    protected:
        ImGuiIO& m_io;
    };
}