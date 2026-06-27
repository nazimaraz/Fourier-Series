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
        void handle() const;

    private:
        void commit() const;

        Settings& settings_;
    };
} // namespace UI
