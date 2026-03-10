#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>
#include <memory>
#include <vector>

#include "Commands/Command.h"
#include "Singletons/Singleton.h"
#include "Gamepad.h"

namespace dae
{
    enum class KeyState { Down, Up, Pressed };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();
        ~InputManager() = default;

        bool ProcessInput(const float deltaTime);
        void BindCommand(unsigned int controllerIndex, Gamepad::Button button, KeyState state, std::unique_ptr<dae::Command> pCommand);

        bool IsControllerConnected(unsigned int controllerIndex) const;

    private:
        using ControllerKey = std::pair<unsigned int, Gamepad::Button>;
        using CommandMap = std::map<std::pair<ControllerKey, KeyState>, std::unique_ptr<Command>>;

        CommandMap m_ConsoleCommands;
        std::vector<std::unique_ptr<Gamepad>> m_pGamepads;
    };
}
#endif
