//
// Created by nazim on 6/27/26.
//

#include <array>
#include <cmath>
#include <cstdio>
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

std::string_view Waves::formula_tex(const size_t wave_index)
{
    if (wave_index >= wave_count)
        return {};

    return formulas.at(wave_index);
}

namespace
{
    [[nodiscard]] std::string format_number(const float value)
    {
        auto buffer = std::array<char, 32>{};
        const auto written = std::snprintf(buffer.data(), buffer.size(), "%.3g", value);
        return written > 0 ? std::string{buffer.data(), static_cast<size_t>(written)} : std::string{};
    }

    // Replace every occurrence of `needle` in `haystack` with `replacement`.
    void replace_all(std::string& haystack, const std::string_view needle, const std::string_view replacement)
    {
        if (needle.empty())
            return;

        for (auto pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + replacement.size()))
            haystack.replace(pos, needle.size(), replacement);
    }

    // Express the time variable `t` via omega = 2*pi*f by rewriting each trig argument. The original
    // formulas only use `t` as the standalone variable inside `\sin(...)` / `\cos(...)`, so the bare
    // token is replaced in place. `\ldots` / `\cdots` are protected first since they contain a `t`.
    [[nodiscard]] std::string rewrite_time_variable(std::string formula)
    {
        replace_all(formula, R"(\ldots)", "\x01");
        replace_all(formula, R"(\cdots)", "\x02");
        replace_all(formula, "t", R"(\omega t)");
        replace_all(formula, "\x01", R"(\ldots)");
        replace_all(formula, "\x02", R"(\cdots)");
        return formula;
    }
} // namespace

std::string Waves::dynamic_formula_tex(const size_t wave_index, const unsigned int harmonic_count, const float radius,
    const float frequency)
{
    auto formula = std::string{formula_tex(wave_index)};
    if (formula.empty())
        return {};

    replace_all(formula, R"(\infty)", std::to_string(harmonic_count));
    formula = rewrite_time_variable(formula);
    const auto omega = [&] -> std::string {
        if (std::fabs(frequency) < 1e-6f)
            return "0";

        if (std::fabs(frequency - 1.f) > 1e-3f)
            return format_number(frequency) + R"(\pi)";

        return R"(2\pi)";
    }();

    if (std::fabs(radius - 1.f) > 1e-3f)
    {
        const auto rhs = std::string_view{formula}.substr(formula.find('=') + 1);
        formula.replace(formula.find('=') + 1, rhs.size(), std::string{format_number(radius)} + R"(\cdot)" + std::string{rhs});
    }

    formula += R"( \quad (\omega = )" + omega + ")";
    return formula;
}
