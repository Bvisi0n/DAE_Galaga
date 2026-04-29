#include <memory>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <Minigin/Core/ColliderComponent.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/IAppState.h>
#include <Minigin/Core/MoveComponent.h>
#include <Minigin/Graphics/PrimitiveRenderComponent.h>
#include <Minigin/Graphics/TextComponent.h>
#include <Minigin/Input/InputManager.h>
#include <Minigin/Input/Keyboard.h>
#include <Minigin/Input/PlayerInputComponent.h>
#include <Minigin/Input/ScopedInputBinding.h>
#include <Minigin/Resources/ResourceManager.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/Common/FPSComponent.h>
#include <Game/Common/MoveCommand.h>
#include <Game/GravityBender/GravityBenderAssembler.h>
#include <Game/GravityBender/GravityBenderBlueprints.h>
#include <Game/GravityBender/PlayerComponent.h>
#include <Game/GravityBender/ScreenWrapComponent.h>
#include <Game/GravityBender/SpawnGravityFieldCommand.h>
#include <Game/GravityBender/SpawnerPortalComponent.h>

namespace bvi::gravity_bender
{
	void GravityBenderAssembler::Assemble( dae::core::IAppState* )
	{
		auto& scene = dae::scenes::SceneManager::GetInstance().CreateScene();

		AssembleBackground( scene );
		AssembleViewportBorder( scene );
		AssembleInstructions( scene );
		AssembleFPSCounter( scene );
		AssemblePlayer( scene );
		AssembleSpawner( scene );
		EnableCollisions( scene );

		scene.Initialize();
	}

	void GravityBenderAssembler::AssembleBackground( dae::scenes::Scene& scene )
	{
		// TODO bvi_gravity_bender - Fetch viewport dimensions and use them.
		constexpr SDL_FRect screenBounds{ 0.f, 0.f, 1024.f, 576.f };
		constexpr SDL_Color black{ 20, 10, 30, 255 };

		auto backgroundObject{ std::make_unique<dae::core::GameObject>() };
		backgroundObject->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::RectShape{ screenBounds, true } }, black );

		scene.AddGameObject( std::move( backgroundObject ) );
	}

	void GravityBenderAssembler::AssembleFPSCounter( dae::scenes::Scene& scene )
	{
		auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 18 ) };
		auto fpsCounter{ std::make_unique<dae::core::GameObject>( 10.f, 10.f ) };
		fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font )->SetColor( SDL_Color{ 150, 200, 150, 255 } );
		fpsCounter->AddComponent<common::FPSComponent>();
		scene.AddGameObject( std::move( fpsCounter ) );
	}

	void GravityBenderAssembler::AssembleInstructions( dae::scenes::Scene& scene )
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

	void GravityBenderAssembler::AssemblePlayer( dae::scenes::Scene& scene )
	{
		auto player{ std::make_unique<dae::core::GameObject>( 400.f, 350.f ) };
		player->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 10.0f, true } }, SDL_Color{ 255, 204, 0, 255 } );
		player->AddComponent<dae::core::ColliderComponent>( 5.f, 5.f, 1 );

		player->AddComponent<ScreenWrapComponent>( 1024.f, 576.f );
		player->AddComponent<PlayerComponent>();

		// TODO bvi_gravity_bender - Once the Command pattern is reworked to be stateless, this should be simplified.
		auto moveComp = player->AddComponent<dae::core::MoveComponent>( 250.f, 2.f );
		auto inputComp = player->AddComponent<dae::input::PlayerInputComponent>();

		using KeyState = dae::input::InputManager::KeyState;
		using Key = dae::input::Keyboard::Key;
		using MoveCommand = common::MoveCommand;
		using vec3 = glm::vec3;

		inputComp->AddBinding( dae::input::ScopedInputBinding{ Key::W, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 0.0f, -400.0f, 0.0f } ) } );
		inputComp->AddBinding( dae::input::ScopedInputBinding{ Key::S, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 0.0f, 400.0f, 0.0f } ) } );
		inputComp->AddBinding( dae::input::ScopedInputBinding{ Key::A, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ -400.0f, 0.0f, 0.0f } ) } );
		inputComp->AddBinding( dae::input::ScopedInputBinding{ Key::D, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 400.0f, 0.0f, 0.0f } ) } );
		inputComp->AddBinding( dae::input::ScopedInputBinding{ Key::Space, KeyState::Pressed, std::make_unique<SpawnGravityFieldCommand>( player.get() ) } );

		scene.AddGameObject( std::move( player ) );
	}

	void GravityBenderAssembler::AssembleSpawner( dae::scenes::Scene& scene )
	{
		auto spawner{ std::make_unique<dae::core::GameObject>() };
		spawner->AddComponent<SpawnerPortalComponent>( UnitData{} );
		spawner->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::RectShape{ SDL_FRect{ -10.f, -10.f, 20.f, 20.f }, false } }, SDL_Color{ 255, 120, 0, 255 }, 3 );

		scene.AddGameObject( std::move( spawner ) );
	}

	void GravityBenderAssembler::AssembleViewportBorder( dae::scenes::Scene& scene )
	{
		// TODO bvi_gravity_bender - Fetch viewport dimensions and use them.
		constexpr SDL_FRect screenBounds{ 0.f, 0.f, 1024.f, 576.f };
		constexpr SDL_Color neonPurple{ 138, 43, 226, 255 };

		auto borderObject{ std::make_unique<dae::core::GameObject>() };
		borderObject->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::RectShape{ screenBounds, false } }, neonPurple, 2 );

		scene.AddGameObject( std::move( borderObject ) );
	}

	void GravityBenderAssembler::EnableCollisions( dae::scenes::Scene& scene )
	{
		scene.GetCollisionSystem().RegisterCallback(
	[] ( dae::core::GameObject* actorA, dae::core::GameObject* actorB )
	{
		actorA->MarkForDeletion();
		actorB->MarkForDeletion();
	} );
	}
}
