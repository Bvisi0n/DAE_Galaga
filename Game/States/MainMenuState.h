#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "Game/Builders/MenuBuilder.h"

#include "Minigin/Core/IGameState.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::states
{
	class MainMenuState final : public dae::core::IGameState
	{
	public:
		void OnEnter() override
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().CreateScene();
			bvi::builders::MenuBuilder::Build( scene );
		}
		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
		}
		void Update() override
		{
			// TODO N: Listen for input to switch to other states.
		}
	};
}
#endif
