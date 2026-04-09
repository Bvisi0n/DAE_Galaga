#ifndef GRAVITYBENDERSTATE_H
#define GRAVITYBENDERSTATE_H

#include <memory>

#include "Game/Builders/GravityBenderBuilder.h"
#include "Game/Components/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/IGameState.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::states
{
	class GravityBenderState final : public dae::core::IGameState
	{
	public:
		GravityBenderState() = default;
		~GravityBenderState() override = default;

		GravityBenderState( const GravityBenderState& ) = delete;
		GravityBenderState( GravityBenderState&& ) = delete;
		GravityBenderState& operator=( const GravityBenderState& ) = delete;
		GravityBenderState& operator=( GravityBenderState&& ) = delete;

		void OnEnter() override
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().CreateScene();
			bvi::builders::GravityBenderBuilder::Build( scene );
		}

		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
			// TODO N: Don't forget to unbind the commands!
				// Maybe Commands should be wrapped in a RAII object that unbinds them when the scene gets destroyed? Ownership questions arise.
		}

		void Update() override
		{}
	};
}
#endif