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
        DrawingInput(const DrawingInput&) = delete;
        DrawingInput(DrawingInput&&) = delete;
        DrawingInput& operator=(const DrawingInput&) = delete;
        DrawingInput& operator=(DrawingInput&&) = delete;
        ~DrawingInput();

        void handle() const;

    private:
        void commit() const;

        Settings& settings_;
    };
} // namespace UI
