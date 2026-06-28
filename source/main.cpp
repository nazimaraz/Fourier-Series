//
// Created by nazim on 18/10/2025.
//

#include "gui/gui.hpp"

auto main() -> int
{
    auto gui = GUI{};
    gui.initialize();
    while (!gui.is_stopped())
        gui.update();

    return 0;
}
