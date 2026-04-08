#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "Game/Builders/MenuBuilder.h"

#include "Minigin/Core/IGameState.h"
#include "Minigin/Scene/SceneManager.h"

namespace dae::core
{
	class MainMenuState final : public IGameState
	{
	public:
		void OnEnter() override
		{
			auto& scene = scenes::SceneManager::GetInstance().CreateScene();
			// TODO H: Engine cannot touch namespace bvi, something is wrong!
			bvi::builders::MenuBuilder::Build( scene );
		}
		void OnExit() override
		{
			scenes::SceneManager::GetInstance().RemoveAllScenes();
		}
		void Update() override
		{
			// TODO N: Listen for input to switch to other states.
		}
	};
}
#endif
