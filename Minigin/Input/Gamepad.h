#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <memory>

namespace dae::input
{
    class Gamepad final
    {
    public:
        enum class Button : unsigned short
        {
            // These values represent XInput button bitmasks, don't change without thinking.
            // Add ButtonMapping logic if you do. (like for the SDL implementation)
            // https://learn.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
            DPadUp          = 0x0001,
            DPadDown        = 0x0002,
            DPadLeft        = 0x0004,
            DPadRight       = 0x0008,
            Start           = 0x0010,
            Back            = 0x0020,
            LeftThumb       = 0x0040,
            RightThumb      = 0x0080,
            LeftShoulder    = 0x0100,
            RightShoulder   = 0x0200,
            A               = 0x1000,
            B               = 0x2000,
            X               = 0x4000,
            Y               = 0x8000
        };

        explicit Gamepad(unsigned int controllerIndex);
        ~Gamepad();

        void Update();

        bool IsDown(Button button) const;
        bool IsUp(Button button) const;
        bool IsPressed(Button button) const;

        bool IsConnected() const;

    private:
        class GamepadImpl;
        std::unique_ptr<GamepadImpl> m_pImpl;
    };
}
#endif
