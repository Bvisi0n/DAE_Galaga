#ifndef GRAVITYBENDERSTATE_H
#define GRAVITYBENDERSTATE_H

#include <memory>

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

			//TODO N: Use a builder instead of doing it here.
			ConstructDebugUI( scene );
			scene.Initialize();
		}

		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
		}

		void Update() override
		{}

	private:
		void ConstructDebugUI( dae::scenes::Scene& scene )
		{
			auto fpsEntity{ std::make_unique<dae::core::GameObject>() };

			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 18 ) };

			fpsEntity->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font )->SetColor( SDL_Color{ 255, 0, 0, 255 } );
			fpsEntity->AddComponent<bvi::components::FPSComponent>();

			scene.AddGameObject( std::move( fpsEntity ) );
		}
	};
}
#endif