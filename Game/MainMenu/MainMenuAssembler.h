#ifndef BVI_MAINMENUASSEMBLER_H
#define BVI_MAINMENUASSEMBLER_H

#include <memory>

#include "Game/Common/FPSComponent.h"
#include "Game/Common/PushStateCommand.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/PlayerInputComponent.h"
#include "Minigin/Input/ScopedInputBinding.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"

namespace bvi::main_menu
{
	class MainMenuAssembler final
	{
	public:
		MainMenuAssembler() = delete;

		static void Assemble( dae::core::IAppState* stateMachine )
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().CreateScene();

			AssembleBackground( scene );
			AssembleLogo( scene );
			AssembleFPSCounter( scene );

		#ifdef ENABLE_GRAVITY_BENDER
			AssembleGravityBenderUI( scene, stateMachine );
		#endif

			scene.Initialize();
		}

	private:
		static void AssembleBackground( dae::scenes::Scene& scene )
		{
			auto background{ std::make_unique<dae::core::GameObject>() };
			background->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "background.png" );
			scene.AddGameObject( std::move( background ) );
		}

		static void AssembleLogo( dae::scenes::Scene& scene )
		{
			auto logo{ std::make_unique<dae::core::GameObject>( 358.f, 180.f ) };
			logo->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "logo.png" );
			scene.AddGameObject( std::move( logo ) );
		}

		static void AssembleFPSCounter( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };
			auto fpsCounter{ std::make_unique<dae::core::GameObject>( 20.f, 20.f ) };
			fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font );
			fpsCounter->AddComponent<common::FPSComponent>();
			scene.AddGameObject( std::move( fpsCounter ) );
		}

		static void AssembleGravityBenderUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };
			auto instructions{ std::make_unique<dae::core::GameObject>( 425.f, 375.f ) };
			instructions->AddComponent<dae::graphics::TextComponent>( "Press F to start", font );

			auto inputComp = instructions->AddComponent<dae::input::PlayerInputComponent>();

			// TODO bvi_main_menu - Once the Command pattern is reworked to be stateless, this should be simplified.
			auto pushStateCommand = std::make_unique<common::PushStateCommand<gravity_bender::GravityBenderState>>( stateMachine );

			inputComp->AddBinding( dae::input::ScopedInputBinding{ dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down, std::move( pushStateCommand ) } );

			scene.AddGameObject( std::move( instructions ) );
		}
	};
}
#endif
