//
// Created by nazim on 5/26/26.
//

#pragma once

namespace UI
{
    class Settings;

    class DrawingInput
    {
    public:
        explicit DrawingInput(Settings& settings);
        auto handle() const -> void;

    private:
        auto commit() const -> void;

        Settings& settings_;
    };
} // namespace UI
