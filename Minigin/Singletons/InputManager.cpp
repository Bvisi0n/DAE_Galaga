#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

#include "Singletons/InputManager.h"

dae::InputManager::InputManager()
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		m_pGamepads.push_back(std::make_unique<Gamepad>(i));
	}
}

bool dae::InputManager::ProcessInput()
{
	for (auto& gamepad : m_pGamepads)
	{
		gamepad->Update();
	}

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			return false;
		}

		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	for (auto& [binding, command] : m_ConsoleCommands)
	{
		const auto& [key, state] = binding;
		const auto& [controllerIdx, button] = key;

		bool shouldExecute = false;
		const auto& gamepad = m_pGamepads[controllerIdx];

		switch (state)
		{
			case KeyState::Down:
				shouldExecute = gamepad->IsDown(button);
				break;
			case KeyState::Up:
				shouldExecute = gamepad->IsUp(button);
				break;
			case KeyState::Pressed:
				shouldExecute = gamepad->IsPressed(button);
				break;
		}

		if (shouldExecute)
		{
			command->Execute();
		}
	}

	return true;
}

void dae::InputManager::BindCommand(unsigned int controllerIndex, Gamepad::Button button, KeyState state, std::unique_ptr<dae::Command> pCommand)
{
	m_ConsoleCommands[{ {controllerIndex, button}, state }] = std::move(pCommand);
}

bool dae::InputManager::IsControllerConnected(unsigned int controllerIndex) const
{
	if (controllerIndex >= m_pGamepads.size())
	{
		return false;
	}

	return m_pGamepads[controllerIndex]->IsConnected();
}
