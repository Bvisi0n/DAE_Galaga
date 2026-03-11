// TODO 4: Split this into seperate dedicated cpp files and have CMake handle the platform specific compilation.

#if WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <XInput.h>
#else
    #include <SDL3/SDL.h>
#endif

#include "Input/Gamepad.h"

namespace dae
{
    class Gamepad::GamepadImpl
    {
#if WIN32
    public:
        GamepadImpl(unsigned int index) : m_ControllerIndex(index)
        {
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        }

        void Update()
        {
            m_PreviousState = m_CurrentState;
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            DWORD result = XInputGetState(m_ControllerIndex, &m_CurrentState);

            if (result != ERROR_SUCCESS)
            {
                ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
                ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
                m_ButtonsPressedThisFrame = 0;
                m_ButtonsReleasedThisFrame = 0;
                m_IsConnected = false;
                return;
            }

            m_IsConnected = true;

            auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
            m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
        }

        bool IsDown(Gamepad::Button button) const
        {
            return m_ButtonsPressedThisFrame & static_cast<unsigned int>(button);
        }

        bool IsUp(Gamepad::Button button) const
        {
            return m_ButtonsReleasedThisFrame & static_cast<unsigned int>(button);
        }

        bool IsPressed(Gamepad::Button button) const
        {
            return m_CurrentState.Gamepad.wButtons & static_cast<unsigned int>(button);
        }

        bool IsConnected() const
        {
            return m_IsConnected;
        }

    private:
        unsigned int m_ControllerIndex;
        XINPUT_STATE m_CurrentState{};
        XINPUT_STATE m_PreviousState{};
        unsigned int m_ButtonsPressedThisFrame{};
        unsigned int m_ButtonsReleasedThisFrame{};
        bool m_IsConnected{};
#else
    public:
        explicit GamepadImpl(unsigned int index)
            : m_ControllerIndex(index) {}

        ~GamepadImpl()
        {
            if (m_pGamepad) SDL_CloseGamepad(m_pGamepad);
        }

        void Update()
        {
            if (!m_pGamepad)
            {
                int count;
                // TODO 4: Wrap this in a small struct/class for RAII.
                SDL_JoystickID* gamepads = SDL_GetGamepads(&count);
                if (gamepads && m_ControllerIndex < (unsigned int)count)
                {
                    m_pGamepad = SDL_OpenGamepad(gamepads[m_ControllerIndex]);
                }
                SDL_free(gamepads); // IMPORTANT!
            }

            if (!m_pGamepad)
            {
                return;
            }

            m_PreviousButtons = m_CurrentButtons;
            m_CurrentButtons = 0;

            // TODO 4: Could be optimized for cache.
            auto MapButton = [&](SDL_GamepadButton sdlButton, Gamepad::Button daeButton)
                {
                    if (SDL_GetGamepadButton(m_pGamepad, sdlButton))
                    {
                        m_CurrentButtons |= static_cast<unsigned int>(daeButton);
                    }
                };

            MapButton(SDL_GAMEPAD_BUTTON_SOUTH, Button::A);
            MapButton(SDL_GAMEPAD_BUTTON_EAST, Button::B);
            MapButton(SDL_GAMEPAD_BUTTON_WEST, Button::X);
            MapButton(SDL_GAMEPAD_BUTTON_NORTH, Button::Y);
            MapButton(SDL_GAMEPAD_BUTTON_START, Button::Start);
            MapButton(SDL_GAMEPAD_BUTTON_BACK, Button::Back);
            MapButton(SDL_GAMEPAD_BUTTON_LEFT_STICK, Button::LeftThumb);
            MapButton(SDL_GAMEPAD_BUTTON_RIGHT_STICK, Button::RightThumb);
            MapButton(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, Button::LeftShoulder);
            MapButton(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, Button::RightShoulder);
            MapButton(SDL_GAMEPAD_BUTTON_DPAD_UP, Button::DPadUp);
            MapButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, Button::DPadDown);
            MapButton(SDL_GAMEPAD_BUTTON_DPAD_LEFT, Button::DPadLeft);
            MapButton(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Button::DPadRight);
        }

        bool IsDown(Gamepad::Button button) const
        {
            return (m_CurrentButtons & static_cast<unsigned int>(button)) && !(m_PreviousButtons & static_cast<unsigned int>(button));
        }

        bool IsUp(Gamepad::Button button) const
        {
            return !(m_CurrentButtons & static_cast<unsigned int>(button)) && (m_PreviousButtons & static_cast<unsigned int>(button));
        }

        bool IsPressed(Gamepad::Button button) const
        {
            return m_CurrentButtons & static_cast<unsigned int>(button);
        }

        bool IsConnected() const
        {
            return m_pGamepad != nullptr;
        }

    private:
        unsigned int m_ControllerIndex;
        SDL_Gamepad* m_pGamepad{ nullptr };
        unsigned int m_CurrentButtons{};
        unsigned int m_PreviousButtons{};
#endif
    };

    Gamepad::Gamepad(unsigned int index)
        : m_pImpl(std::make_unique<GamepadImpl>(index)) {}

    Gamepad::~Gamepad() = default;

    void Gamepad::Update()
    {
        m_pImpl->Update();
    }

    bool Gamepad::IsDown(Button button) const
    {
        return m_pImpl->IsDown(button);
    }

    bool Gamepad::IsUp(Button button) const
    {
        return m_pImpl->IsUp(button);
    }

    bool Gamepad::IsPressed(Button button) const
    {
        return m_pImpl->IsPressed(button);
    }

    bool Gamepad::IsConnected() const
    {
        return m_pImpl->IsConnected();
    }
}
