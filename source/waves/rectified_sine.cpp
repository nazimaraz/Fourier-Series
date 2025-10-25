//
// Created by Nazım Can on 25.10.2025.
//

#include "rectified_sine.hpp"
#include "math/math.h"
#include "gui/settings.hpp"
#include "function.h"

using namespace Waves;

RectifiedSine::RectifiedSine()
    : Wave{"Rectified Sine Wave", Type::RectifiedSine}
{}

std::pair<float, float> RectifiedSine::get_formula(const float i) const
{
    return {};
}
