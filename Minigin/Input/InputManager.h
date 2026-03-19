#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>
#include <memory>
#include <vector>
#include <type_traits>

#include "Commands/Command.h"
#include "Input/Gamepad.h"
#include "Input/Keyboard.h"
#include "Singletons/Singleton.h"

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
    enum class KeyState { Down, Up, Pressed };
        InputManager();
        ~InputManager() = default;

        bool ProcessInput(const float deltaTime);

        // TODO N: Use concepts.
        template <typename T>
        void BindCommand(T inputType, KeyState state, std::unique_ptr<Command> pCommand, unsigned int controllerIndex = 0)
        {
            if constexpr (std::is_same_v<T, Keyboard::Key>)
            {
                m_pKeyboardCommands[{inputType, state}] = std::move(pCommand);
            }
            else if constexpr (std::is_same_v<T, Gamepad::Button>)
            {
                m_pGamepadCommands[{controllerIndex, inputType, state}] = std::move(pCommand);
            }
        }

        // TODO N: Use concepts.
        template <typename T>
        void UnbindCommand(T inputType, KeyState state, unsigned int controllerIndex = 0)
        {
            if constexpr (std::is_same_v<T, Keyboard::Key>)
            {
                m_pKeyboardCommands.erase({ inputType, state });
            }
            else if constexpr (std::is_same_v<T, Gamepad::Button>)
            {
                m_pGamepadCommands.erase({ controllerIndex, inputType, state });
            }
        }

        bool IsControllerConnected(unsigned int controllerIndex) const;

    private:
        struct ControllerKey
        {
            unsigned int controllerIndex;
            Gamepad::Button button;
        };

        using ControllerBinding = std::tuple<unsigned int, Gamepad::Button, KeyState>;
        using KeyboardBinding = std::pair<Keyboard::Key, KeyState>;

        // TODO N: Use std::vector and Binding struct?
        //       Data locality, cache misses, 1 command per button limit.
        std::map<ControllerBinding, std::unique_ptr<Command>> m_pGamepadCommands;
        std::map<KeyboardBinding, std::unique_ptr<Command>> m_pKeyboardCommands;

        std::unique_ptr<Keyboard> m_pKeyboard;
        std::vector<std::unique_ptr<Gamepad>> m_pGamepads;
    };
}
#endif
