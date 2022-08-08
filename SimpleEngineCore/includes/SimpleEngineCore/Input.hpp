#pragma once

#include "Keys.hpp"

namespace SimpleEngine {

    class Input {
    public:
        static bool IsKeyPressed(const KeyCode key_code);
        static void PressKey(const KeyCode key_code);
        static void ReleaseKey(const KeyCode key_code);

        static bool IsMouseButtonPressed(const MouseButton mouse_button);
        static void PressMouseButton(const MouseButton mouse_button);
        static void ReleaseMouseButton(const MouseButton mouse_button);

    private:
        static bool m_keys_pressed[];
        static bool m_mouse_buttons_pressed[];
};
}