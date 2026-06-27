//
// Created by nazim on 6/27/26.
//

#include "formula_tex.hpp"
#include "wave_variant.hpp"

using namespace Waves;

namespace
{
    // Indexing mirrors Waves::WaveVariant (see wave_variant.hpp).
    constexpr auto formulas = std::array<std::string_view, wave_count>{
        // Sawtooth
        R"(f(t)=\dfrac{2}{\pi}\sum_{n=1}^{\infty}\dfrac{(-1)^{n+1}}{n}\sin(nt))",
        // Square
        R"(f(t)=\dfrac{4}{\pi}\sum_{n=1,3,5,\ldots}^{\infty}\dfrac{1}{n}\sin(nt))",
        // Triangle
        R"(f(t)=\dfrac{8}{\pi^{2}}\sum_{n=1,3,5,\ldots}^{\infty}\dfrac{(-1)^{(n-1)/2}}{n^{2}}\sin(nt))",
        // Semicircle (Bessel J_1 coefficients)
        R"(f(t)=\dfrac{\pi}{4}+\sum_{n=1}^{\infty}\dfrac{(-1)^{n}}{n}J_{1}(n\pi)\cos(nt))",
        // Half-wave rectified sine
        R"(f(t)=\dfrac{1}{\pi}+\dfrac{1}{2}\sin(t)-\dfrac{2}{\pi}\sum_{n=2,4,6,\ldots}^{\infty}\dfrac{1}{n^{2}-1}\cos(nt))",
        // Full-wave rectified sine
        R"(f(t)=\dfrac{2}{\pi}-\dfrac{4}{\pi}\sum_{n=1}^{\infty}\dfrac{1}{4n^{2}-1}\cos(2nt))",
        // Parabolic
        R"(f(t)=\dfrac{32}{\pi^{3}}\sum_{n=1,3,5,\ldots}^{\infty}\dfrac{(-1)^{(n-1)/2}}{n^{3}}\cos(nt))",
        // Cosine
        R"(f(t)=\cos(t))",
        // Pulse 25%
        R"(f(t)=0.25+\dfrac{2}{\pi}\sum_{n=1}^{\infty}\dfrac{\sin(0.25\pi n)}{n}\cos(nt))",
        // Pulse 75%
        R"(f(t)=0.75+\dfrac{2}{\pi}\sum_{n=1}^{\infty}\dfrac{\sin(0.75\pi n)}{n}\cos(nt))",
        // Heart (tabulated parametric harmonics)
        R"(\bigl(x(t),y(t)\bigr)=\sum_{n}(a_{n}\cos nt+b_{n}\sin nt))",
        // User drawing (DFT-derived)
        R"(f(t)=\sum_{n}\bigl(a_{n}\cos nt+b_{n}\sin nt\bigr))",
    };
} // namespace

std::string_view Waves::formula_tex(const size_t wave_index)
{
    if (wave_index >= wave_count)
        return {};
    return formulas.at(wave_index);
}
