//
// Created by Nazım Can on 19.10.2025.
//

#pragma once

#include <cstdint>

namespace Waves
{
    enum class Type : std::uint8_t
    {
        Sawtooth,
        Triangle,
        Semicircle,
        Square,
        RectifiedSine
    };
} // namespace Waves
