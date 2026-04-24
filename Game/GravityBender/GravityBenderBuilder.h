#ifndef BVI_GRAVITYBENDERBUILDER_H
#define BVI_GRAVITYBENDERBUILDER_H

#include <memory>
#include <utility>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include "Game/Common/FPSComponent.h"
#include "Game/GravityBender/GravityBenderBlueprints.h"
#include "Game/GravityBender/PlayerComponent.h"
#include "Game/GravityBender/ScreenWrapComponent.h"
#include "Game/GravityBender/SpawnerPortalComponent.h"

#include "Minigin/Core/ColliderComponent.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Graphics/PrimitiveRenderComponent.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"

namespace bvi::gravity_bender
{
	class GravityBenderBuilder final
	{
	public:
		GravityBenderBuilder() = delete;

		static void Build( dae::scenes::Scene& scene )
		{
			BuildViewportBorder( scene );
			BuildInstructions( scene );
			BuildFPSCounter( scene );
			BuildPlayer( scene );
			BuildSpawner( scene );
			EnableCollisions( scene );
			scene.Initialize();
		}

	private:
		static void BuildViewportBorder( dae::scenes::Scene& scene )
		{
			// TODO bvi_gravity_bender - Fetch screen dimensions and use them.
			constexpr SDL_FRect screenBounds{ 0.f, 0.f, 1024.f, 576.f };
			constexpr SDL_Color neonPurple{ 138, 43, 226, 255 };

			auto borderObject{ std::make_unique<dae::core::GameObject>() };

			borderObject->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::RectShape{ screenBounds, false } }, neonPurple, 2 );

			scene.AddGameObject( std::move( borderObject ) );
		}

		static void BuildInstructions( dae::scenes::Scene& scene )
		{
			SDL_Color fontColor{ 255, 255, 255, 255 };
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 16 ) };
			auto line{ std::make_unique<dae::core::GameObject>( 90.f, 10.f ) };
			line->AddComponent<dae::graphics::TextComponent>( "Welcome Commander, you're just in time!", font )->SetColor( fontColor );
			scene.AddGameObject( std::move( line ) );

			line = std::make_unique<dae::core::GameObject>( 10.f, 30.f );
			line->AddComponent<dae::graphics::TextComponent>( "Make the red rectangles crash into eachother before they overwhelm the system!", font )->SetColor( fontColor );
			scene.AddGameObject( std::move( line ) );

			line = std::make_unique<dae::core::GameObject>( 10.f, 50.f );
			line->AddComponent<dae::graphics::TextComponent>( "Use WASD to move", font )->SetColor( fontColor );
			scene.AddGameObject( std::move( line ) );

			line = std::make_unique<dae::core::GameObject>( 10.f, 70.f );
			line->AddComponent<dae::graphics::TextComponent>( "Use SPACE to place a temporary gravity well at your location", font )->SetColor( fontColor );
			scene.AddGameObject( std::move( line ) );
		}

		static void BuildFPSCounter( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 18 ) };
			auto fpsCounter{ std::make_unique<dae::core::GameObject>( 10.f, 10.f ) };
			fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font )->SetColor( SDL_Color{ 150, 200, 150, 255 } );
			fpsCounter->AddComponent<common::FPSComponent>();
			scene.AddGameObject( std::move( fpsCounter ) );
		}

		static void BuildPlayer( dae::scenes::Scene& scene )
		{
			auto player{ std::make_unique<dae::core::GameObject>( 400.f, 350.f ) };
			player->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 10.0f, true } }, SDL_Color{ 255, 204, 0, 255 } );
			player->AddComponent<dae::core::ColliderComponent>( 5.f, 5.f, 1 );
			player->AddComponent<dae::core::MoveComponent>( 250.f, 2.f );
			player->AddComponent<ScreenWrapComponent>( 1024.f, 576.f );
			player->AddComponent<PlayerComponent>();

			scene.AddGameObject( std::move( player ) );
		}

		static void BuildSpawner( dae::scenes::Scene& scene )
		{
			auto spawner{ std::make_unique<dae::core::GameObject>() };
			spawner->AddComponent<SpawnerPortalComponent>( UnitData{} );
			spawner->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::RectShape{ SDL_FRect{ -10.f, -10.f, 20.f, 20.f }, false } }, SDL_Color{ 255, 120, 0, 255 }, 3 );

			scene.AddGameObject( std::move( spawner ) );
		}

		static void EnableCollisions( dae::scenes::Scene& scene )
		{
			scene.GetCollisionSystem().RegisterCallback(
		[] ( dae::core::GameObject* actorA, dae::core::GameObject* actorB )
		{
			actorA->MarkForDeletion();
			actorB->MarkForDeletion();
		} );
		}
	};
}
#endif
