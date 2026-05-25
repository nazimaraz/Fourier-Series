# Fourier Series

An interactive Fourier-series visualiser written in modern C++23. Drag the sliders
to add harmonics, watch nested epicycles synthesise a waveform in real time, and
see — at the bottom of the list — a Heart shape traced out by the **complex**
Fourier series, using exactly the same dispatch machinery as every other wave.

It compiles to a native binary on desktop and to WebAssembly via Emscripten.

---

## What the project demonstrates

This is a portfolio project for showcasing C++ design taste, not just feature
breadth. The interesting question is not "did you visualise a Fourier series"
(it's a well-known exercise), but **how the dispatch is structured underneath**
— and the headline claim is that the same architecture handles 1-D wave
synthesis and 2-D path tracing without any new code path.

Highlights, with file pointers:

| Technique | Where to read it | Why it matters |
|---|---|---|
| `std::variant` over a **closed set of stateless type tags** | [`source/waves/wave_variant.hpp`](source/waves/wave_variant.hpp) | Polymorphism without inheritance, vtables, or heap allocation. Each alternative is 1 byte; the whole variant is 2 bytes and trivially copyable. |
| **C++20 concept** (`WaveShape`) constraining the dispatch | [`source/waves/wave_shape.hpp`](source/waves/wave_shape.hpp) | Misuse fails with a precise concept error at the call site instead of a 40-line template instantiation wall inside `compute_for`. |
| `std::visit` with a **template lambda** | [`source/waves/wave_variant.cpp`](source/waves/wave_variant.cpp) | One lambda body, eleven compiler-specialised instantiations, zero virtual dispatch. |
| **Compile-time factory table** via `std::index_sequence` + pack expansion | `make_factories` in [`source/waves/wave_variant.cpp`](source/waves/wave_variant.cpp) | Runtime `index → variant` lookup is one array load + one indirect call; the table itself lives in `.rodata`. |
| **Compile-time name table** derived from variant alternatives | `wave_names`, `wave_names_cstr` in [`source/waves/wave_variant.tpp`](source/waves/wave_variant.tpp) | The combo-box labels regenerate automatically when a new shape is added. No DRY violation. |
| `std::in_place_index<I>` for **move-deleted in-place construction** | `make_factories` lambda | Each variant alternative used to inherit a `Wave` base with `= delete`d copy/move; `in_place_index` constructs the alternative directly inside the variant's storage. The base is gone now, but the pattern is preserved as the right tool. |
| **Non-type template parameter (NTTP)** for shapes parameterised at compile time | [`source/waves/shapes/pulse.hpp`](source/waves/shapes/pulse.hpp) | `Pulse<25>` and `Pulse<75>` are distinct types but each remains a 1-byte stateless tag. |
| **Compile-time string composition** | `name_storage` IIFE inside `Pulse<N>` | The display name `"Pulse (25%)"` is built at template-instantiation time by an immediately-invoked lambda writing into a `static constexpr std::array<char>` — no runtime formatting, no `std::format` (not `constexpr` until C++26). |
| **Compile-time design invariants** as `static_assert`s | bottom of [`source/waves/wave_variant.hpp`](source/waves/wave_variant.hpp) | `is_trivially_copyable`, `sizeof <= 16`, "every alternative satisfies `WaveShape`" — checked by the build. Break one and the compile fails. |
| **2-D path tracing** as a free extension of the same architecture | [`source/waves/shapes/heart.hpp`](source/waves/shapes/heart.hpp) + [`source/renderers/wave_renderer.cpp`](source/renderers/wave_renderer.cpp) | The same epicycle accumulator that synthesises a square wave also synthesises a Heart by taking complex coefficients (with negative `n` for clockwise rotation). Zero new code path. |

The rest of this README explains *why* these choices over the more obvious
ones.

---

## Design notes

### Why `std::variant` over `shared_ptr<Wave>` inheritance

An earlier iteration of the project had a `Wave` base class with virtual
`get_formula()` / `get_dc()`, and `Settings` stored a
`std::unordered_map<Waves::Type, std::shared_ptr<Wave>>` of wave instances.

That design has four costs the project doesn't need to pay:

1. **Open extensibility we don't need.** The set of wave types is closed —
   they ship with the binary, and end-users select one from a combo box.
   Virtual dispatch buys runtime extensibility (load a plugin from a `.so`);
   we don't.
2. **Heap allocation + reference counting per wave.** `shared_ptr` is the
   wrong ownership model: there's exactly one `Settings`, it owns the waves;
   nobody else needs a share. The atomic ref-count is pure overhead.
3. **Indirect call per harmonic per frame.** With ~10 harmonics × 120 FPS,
   that's ~1200 virtual calls per second. Negligible cost, but also
   *unnecessary* — `std::visit` over a variant dispatches to a direct,
   inline-friendly call.
4. **Per-instance state we never used.** `Wave` carried a `std::string name_`,
   a `shared_ptr<Settings>` back-reference, and a `Type type_` enum. None of
   it was read except for the combo box label, which is now a compile-time
   `string_view` on each type.

The variant design fixes all four:

```cpp
using WaveVariant = std::variant<
    Sawtooth, Square, Triangle, Semicircle,
    HalfWaveRectifiedSine, FullWaveRectifiedSine,
    Parabolic, Cosine,
    Pulse25, Pulse75,
    Heart>;
```

Each alternative is a stateless 1-byte type tag; `WaveVariant` is 2 bytes and
trivially copyable. The "is this still tiny?" claim is enforced by:

```cpp
static_assert(std::is_trivially_copyable_v<WaveVariant>);
static_assert(sizeof(WaveVariant) <= 16,
    "WaveVariant alternatives are stateless type tags; "
    "the variant should fit in a couple of bytes");
```

The trade-off is real and worth naming: the variant must list every alternative
at compile time. Users can't add a new wave at runtime. For this project
that's the right side of the trade.

### Why a concept (`WaveShape`)

`std::visit` is happy to dispatch to anything callable; nothing in the
language stops you from adding a broken type to the variant. The concept makes
the interface a first-class object:

```cpp
template <typename W>
concept WaveShape = requires(float i) {
    { W::name }       -> std::convertible_to<std::string_view>;
    { W::dc() }       -> std::same_as<float>;
    { W::formula(i) } -> std::same_as<HarmonicTerm>;
};

template <WaveShape W>
ComputeResult compute_for(const ComputeParams& params);
```

A `static_assert` at namespace scope fold-expression-checks that *every*
variant alternative satisfies the concept:

```cpp
static_assert(
    []<size_t... Is>(std::index_sequence<Is...>) {
        return (WaveShape<std::variant_alternative_t<Is, WaveVariant>> && ...);
    }(std::make_index_sequence<wave_count>{}),
    "every WaveVariant alternative must satisfy the WaveShape concept");
```

When a broken type is added, the diagnostic is four lines and points directly
at the missing member, instead of a wall of template instantiations inside
`compute_for`'s body.

### Why the factory table is a `static constexpr` array of function pointers

The dispatch problem: given a runtime `size_t index ∈ [0, N)`, construct
`std::variant_alternative_t<index, WaveVariant>{}` and return it as a
`WaveVariant`. The standard library doesn't provide this directly.

The textbook approach is a chain of `if`/`switch`. The pattern used here is
prettier and more maintainable: build a constant table of factory function
pointers at compile time, one per alternative.

```cpp
template <size_t... Is>
constexpr auto make_factories(std::index_sequence<Is...>) {
    return std::array<WaveVariant (*)(), sizeof...(Is)>{
        +[]() -> WaveVariant {
            return WaveVariant{std::in_place_index<Is>};
        }...
    };
}
```

Three idioms compress into that one expression:

- `std::index_sequence` provides a compile-time integer pack.
- Pack expansion `[]...` instantiates one captureless lambda per `Is`, each
  capturing a different non-type template argument.
- Unary `+` converts each captureless lambda to a plain
  `WaveVariant(*)()` so they share a type and can sit in one `std::array`.

Adding a new wave to the variant extends the table by one entry automatically.
`make_wave_at(i)` then resolves to one array load and one indirect call.

`std::in_place_index<I>` is necessary because each wave type formerly inherited
a `Wave` base with deleted copy/move; the `in_place` constructor builds the
alternative directly inside the variant's storage. Even after the base was
removed, the pattern is preserved as the correct one — it documents intent.

### Why the phase clock is `∫ f dt`, not `f · t`

A subtle DSP bug worth knowing about: if the wave's angle is computed as
`n · 2π · f · t`, dragging the frequency slider doesn't smoothly shift the
wave — it scrambles the high harmonics. For harmonic `n = 10` at `t = 10s`,
a tiny `Δf = 0.01` shifts the phase by `n · 2π · Δf · t = 2π` — a full
rotation. Visually, the wave becomes noise while the slider moves.

The fix is to accumulate a *phase* rather than reading off a clock:

```cpp
// each frame, before dispatch:
phase += frequency * delta_t;
// inside the harmonic loop:
const auto angle = n * 2π * phase + per_harmonic_phase;
```

`phase` evolves smoothly; changing `frequency` only affects *future*
evolution, never the past. The visualisation is rock-steady through the entire
slider range.

This is the standard phase-accumulator pattern from DSP. See
[`Settings::set_phase`](source/gui/settings.hpp) for the storage and
[`GUI::update_impl`](source/gui/gui.cpp) for the accumulation.

### Why a non-type template parameter for `Pulse<N>`

The classical resume-checklist version of this would be either a
`std::function<HarmonicTerm(float, float)>` per wave (one parameter for the
harmonic index, another for the duty cycle) or a `Pulse` struct with a
runtime `float duty` member.

Both work; both are bigger than they need to be. `Pulse<25>` and `Pulse<75>`
have *known-at-compile-time* duty cycles, so the cleanest expression is to
bake the parameter into the type:

```cpp
template <int DutyPercent>
struct Pulse {
    static_assert(DutyPercent > 0 && DutyPercent < 100);
    static constexpr std::string_view name = /* built at compile time */;
    static HarmonicTerm formula(float i);  // uses DutyPercent
    static constexpr float dc() { return DutyPercent / 100.f; }
};
```

Each instantiation is still a 1-byte stateless type tag. `sizeof(WaveVariant)`
doesn't grow when more duty cycles are added — they're all the same size on
the runtime side, even though they're distinct types on the compile-time side.

The display name `"Pulse (25%)"` is composed at template-instantiation time
by an immediately-invoked lambda writing into a
`static constexpr std::array<char, 16>`, then exposed as a `string_view`
viewing into it. No runtime formatting, no allocation, no `std::format`
(which isn't `constexpr` until C++26).

The trade-off is that you can't drag a slider through `Pulse<25>` →
`Pulse<26>` — those are different types. For a closed set of canonical duty
cycles that's fine; for runtime-adjustable parameters a runtime-state
alternative would be the answer.

### Why 2-D path tracing is a free extension

The headline architectural claim: **the same architecture that synthesises a
square wave can also draw a heart with no new dispatch code.**

The reasoning: the epicycle accumulator does

```cpp
position += {radius * cos(angle), radius * sin(angle)};
```

which is the polar form of the complex exponential
`r · exp(i · angle) = c_n · exp(i · n · ω · t)`. That's *already* a complex
Fourier synthesis — it just happens that for 1-D waveforms the renderer
throws away the X component and only plots `tip.y`.

The complex Fourier series of a 2-D closed curve `z(t) = x(t) + i · y(t)`
has the same form, with two differences:

1. The `n`s span both positive and negative integers (positive `n` is a
   counter-clockwise epicycle, negative `n` is clockwise).
2. The coefficients are complex — magnitudes set the radii, arguments set
   the phases.

Both are already representable in `HarmonicTerm { n, coefficient, phase }`.
`n` can be negative (`cos(−x) = cos(x)`, `sin(−x) = −sin(x)`); the phase
field shifts the sin into an arbitrary complex argument.

Adding the Heart was 22 lines of code in
[`shapes/heart.hpp`](source/waves/shapes/heart.hpp) plus a short
implementation: eight hand-derived complex coefficients (from the parametric
heart curve `x(t) = 16 sin³ t`, `y(t) = 13 cos t − 5 cos 2t − 2 cos 3t − cos 4t`)
encoded as `HarmonicTerm`s with `n ∈ {±1, ±2, ±3, ±4}`.

The only rendering addition was an 11-line block in `WaveRenderer::draw()`
that pushes the cumulative tip into a `circular_buffer<raylib::Vector2>`
and draws it as a line strip. Everything else — the variant alternative,
the concept check, the factory table, the combo-box label — extended itself.

---

## Wave catalogue

The eleven shapes shipped with the project:

| Shape | What it is | Spectrum decay | Notes |
|---|---|---|---|
| Sawtooth | `(2/π) Σ sin(nωt)/n` | 1/n | Slow decay, sharp discontinuity. |
| Square | `(4/π) Σ sin(nωt)/n` for odd `n` | 1/n | Classic; visible Gibbs ringing. |
| Triangle | `(8/π²) Σ (−1)ᵏ sin((2k+1)ωt)/(2k+1)²` | 1/n² | Continuous but kinked. |
| Semicircle | Bessel-function coefficients | varies | Bumpy "rolling-circle" feel. |
| Half-Wave Rectified Sine | `1/π + ½ sin(ωt) − (2/π) Σ cos(2kωt)/(4k²−1)` | 1/n² | Non-zero DC. |
| Full-Wave Rectified Sine | `2/π − (4/π) Σ cos(2kωt)/(4k²−1)` | 1/n² | The `|sin|` shape. |
| Parabolic | `(32/π³) Σ (−1)ᵏ cos((2k+1)ωt)/(2k+1)³` | 1/n³ | Looks like a sine after just 2 harmonics; demonstrates smoothness ↔ decay. |
| Cosine | One harmonic, `n=1`, `phase=π/2` | n/a | The trivial Fourier series — one term. |
| Pulse (25%) / Pulse (75%) | `(2/πn) sin(πnD)` cosine basis | varies with D | NTTP-parameterised; spectrum changes with duty cycle. |
| Heart | Eight complex coefficients, `n ∈ {±1..±4}` | n/a | 2-D path; tip traces a heart over one period. |

Compare Square (1/n) to Triangle (1/n²) to Parabolic (1/n³) at the same harmonic
count — the smoother the original function, the faster the Fourier series
concentrates in the fundamental. Parabolic looks like a pure sine after just
two harmonics; Square still rings visibly at fifty.

---

## Architecture map

```
┌──────────────────────────────────────────────────────────┐
│  GUI (gui.hpp/cpp, main.cpp)                              │
│   - raylib window + ImGui controls                        │
│   - owns Settings, owns WaveRenderer                      │
└──────────────────────────────────────────────────────────┘
          │                                ▲
          ▼                                │
┌─────────────────────┐          ┌──────────────────────┐
│   Settings           │   ◄─────│   WaveRenderer        │
│   - selected_wave_   │          │   - draw():           │
│     index_           │          │     compute() +       │
│   - radius, phase,   │          │     epicycle draw +   │
│     harmonic_count   │          │     1-D wave +        │
│   - wave_, path_     │          │     2-D path trace    │
└─────────────────────┘          └──────────────────────┘
          │                                ▲
          ▼                                │
┌──────────────────────────────────────────────────────────┐
│  Waves (waves/)                                           │
│   - WaveVariant = std::variant<Sawtooth, ..., Heart>      │
│   - WaveShape concept (wave_shape.hpp)                    │
│   - free compute(variant, params) via std::visit          │
│   - per-shape structs in shapes/*.hpp                     │
└──────────────────────────────────────────────────────────┘
```

The dependency arrows point strictly downward. `Waves::` knows nothing about
`Settings` or `WaveRenderer`; `Settings` knows about `WaveVariant` (it stores
the index) but nothing about rendering; `WaveRenderer` reads `Settings` by
reference and calls `Waves::compute`.

---

## Building

### Native

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/source/Fourier-Series
```

Requires a C++23 compiler (tested with Clang 20 / GCC 15.2) and Boost (for
`circular_buffer`).

### WebAssembly

```bash
emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web
# open build-web/source/Fourier-Series.html in a browser
```

A pre-built `Fourier-Series.html` / `.js` / `.wasm` are checked in at the
repo root for quick demo.

---

## Adding a new wave

Two files plus one variant-alias edit:

1. Create `source/waves/shapes/<name>.hpp` with a `struct <Name>` exposing
   `static constexpr std::string_view name`,
   `static HarmonicTerm formula(float)`, and
   `static constexpr float dc()`.
2. Create `source/waves/shapes/<name>.cpp` with the `formula(float)` body
   (only needed if it isn't inline).
3. Add `<Name>` to the `WaveVariant` alias in
   `source/waves/wave_variant.hpp`.

Everything downstream — name table, combo box, factory table, concept
verification — extends itself. The `static_assert` chain refuses to compile
if the new shape doesn't satisfy `WaveShape`.

---

## Code quality

- **`clang-format`** ([.clang-format](.clang-format)) — enforced consistent
  style.
- **`clang-tidy`** ([.clang-tidy](.clang-tidy)) — curated check set over
  `bugprone-*`, `cert-*`, `clang-analyzer-*`, `concurrency-*`,
  `cppcoreguidelines-*`, `misc-*`, `modernize-*`, `performance-*`,
  `portability-*`, `readability-*`. Zero warnings on user code.
- **`-Wall -Wextra -Wpedantic -Werror`** in
  [`cmake/CompileOptions.cmake`](cmake/CompileOptions.cmake).
- **`[[nodiscard]]`** on every getter, pure function, and factory.
- **`static_assert`** for design invariants that need to stay true through
  refactors.

---

## License

MIT — see [LICENSE](LICENSE).
