//
// Created by nazim on 6/27/26.
//

#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
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

std::string_view Waves::formula_tex(const std::size_t wave_index)
{
    if (wave_index >= wave_count)
        return {};

    return formulas.at(wave_index);
}

namespace
{
    [[nodiscard]] std::string format_number(const float value)
    {
        auto stream = std::ostringstream{};
        stream << std::setprecision(3) << value;
        return stream.str();
    }

    // Replace every occurrence of `needle` in `haystack` with `replacement`.
    void replace_all(std::string& haystack, const std::string_view needle, const std::string_view replacement)
    {
        if (needle.empty())
            return;

        for (auto pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + replacement.size()))
            haystack.replace(pos, needle.size(), replacement);
    }

    [[nodiscard]] std::string rewrite_time_tokens(std::string formula, const std::string_view replacement)
    {
        replace_all(formula, R"(\ldots)", "\x01");
        replace_all(formula, R"(\cdots)", "\x02");
        replace_all(formula, "t", replacement);
        replace_all(formula, "\x01", R"(\ldots)");
        replace_all(formula, "\x02", R"(\cdots)");
        return formula;
    }

    // Show the transformed input symbolically on the left and plug the numeric frequency into the series.
    [[nodiscard]] std::string rewrite_time_variable(std::string formula, const float frequency)
    {
        const auto equals_pos = formula.find('=');
        if (equals_pos == std::string::npos)
            return rewrite_time_tokens(std::move(formula), R"(2\pi f t)");

        auto lhs = formula.substr(0, equals_pos);
        auto rhs = formula.substr(equals_pos + 1);
        lhs = rewrite_time_tokens(std::move(lhs), R"(2\pi f t)");
        rhs = rewrite_time_tokens(std::move(rhs), R"((2\pi )" + format_number(frequency) + ")t");
        return lhs + "=" + rhs;
    }
} // namespace

std::string Waves::dynamic_formula_tex(const std::size_t wave_index, const unsigned int harmonic_count, const float radius,
    const float frequency)
{
    auto formula = std::string{formula_tex(wave_index)};
    if (formula.empty())
        return {};

    replace_all(formula, R"(\infty)", std::to_string(harmonic_count));
    formula = rewrite_time_variable(formula, frequency);
    if (std::fabs(radius - 1.f) > 1e-3f)
    {
        const auto rhs = std::string_view{formula}.substr(formula.find('=') + 1);
        formula.replace(formula.find('=') + 1, rhs.size(), std::string{format_number(radius)} + R"(\cdot)" + std::string{rhs});
    }

    return formula;
}
