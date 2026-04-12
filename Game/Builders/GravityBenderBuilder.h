#ifndef GRAVITYBENDERBUILDER_H
#define GRAVITYBENDERBUILDER_H

#include <memory>

#include "SDL3/SDL.h"

#include <glm/ext/vector_float2.hpp>

#include "Game/Commands/MoveCommand.h"
#include "Game/Commands/SpawnGravityFieldCommand.h"
#include "Game/Components/FPSComponent.h"
#include "Game/Components/ScreenWrapComponent.h"
#include "Game/Components/SpawnerPortalComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/PrimitiveRenderComponent.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/Keyboard.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"

namespace bvi::builders
{
	class GravityBenderBuilder final
	{
	public:
		GravityBenderBuilder() = delete;

		static void Build( dae::scenes::Scene& scene )
		{
			BuildBackground( scene );
			BuildViewportBorder( scene );
			BuildFPSCounter( scene );
			BuildPlayer( scene );
			BuildSpawner( scene );
			scene.Initialize();
		}

	private:
		static void BuildBackground( dae::scenes::Scene& scene )
		{
			auto background{ std::make_unique<dae::core::GameObject>() };
			background->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "background.png" );
			scene.AddGameObject( std::move( background ) );
		}

		static void BuildViewportBorder( dae::scenes::Scene& scene )
		{
			// TODO L: Fetch screen dimensions and use them.
			constexpr SDL_FRect screenBounds{ 0.f, 0.f, 1024.f, 576.f };
			constexpr SDL_Color neonPurple{ 128, 0, 128, 255 };

			auto borderObject{ std::make_unique<dae::core::GameObject>() };
			borderObject->AddComponent<dae::graphics::PrimitiveRenderComponent>( screenBounds, neonPurple );

			scene.AddGameObject( std::move( borderObject ) );
		}

		static void BuildFPSCounter( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 18 ) };
			auto fpsCounter{ std::make_unique<dae::core::GameObject>( 10.f, 10.f ) };
			fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font )->SetColor( SDL_Color{ 255, 0, 0, 255 } );
			fpsCounter->AddComponent<components::FPSComponent>();
			scene.AddGameObject( std::move( fpsCounter ) );
		}

		static void BuildPlayer( dae::scenes::Scene& scene )
		{
			auto player{ std::make_unique<dae::core::GameObject>( 400.f, 350.f ) };
			player->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "starfighter.png" );
			auto* moveComp = player->AddComponent<dae::core::MoveComponent>( 250.f, 2.f );
			player->AddComponent<bvi::components::ScreenWrapComponent>( 1024.f, 576.f );

			auto& input = dae::input::InputManager::GetInstance();

			input.BindCommand( dae::input::Keyboard::Key::W, dae::input::InputManager::KeyState::Pressed, std::make_unique<commands::MoveCommand>( moveComp, glm::vec3{ 0.0f, -400.0f, 0.0f } ) );

			input.BindCommand( dae::input::Keyboard::Key::S, dae::input::InputManager::KeyState::Pressed, std::make_unique<commands::MoveCommand>( moveComp, glm::vec3{ 0.0f, 400.0f, 0.0f } ) );

			input.BindCommand( dae::input::Keyboard::Key::A, dae::input::InputManager::KeyState::Pressed, std::make_unique<commands::MoveCommand>( moveComp, glm::vec3{ -400.0f, 0.0f, 0.0f } ) );

			input.BindCommand( dae::input::Keyboard::Key::D, dae::input::InputManager::KeyState::Pressed, std::make_unique<commands::MoveCommand>( moveComp, glm::vec3{ 400.0f, 0.0f, 0.0f } ) );

			input.BindCommand( dae::input::Keyboard::Key::Space, dae::input::InputManager::KeyState::Pressed, std::make_unique<commands::SpawnGravityFieldCommand>( player.get() ) );

			scene.AddGameObject( std::move( player ) );
		}

		static void BuildSpawner( dae::scenes::Scene& scene )
		{
			auto spawner{ std::make_unique<dae::core::GameObject>() };
			spawner->AddComponent<components::SpawnerPortalComponent>( blueprints::UnitData{} );
			spawner->AddComponent<dae::graphics::PrimitiveRenderComponent>( SDL_FRect{ 0.f, 0.f, 32.f, 32.f }, SDL_Color{ 255, 0, 0, 255 }, 2 );
			scene.AddGameObject( std::move( spawner ) );
		}
	};
}
#endif
