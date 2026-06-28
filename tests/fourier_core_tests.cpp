#include <print>
#include "math/math.hpp"
#include "waves/dft.hpp"
#include "waves/shapes/cosine.hpp"
#include "waves/shapes/pulse.hpp"
#include "waves/shapes/sawtooth.hpp"
#include "waves/shapes/square.hpp"
#include "waves/shapes/triangle.hpp"

namespace
{
    [[nodiscard]] auto nearly_equal(const float a, const float b, const float epsilon = 1e-4f) -> bool
    {
        return std::fabs(a - b) <= epsilon;
    }

    auto require(const bool condition, const std::string_view message) -> void
    {
        if (condition)
            return;

        std::println(stderr, "FAILED: {}", message);
        std::exit(EXIT_FAILURE);
    }

    auto test_wave_formulas() -> void
    {
        {
            const auto first = Waves::Square::formula(0.f);
            const auto third = Waves::Square::formula(1.f);
            require(nearly_equal(first.n, 1.f), "square first harmonic index");
            require(nearly_equal(first.coefficient, 4.f / math::pi_v<float>), "square first coefficient");
            require(nearly_equal(third.n, 3.f), "square second stored harmonic is n=3");
            require(nearly_equal(third.coefficient, 4.f / (3.f * math::pi_v<float>) ), "square n=3 coefficient");
        }

        {
            const auto first = Waves::Sawtooth::formula(0.f);
            const auto second = Waves::Sawtooth::formula(1.f);
            require(nearly_equal(first.n, 1.f), "sawtooth first harmonic index");
            require(nearly_equal(first.coefficient, 2.f / math::pi_v<float>), "sawtooth first coefficient");
            require(nearly_equal(second.n, 2.f), "sawtooth second harmonic index");
            require(nearly_equal(second.coefficient, 1.f / math::pi_v<float>), "sawtooth second coefficient");
        }

        {
            const auto first = Waves::Triangle::formula(0.f);
            const auto third = Waves::Triangle::formula(1.f);
            require(nearly_equal(first.n, 1.f), "triangle first harmonic index");
            require(nearly_equal(first.coefficient, 8.f / math::square(math::pi_v<float>)), "triangle first coefficient");
            require(nearly_equal(third.n, 3.f), "triangle second stored harmonic is n=3");
            require(nearly_equal(third.coefficient, -8.f / (9.f * math::square(math::pi_v<float>))), "triangle n=3 coefficient");
        }

        {
            const auto first = Waves::Cosine::formula(0.f);
            const auto second = Waves::Cosine::formula(1.f);
            require(nearly_equal(first.n, 1.f), "cosine first harmonic index");
            require(nearly_equal(first.coefficient, 1.f), "cosine first coefficient");
            require(nearly_equal(first.phase, math::pi_v<float> / 2.f), "cosine phase");
            require(nearly_equal(second.coefficient, 0.f), "cosine higher harmonics disabled");
        }

        {
            const auto first = Waves::Pulse25::formula(0.f);
            require(nearly_equal(Waves::Pulse25::dc(), 0.25f), "pulse 25 dc");
            require(nearly_equal(first.n, 1.f), "pulse 25 first harmonic index");
            require(nearly_equal(first.coefficient, (2.f / math::pi_v<float>) *std::sin(math::pi_v<float> * 0.25f)),
                "pulse 25 first coefficient");
            require(nearly_equal(first.phase, math::pi_v<float> / 2.f), "pulse 25 phase");
        }
    }

    auto test_resample_uniform() -> void
    {
        const auto square = std::vector<Vector2>{
            {0.f, 0.f},
            {2.f, 0.f},
            {2.f, 2.f},
            {0.f, 2.f},
        };

        const auto samples = Waves::resample_uniform(square, 4);
        require(samples.size() == 4u, "resample returns requested sample count");
        require(nearly_equal(samples[0].x, 0.f) && nearly_equal(samples[0].y, 0.f), "resample first corner");
        require(nearly_equal(samples[1].x, 2.f) && nearly_equal(samples[1].y, 0.f), "resample second corner");
        require(nearly_equal(samples[2].x, 2.f) && nearly_equal(samples[2].y, 2.f), "resample third corner");
        require(nearly_equal(samples[3].x, 0.f) && nearly_equal(samples[3].y, 2.f), "resample fourth corner");
    }

    auto test_compute_dft_circle() -> void
    {
        auto circle = std::vector<Vector2>{};
        constexpr auto sample_count = 128u;
        circle.reserve(sample_count);
        for (auto i = 0u; i < sample_count; ++i)
        {
            const auto angle = 2.f * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(sample_count);
            circle.push_back({std::cos(angle), std::sin(angle)});
        }

        const auto result = Waves::compute_dft(circle, 5);
        require(!result.harmonics.empty(), "circle DFT produces harmonics");
        require(nearly_equal(result.harmonics.front().n, 1.f, 1e-3f), "circle DFT dominant harmonic is n=1");
        require(nearly_equal(result.harmonics.front().coefficient, 1.f, 1e-4f), "circle DFT dominant harmonic is normalized");
        require(std::fabs(result.harmonics.front().phase) < 2e-2f, "circle DFT dominant harmonic phase");
    }
} // namespace

auto main() -> int
{
    test_wave_formulas();
    test_resample_uniform();
    test_compute_dft_circle();
    return 0;
}
