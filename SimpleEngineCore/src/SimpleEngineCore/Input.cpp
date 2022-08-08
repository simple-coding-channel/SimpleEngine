#include "SimpleEngineCore/Input.hpp"

namespace SimpleEngine {
    bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
    bool Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};

    bool Input::IsKeyPressed(const KeyCode key_code)
    {
        return m_keys_pressed[static_cast<size_t>(key_code)];
    }

    void Input::PressKey(const KeyCode key_code)
    {
        m_keys_pressed[static_cast<size_t>(key_code)] = true;
    }

    void Input::ReleaseKey(const KeyCode key_code)
    {
        m_keys_pressed[static_cast<size_t>(key_code)] = false;
    }

    bool Input::IsMouseButtonPressed(const MouseButton mouse_button)
    {
        return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
    }

    void Input::PressMouseButton(const MouseButton mouse_button)
    {
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
    }

    void Input::ReleaseMouseButton(const MouseButton mouse_button)
    {
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
    }
}