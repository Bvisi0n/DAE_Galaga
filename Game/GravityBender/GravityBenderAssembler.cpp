#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <Minigin/Core/ColliderComponent.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/IAppState.h>
#include <Minigin/Core/MoveComponent.h>
#include <Minigin/Core/Transform.h>
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
	void GravityBenderAssembler::Assemble( dae::core::IAppState* /*stateMachine*/ )
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
		constexpr const auto& viewportConfig = bvi::gravity_bender::config::c_GlobalConfig.viewport;

		constexpr SDL_FRect screenBounds
		{
			.x = 0.F,
			.y = 0.F,
			.w = viewportConfig.width,
			.h = viewportConfig.height
		};

		constexpr dae::graphics::RectShape rectConfig
		{
			.bounds = screenBounds,
			.isFilled = true
		};

		constexpr SDL_FColor bgColor
		{
			.r = viewportConfig.backgroundColor.r,
			.g = viewportConfig.backgroundColor.g,
			.b = viewportConfig.backgroundColor.b,
			.a = viewportConfig.backgroundColor.a
		};

		auto backgroundObject{ std::make_unique<dae::core::GameObject>() };
		backgroundObject->AddComponent<dae::graphics::PrimitiveRenderComponent>
			( dae::graphics::PrimitiveShape{ rectConfig }, bgColor );

		scene.AddGameObject( std::move( backgroundObject ) );
	}

	void GravityBenderAssembler::AssembleFPSCounter( dae::scenes::Scene& scene )
	{
		constexpr const auto& uiConfig = bvi::gravity_bender::config::c_GlobalConfig.ui;

		constexpr SDL_FColor fpsColor
		{
			.r = uiConfig.fpsColor.r,
			.g = uiConfig.fpsColor.g,
			.b = uiConfig.fpsColor.b,
			.a = uiConfig.fpsColor.a
		};

		using ResourceManager = dae::resources::ResourceManager;
		auto font{ ResourceManager::GetInstance().LoadFont( "Lingua.otf", uiConfig.fpsFontSize ) };

		using Object = dae::core::GameObject;
		using Descriptor = dae::core::TransformDescriptor;
		auto fpsCounter
		{
			std::make_unique<Object>( Descriptor{.localPosition = { uiConfig.fpsStartX, uiConfig.fpsStartY, 0.F } } )
		};

		fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font )->SetColor( fpsColor );
		fpsCounter->AddComponent<common::FPSComponent>();

		scene.AddGameObject( std::move( fpsCounter ) );
	}

	void GravityBenderAssembler::AssembleInstructions( dae::scenes::Scene& scene )
	{
		constexpr const auto& uiConfig = bvi::gravity_bender::config::c_GlobalConfig.ui;

		constexpr SDL_FColor textColor
		{
			.r = uiConfig.instructionsColor.r,
			.g = uiConfig.instructionsColor.g,
			.b = uiConfig.instructionsColor.b,
			.a = uiConfig.instructionsColor.a
		};

		using ResourceManager = dae::resources::ResourceManager;
		auto font{ ResourceManager::GetInstance().LoadFont( "Lingua.otf", uiConfig.instructionsFontSize ) };

		struct InstructionLine
		{
			float xOffset;
			std::string_view text;
		};

		constexpr std::array<InstructionLine, 4> lines
		{
			InstructionLine
			{
				.xOffset = 90.F,
				.text = "Welcome Commander, you're just in time!"
			},
			InstructionLine
			{
				.xOffset = 10.F,
				.text = "Make the red rectangles crash into each other before they overwhelm the system!"
			},
			InstructionLine
			{
				.xOffset = 10.F,
				.text = "Use WASD to move"
			},
			InstructionLine
			{
				.xOffset = 10.F,
				.text = "Use SPACE to place a temporary gravity well at your location"
			}
		};

		float currentY = uiConfig.instructionsStartY;

		for ( const auto& line : lines )
		{
			using Object = dae::core::GameObject;
			using Descriptor = dae::core::TransformDescriptor;
			auto textObject
			{
				std::make_unique<Object>( Descriptor{.localPosition = { line.xOffset, currentY, 0.F } } )
			};

			textObject->AddComponent<dae::graphics::TextComponent>( std::string{ line.text }, font )
				->SetColor( textColor );

			scene.AddGameObject( std::move( textObject ) );

			currentY += uiConfig.instructionsLineSpacing;
		}
	}

	void GravityBenderAssembler::AssemblePlayer( dae::scenes::Scene& scene )
	{
		namespace config = bvi::gravity_bender::config;
		constexpr const auto& playerConfig = config::c_GlobalConfig.player;

		using Object = dae::core::GameObject;
		using Descriptor = dae::core::TransformDescriptor;
		auto player
		{
			std::make_unique<Object>( Descriptor{.localPosition = { playerConfig.startX, playerConfig.startY, 0.F } } )
		};

		constexpr SDL_FColor playerColor
		{
			.r = playerConfig.color.r,
			.g = playerConfig.color.g,
			.b = playerConfig.color.b,
			.a = playerConfig.color.a
		};

		constexpr dae::graphics::CircleShape shapeConfig
		{
			.radius = playerConfig.radius,
			.isFilled = true
		};

		using PrimitiveRenderComponent = dae::graphics::PrimitiveRenderComponent;
		using PrimitiveShape = dae::graphics::PrimitiveShape;
		player->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ shapeConfig }, playerColor );

		constexpr const auto& viewportConfig = config::c_GlobalConfig.viewport;
		const float width = viewportConfig.width;
		const float height = viewportConfig.height;
		const std::array<glm::vec3, 8> offsets =
		{ {
			{0.F, -height, 0.F},
			{width, 0.F, 0.F},
			{0.F, height, 0.F},
			{-width, 0.F, 0.F},
			{width, -height, 0.F},
			{width, height, 0.F},
			{-width, height, 0.F},
			{-width, -height, 0.F}
		} };

		for ( const auto& offset : offsets )
		{
			auto cloneRoot = std::make_unique<dae::core::GameObject>();
			cloneRoot->GetTransform().SetLocalPosition( offset );
			cloneRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ shapeConfig }, playerColor );

			cloneRoot->SetParent( player.get(), false );
			scene.AddGameObject( std::move( cloneRoot ) );
		}

		using Collider = dae::core::ColliderComponent;
		using Size2D = dae::core::Size2D;
		player->AddComponent<Collider>
			(
				Size2D
				{
					.width = playerConfig.colliderSize,
					.height = playerConfig.colliderSize
				},
				1
			);

		player->AddComponent<ScreenWrapComponent>( playerConfig.radius );

		player->AddComponent<PlayerComponent>();

		using MoveComponent = dae::core::MoveComponent;
		using MoveDescriptor = dae::core::MoveDescriptor;
		auto* moveComp
		{
			player->AddComponent<MoveComponent>
			(
				MoveDescriptor
				{
					.maxSpeed = playerConfig.baseSpeed,
					.drag = playerConfig.speedMultiplier
				}
			)
		};

		auto* inputComp{ player->AddComponent<dae::input::PlayerInputComponent>() };

		using Key = dae::input::Keyboard::Key;
		using KeyState = dae::input::InputManager::KeyState;
		using MoveCommand = common::MoveCommand;
		using ScopedInputBinding = dae::input::ScopedInputBinding;
		using vec3 = glm::vec3;

		inputComp->AddBinding( ScopedInputBinding{ Key::W, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 0.F, -playerConfig.inputForce, 0.F } ) } );
		inputComp->AddBinding( ScopedInputBinding{ Key::S, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 0.F,  playerConfig.inputForce, 0.F } ) } );
		inputComp->AddBinding( ScopedInputBinding{ Key::A, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ -playerConfig.inputForce, 0.F, 0.F } ) } );
		inputComp->AddBinding( ScopedInputBinding{ Key::D, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{  playerConfig.inputForce, 0.F, 0.F } ) } );

		inputComp->AddBinding( ScopedInputBinding{ Key::Space, KeyState::Pressed, std::make_unique<SpawnGravityFieldCommand>( player.get() ) } );

		scene.AddGameObject( std::move( player ) );
	}

	void GravityBenderAssembler::AssembleSpawner( dae::scenes::Scene& scene )
	{
		constexpr const auto& portalConfig = bvi::gravity_bender::config::c_GlobalConfig.portal;
		constexpr float halfSize{ portalConfig.portalSize / 2.F };

		constexpr SDL_FRect localBounds
		{
			.x = -halfSize,
			.y = -halfSize,
			.w = portalConfig.portalSize,
			.h = portalConfig.portalSize
		};

		constexpr dae::graphics::RectShape rectConfig
		{
			.bounds = localBounds,
			.isFilled = false
		};

		constexpr SDL_FColor portalColor
		{
			.r = portalConfig.portalColor.r,
			.g = portalConfig.portalColor.g,
			.b = portalConfig.portalColor.b,
			.a = portalConfig.portalColor.a
		};

		auto spawner{ std::make_unique<dae::core::GameObject>() };

		spawner->AddComponent<SpawnerPortalComponent>( portalConfig );

		spawner->AddComponent<dae::graphics::PrimitiveRenderComponent>
			(
				dae::graphics::PrimitiveShape{ rectConfig },
				portalColor,
				portalConfig.portalLineThickness
			);

		scene.AddGameObject( std::move( spawner ) );
	}

	void GravityBenderAssembler::AssembleViewportBorder( dae::scenes::Scene& scene )
	{
		constexpr const auto& viewportConfig = bvi::gravity_bender::config::c_GlobalConfig.viewport;

		constexpr SDL_FRect screenBounds
		{
			.x = 0.F,
			.y = 0.F,
			.w = viewportConfig.width,
			.h = viewportConfig.height
		};

		constexpr dae::graphics::RectShape rectConfig
		{
			.bounds = screenBounds,
			.isFilled = false
		};

		constexpr SDL_FColor borderColor
		{
			.r = viewportConfig.borderColor.r,
			.g = viewportConfig.borderColor.g,
			.b = viewportConfig.borderColor.b,
			.a = viewportConfig.borderColor.a
		};

		auto borderObject{ std::make_unique<dae::core::GameObject>() };

		borderObject->AddComponent<dae::graphics::PrimitiveRenderComponent>
			(
				dae::graphics::PrimitiveShape{ rectConfig },
				borderColor,
				viewportConfig.borderThickness
			);

		scene.AddGameObject( std::move( borderObject ) );
	}

	void GravityBenderAssembler::EnableCollisions( dae::scenes::Scene& scene )
	{
		scene.GetCollisionSystem().RegisterCallback
		(
			[] ( dae::core::GameObject* actorA, dae::core::GameObject* actorB )
			{
				if ( actorA != nullptr )
				{
					actorA->MarkForDeletion();
				}

				if ( actorB != nullptr )
				{
					actorB->MarkForDeletion();
				}
			}
		);
	}
}
