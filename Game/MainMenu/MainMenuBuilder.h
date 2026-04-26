#ifndef BVI_MENUBUILDER_H
#define BVI_MENUBUILDER_H

#include <memory>

#include "Game/Common/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Input/PlayerInputComponent.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"

// TODO bvi_main_menu - Review builder pattern. (Inspired by)
	// https://refactoring.guru/design-patterns/builder
	// https://www.geeksforgeeks.org/system-design/builder-pattern-c-design-patterns/
	// Both sources show a director and don't use static methods.
	// More research is needed. Feels a bit like it's director and builder mashed in a single class.

namespace bvi::main_menu
{
	class MainMenuBuilder final
	{
	public:
		MainMenuBuilder() = delete;

		static void Build( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine )
		{
			BuildBackground( scene );
			BuildLogo( scene );
			BuildFPSCounter( scene );

		#ifdef ENABLE_GRAVITY_BENDER
			BuildGravityBenderUI( scene, stateMachine );
		#endif

			scene.Initialize();
		}

	private:
		static void BuildBackground( dae::scenes::Scene& scene )
		{
			auto background{ std::make_unique<dae::core::GameObject>() };
			background->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "background.png" );
			scene.AddGameObject( std::move( background ) );
		}

		static void BuildLogo( dae::scenes::Scene& scene )
		{
			auto logo{ std::make_unique<dae::core::GameObject>( 358.f, 180.f ) };
			logo->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "logo.png" );
			scene.AddGameObject( std::move( logo ) );
		}

		static void BuildFPSCounter( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };
			auto fpsCounter{ std::make_unique<dae::core::GameObject>( 20.f, 20.f ) };
			fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font );
			fpsCounter->AddComponent<common::FPSComponent>();
			scene.AddGameObject( std::move( fpsCounter ) );
		}

		static void BuildGravityBenderUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };
			auto instructions{ std::make_unique<dae::core::GameObject>( 425.f, 375.f ) };
			instructions->AddComponent<dae::graphics::TextComponent>( "Press F to start", font );

			auto inputComp = instructions->AddComponent<dae::input::PlayerInputComponent>();

			auto pushStateCommand = std::make_unique<common::PushStateCommand<gravity_bender::GravityBenderState>>( stateMachine );

			inputComp->AddBinding( dae::input::ScopedInputBinding{ dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down, std::move( pushStateCommand ) } );

			scene.AddGameObject( std::move( instructions ) );
		}
	};
}
#endif
