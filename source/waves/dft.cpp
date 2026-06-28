//
// Created by nazim on 5/26/26.
//

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstddef>
#include <iterator>
#include <numbers>
#include <span>
#include <utility>
#include "dft.hpp"

using namespace Waves;

auto Waves::resample_uniform(const std::span<const Vector2> points, const std::size_t num_samples) -> std::vector<Vector2>
{
    if (points.size() < 2)
        return {};

    auto arc = std::vector(points.size() + 1, 0.f);
    for (auto i = std::size_t{0}; i < points.size(); ++i)
    {
        const auto& a = points[i];
        const auto& b = points[(i + 1) % points.size()];
        arc[i + 1] = arc[i] + std::hypot(b.x - a.x, b.y - a.y);
    }

    const auto total = arc.back();
    if (total == 0.f)
        return {};

    auto out = std::vector<Vector2>{};
    out.reserve(num_samples);
    for (auto k = std::size_t{}; k < num_samples; ++k)
    {
        const auto target = total * static_cast<float>(k) / static_cast<float>(num_samples);
        const auto upper = std::ranges::lower_bound(arc, target);
        const auto j = std::clamp<std::size_t>(
            upper == arc.begin() ? std::size_t{} : static_cast<std::size_t>(std::distance(arc.begin(), upper) - 1),
            std::size_t{},
            points.size() - 1u
        );

        const auto seg = arc[j + 1] - arc[j];
        const auto t = seg == 0.f ? 0.f : (target - arc[j]) / seg;
        const auto& a = points[j];
        const auto& b = points[(j + 1) % points.size()];
        out.emplace_back(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y));
    }

    return out;
}

auto Waves::compute_dft(const std::span<const Vector2> points, const std::size_t max_harmonics) -> DftResult
{
    if (points.size() < 3)
        return {.harmonics = {}, .dc_value = 0.f};

    constexpr auto target_samples = std::size_t{512};
    const auto resampled = resample_uniform(points, target_samples);
    if (resampled.empty())
        return {.harmonics = {}, .dc_value = 0.f};

    auto cx = 0.f;
    auto cy = 0.f;
    for (const auto& p : resampled)
    {
        cx += p.x;
        cy += p.y;
    }

    cx /= static_cast<float>(resampled.size());
    cy /= static_cast<float>(resampled.size());
    auto z = std::vector<std::complex<float>>{};
    z.reserve(resampled.size());
    for (const auto& p : resampled)
        z.emplace_back(p.x - cx, p.y - cy);

    const auto sample_count = z.size();
    const auto half = static_cast<int>(std::min(max_harmonics, sample_count / 2));
    auto coefficients = std::vector<std::pair<int, std::complex<float>>>{};
    coefficients.reserve(2 * static_cast<std::size_t>(half));
    for (auto n = -half; n <= half; ++n)
    {
        if (n == 0)
            continue;

        auto c = std::complex{0.f, 0.f};
        for (auto k = std::size_t{}; k < sample_count; ++k)
        {
            const auto angle =
                -2.f * std::numbers::pi_v<float> * static_cast<float>(n) * static_cast<float>(k) / static_cast<float>(sample_count);
            c += z[k] * std::complex{std::cos(angle), std::sin(angle)};
        }

        c /= static_cast<float>(sample_count);
        coefficients.emplace_back(n, c);
    }

    std::ranges::sort(coefficients, [](const auto& a, const auto& b) -> bool {
        return std::abs(a.second) > std::abs(b.second);
    });
    if (coefficients.size() > max_harmonics)
        coefficients.resize(max_harmonics);

    const auto max_mag = coefficients.empty() ? 0.f : std::abs(coefficients.front().second);
    if (max_mag == 0.f)
        return {.harmonics = {}, .dc_value = 0.f};

    auto result = DftResult{.harmonics = {}, .dc_value = 0.f};
    result.harmonics.reserve(coefficients.size());
    for (const auto& [n, c] : coefficients)
    {
        result.harmonics.push_back({
            .n = static_cast<float>(n),
            .coefficient = std::abs(c) / max_mag,
            .phase = std::arg(c),
        });
    }

    return result;
}
