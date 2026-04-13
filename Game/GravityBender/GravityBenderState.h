#ifndef GRAVITYBENDERSTATE_H
#define GRAVITYBENDERSTATE_H

#include "Game/GravityBender/GravityBenderBuilder.h"

#include "Minigin/Core/IGameState.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::gravity_bender
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
			GravityBenderBuilder::Build( scene );
		}

		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
			// TODO N: Don't forget to unbind the commands!
				// Maybe Commands should be wrapped in a RAII object that unbinds them when the scene gets destroyed? Ownership questions arise.
				// Or is the MainMenu approach better? (bind and unbind in the state class) but then how do we create the binds?
				// A case of shared pointers like with textures and fonts?
		}

		void Update() override
		{}
	};
}
#endif