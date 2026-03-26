#if WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <XInput.h>
#else
    #include <SDL3/SDL.h>
#endif

#include <memory>

#include "Input/Gamepad.h"

// TODO L: Rework the SDL implementation.
//       XInput was made first and works as intended, SDL was then butchered to match it.
//       A result of this is a flawed controller identification logic.
//       SDL is event driven, currently burning cycles for no reason simulating XInput.

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

            auto button_changes = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
            m_ButtonsPressedThisFrame = button_changes & m_CurrentState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = button_changes & (~m_CurrentState.Gamepad.wButtons);
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
                std::unique_ptr<SDL_JoystickID, void(*)(void*)> gamepads(SDL_GetGamepads(&count), SDL_free);
                if (gamepads && m_ControllerIndex < static_cast<unsigned int>(count))
                {
                    m_pGamepad = SDL_OpenGamepad(gamepads.get()[m_ControllerIndex]);
                }
            }

            if (!m_pGamepad)
            {
                return;
            }

            m_PreviousButtons = m_CurrentButtons;
            m_CurrentButtons = 0;

            struct ButtonMapping
            {
                SDL_GamepadButton sdlButton;
                Gamepad::Button daeButton;
            };

            // https://wiki.libsdl.org/SDL3/SDL_GamepadButton
            static constexpr ButtonMapping mapping[] = {
                { SDL_GAMEPAD_BUTTON_SOUTH, Button::A },
                { SDL_GAMEPAD_BUTTON_EAST, Button::B },
                { SDL_GAMEPAD_BUTTON_WEST, Button::X },
                { SDL_GAMEPAD_BUTTON_NORTH, Button::Y },
                { SDL_GAMEPAD_BUTTON_START, Button::Start },
                { SDL_GAMEPAD_BUTTON_BACK, Button::Back },
                { SDL_GAMEPAD_BUTTON_LEFT_STICK, Button::LeftThumb },
                { SDL_GAMEPAD_BUTTON_RIGHT_STICK, Button::RightThumb },
                { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, Button::LeftShoulder },
                { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, Button::RightShoulder },
                { SDL_GAMEPAD_BUTTON_DPAD_UP, Button::DPadUp },
                { SDL_GAMEPAD_BUTTON_DPAD_DOWN, Button::DPadDown },
                { SDL_GAMEPAD_BUTTON_DPAD_LEFT, Button::DPadLeft },
                { SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Button::DPadRight }
            };

            for (const auto& m : mapping)
            {
                if (SDL_GetGamepadButton(m_pGamepad, m.sdlButton))
                {
                    m_CurrentButtons |= static_cast<unsigned int>(m.daeButton);
                }
            }
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
