#ifndef BVI_GRAVITYBENDERSTATE_H
#define BVI_GRAVITYBENDERSTATE_H

#include "Game/GravityBender/GravityBenderBuilder.h"
#include "Game/GravityBender/GravityRegistry.h"

#include "Minigin/Core/IAppState.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::gravity_bender
{
	class GravityBenderState final : public dae::core::IAppState
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
		}

		void Update( float deltaTime ) override
		{
			GravityRegistry::Update( deltaTime );
		}
	};
}
#endif