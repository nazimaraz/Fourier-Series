//
// Created by nazim on 6/27/26.
//

#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <utility>
#include <sstream>
#include <string>
#include <string_view>
#include "formula_tex.hpp"
#include "wave_variant.hpp"

using namespace Waves;

namespace
{
    template <std::size_t... Is>
    [[nodiscard]] constexpr auto make_formula_table(std::index_sequence<Is...> /*index_sequence*/)
        -> std::array<std::string_view, wave_count>
    {
        return {std::variant_alternative_t<Is, WaveVariant>::latex...};
    }

    constexpr auto formulas = make_formula_table(std::make_index_sequence<wave_count>{});
} // namespace

auto Waves::formula_tex(const std::size_t wave_index) -> std::string_view
{
    if (wave_index >= wave_count)
        return {};

    return formulas.at(wave_index);
}

namespace
{
    [[nodiscard]] auto format_number(const float value) -> std::string
    {
        auto stream = std::ostringstream{};
        stream << std::setprecision(3) << value;
        return stream.str();
    }

    // Replace every occurrence of `needle` in `haystack` with `replacement`.
    auto replace_all(std::string& haystack, const std::string_view needle, const std::string_view replacement) -> void
    {
        if (needle.empty())
            return;

        for (auto pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + replacement.size()))
            haystack.replace(pos, needle.size(), replacement);
    }

    [[nodiscard]] auto rewrite_time_tokens(std::string formula, const std::string_view replacement) -> std::string
    {
        replace_all(formula, R"(\ldots)", "\x01");
        replace_all(formula, R"(\cdots)", "\x02");
        replace_all(formula, "t", replacement);
        replace_all(formula, "\x01", R"(\ldots)");
        replace_all(formula, "\x02", R"(\cdots)");
        return formula;
    }

    // Show the transformed input symbolically on the left and plug the numeric frequency into the series.
    [[nodiscard]] auto rewrite_time_variable(std::string formula, const float frequency) -> std::string
    {
        const auto equals_pos = formula.find('=');
        if (equals_pos == std::string::npos)
            return rewrite_time_tokens(std::move(formula), R"(2\pi ft)");

        auto lhs = formula.substr(0, equals_pos);
        auto rhs = formula.substr(equals_pos + 1);
        lhs = rewrite_time_tokens(std::move(lhs), R"(2\pi f t)");
        rhs = rewrite_time_tokens(std::move(rhs), R"(2\pi )" + format_number(frequency) + "t");
        return lhs + "=" + rhs;
    }
} // namespace

auto Waves::dynamic_formula_tex(const std::size_t wave_index, const unsigned int harmonic_count, const float radius,
    const float frequency) -> std::string
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
