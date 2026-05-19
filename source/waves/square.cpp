//
// Created by Nazım Can on 25.10.2025.
//

#include "square.hpp"
#include "math/math.h"
#include "gui/settings.hpp"
#include "function.h"

using namespace Waves;

Square::Square()
    : Wave{"Square Wave", Type::Square}
{}

HarmonicTerm Square::get_formula(const float i) const
{
    const auto n = i * 2.f + 1.f;
    const auto coefficient = 4.f / (math::pi_v<float> * n);
    return {n, coefficient, 0.f};
}
