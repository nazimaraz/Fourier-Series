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
    // Maximum number of contributing terms expanded explicitly; further ones collapse to \cdots.
    constexpr auto max_expanded_terms = size_t{4};

    [[nodiscard]] std::string format_number(const float value)
    {
        auto buffer = std::array<char, 32>{};
        std::snprintf(buffer.data(), buffer.size(), "%.3g", value);
        return buffer.data();
    }

    // Format a real-valued amplitude A as "A" (always signed via the caller's separator).
    [[nodiscard]] std::string amplitude_tex(const float amplitude)
    {
        return format_number(std::fabs(amplitude));
    }

    // Format the trigonometric argument n*omega*t + phase so that degenerate factors are dropped.
    [[nodiscard]] std::string argument_tex(const float n, const float phase)
    {
        auto out = std::string{};
        const auto abs_n = std::fabs(n);
        const auto n_is_one = std::fabs(abs_n - 1.f) < 1e-3f;
        const auto n_is_zero = abs_n < 1e-6f;
        if (n_is_zero)
            out += "0";
        else
        {
            if (!n_is_one)
                out += format_number(abs_n);
            out += R"(\omega t)";
        }

        const auto abs_phase = std::fabs(phase);
        const auto phase_is_zero = abs_phase < 1e-3f;
        if (!phase_is_zero)
        {
            out += phase < 0.f ? "-" : "+";
            out += format_number(abs_phase);
        }
        return out;
    }

    void append_term(std::string& out, const std::string_view sign, const float amplitude, const float n, const float phase)
    {
        out += sign;
        out += amplitude_tex(amplitude);
        const auto arg = argument_tex(n, phase);
        if (arg.find('+') != std::string::npos || arg.find('-') != std::string::npos)
            out += R"(\sin\left()" + arg + R"(\right))";
        else
            out += R"(\sin()" + arg + ")";
    }

    [[nodiscard]] HarmonicTerm term_at(const WaveVariant& wave, const float i)
    {
        return std::visit([&](const auto& w) { return w.formula(i); }, wave);
    }
} // namespace

std::string Waves::dynamic_formula_tex(const WaveVariant& wave, const unsigned int harmonic_count, const float radius,
    const float frequency, const bool* const enabled_mask)
{
    // Build omega = 2*pi*f, dropping the factor when degenerate.
    auto omega = std::string{};
    const auto f_is_one = std::fabs(frequency - 1.f) < 1e-3f;
    const auto f_is_zero = std::fabs(frequency) < 1e-6f;
    if (f_is_zero)
        omega = "0";
    else
    {
        if (!f_is_one)
            omega += format_number(frequency);
        omega += R"(2\pi)";
    }

    auto out = std::string{R"(f(t) = )"};

    // DC offset: radius * dc(wave).
    const auto dc = std::visit([](const auto& w) { return w.dc(); }, wave);
    const auto dc_amplitude = radius * dc;
    auto dc_written = false;
    if (std::fabs(dc_amplitude) > 1e-6f)
    {
        out += amplitude_tex(dc_amplitude);
        dc_written = true;
    }

    // Expand the first contributing terms, skip disabled/zero ones, collapse the rest.
    auto expanded = size_t{0};
    auto first_term = true;
    auto skipped_after_cap = false;
    for (auto i = 0u; i < harmonic_count; ++i)
    {
        const auto enabled = enabled_mask == nullptr || enabled_mask[i];
        if (!enabled)
            continue;

        const auto [n, coefficient, phase] = term_at(wave, static_cast<float>(i));
        const auto amplitude = radius * coefficient;
        if (std::fabs(amplitude) < 1e-6f)
            continue;

        if (expanded >= max_expanded_terms)
        {
            skipped_after_cap = true;
            continue;
        }

        const auto sign = (first_term && !dc_written) ? std::string_view{} :
                          (first_term && dc_written)  ? std::string_view{" + "} :
                                                        std::string_view{" + "};
        append_term(out, sign, amplitude, n, phase);
        ++expanded;
        first_term = false;
    }

    if (skipped_after_cap)
        out += R"( + \cdots)";

    // Attach the definition of omega below the equation so the displayed frequency is unambiguous.
    out += R"( \quad (\omega = )" + omega + ")";
    return out;
}
