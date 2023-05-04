#pragma once

#include "client/GL/texture.hpp"

namespace cybrion::ui
{
    bool HoverableImageButton(const GL::Texture &img1, const GL::Texture &img2, i32 width, i32 height, bool &state);
}