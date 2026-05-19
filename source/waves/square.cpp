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

std::pair<float, float> Square::get_formula(const float i) const
{
    const auto n = i * 2 + 1;
    const auto formula = 4 / (math::pi_v<float> * n);
    return {n, formula};
}
