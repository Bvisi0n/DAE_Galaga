// This could & should be done in CMake but I'm secretly a first year and have no formal training in CMake.
// TODO 4: Split this into seperate dedicated cpp files and have CMake handle the platform specific compilation.

#if WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <XInput.h>
#endif

#include "Gamepad.h"

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
        GamepadImpl(unsigned int /*index*/) {}
        void Update() {}
        bool IsDown(Gamepad::Button /*button*/) const { return false; }
        bool IsUp(Gamepad::Button /*button*/) const { return false; }
        bool IsPressed(Gamepad::Button /*button*/) const { return false; }
        bool IsConnected() const { return false; }
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
