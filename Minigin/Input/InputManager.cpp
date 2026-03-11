#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

#include "Input/InputManager.h"

dae::InputManager::InputManager()
	: m_pKeyboard(std::make_unique<Keyboard>())
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		m_pGamepads.push_back(std::make_unique<Gamepad>(i));
	}
}

bool dae::InputManager::ProcessInput(const float deltaTime)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			return false;
		}

		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	// Careful when moving these around, it's dependent on SDL_PollEvent(). Check Emscripten build if you do!
	// See https://youtu.be/TSlJ3dX5GCI?si=_Nb7xtJYRhaWbBvI&t=339 for info.

	m_pKeyboard->Update();

	for (auto& [binding, command] : m_pKeyboardCommands)
	{
		bool execute = false;
		switch (binding.second)
		{
			case KeyState::Down:
				execute = m_pKeyboard->IsDown(binding.first);
				break;
			case KeyState::Up:
				execute = m_pKeyboard->IsUp(binding.first);
				break;
			case KeyState::Pressed:
				execute = m_pKeyboard->IsPressed(binding.first);
				break;
		}

		if (execute)
		{
			command->Execute(deltaTime);
		}
	}

	for (auto& gamepad : m_pGamepads)
	{
		gamepad->Update();
	}

	for (auto& [binding, command] : m_pGamepadCommands)
	{
		const auto& [index, button, state] = binding;
		bool execute = false;
		switch (state)
		{
		case KeyState::Down:
			execute = m_pGamepads[index]->IsDown(button);
			break;
		case KeyState::Up:
			execute = m_pGamepads[index]->IsUp(button);
			break;
		case KeyState::Pressed:
			execute = m_pGamepads[index]->IsPressed(button);
			break;
		}

		if (execute)
		{
			command->Execute(deltaTime);
		}
	}

	return true;
}

bool dae::InputManager::IsControllerConnected(unsigned int controllerIndex) const
{
	if (controllerIndex >= m_pGamepads.size())
	{
		return false;
	}

	return m_pGamepads[controllerIndex]->IsConnected();
}
