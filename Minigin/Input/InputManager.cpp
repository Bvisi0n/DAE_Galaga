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
	for (auto& gamepad : m_pGamepads)
	{
		gamepad->Update();
	}

	auto checkState = [](KeyState state, auto isDown, auto isUp, auto isPressed) -> bool
		{
			switch (state)
			{
				case KeyState::Down:
					return isDown();
				case KeyState::Up:
					return isUp();
				case KeyState::Pressed:
					return isPressed();
				default:
					return false;
			}
		};

	for (auto& [binding, command] : m_pKeyboardCommands)
	{
		const auto& [key, state] = binding;
		if (checkState(state,	[&] { return m_pKeyboard->IsDown(key); },
								[&] { return m_pKeyboard->IsUp(key); },
								[&] { return m_pKeyboard->IsPressed(key); }))
		{
			command->Execute(deltaTime);
		}
	}
	for (auto& [binding, command] : m_pGamepadCommands)
	{
		const auto& [idx, btn, state] = binding;
		if (checkState(state,	[&] { return m_pGamepads[idx]->IsDown(btn); },
								[&] { return m_pGamepads[idx]->IsUp(btn); },
								[&] { return m_pGamepads[idx]->IsPressed(btn); }))
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
