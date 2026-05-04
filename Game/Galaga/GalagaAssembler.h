#ifndef BVI_GALAGAASSEMBLER_H
#define BVI_GALAGAASSEMBLER_H

#include <memory>
#include <utility>

#include <Game/Common/FPSComponent.h>
#include <Game/Common/PushStateCommand.h>
#include <Game/MainMenu/MainMenuState.h>

#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/IAppState.h>
#include <Minigin/Core/Transform.h>
#include <Minigin/Graphics/TextComponent.h>
#include <Minigin/Graphics/TextureComponent.h>
#include <Minigin/Input/InputManager.h>
#include <Minigin/Input/Keyboard.h>
#include <Minigin/Input/PlayerInputComponent.h>
#include <Minigin/Input/ScopedInputBinding.h>
#include <Minigin/Resources/ResourceManager.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/SceneManager.h>

// TODO dae_input - Split into .cpp/.h

namespace bvi::galaga
{
	class GalagaAssembler final
	{
	public:
		GalagaAssembler() = delete;

		static void Assemble( dae::core::IAppState* stateMachine )
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().CreateScene();

			AssembleBackground( scene );
			AssembleFPSCounter( scene );

			AssembleBackToMainMenuUI( scene, stateMachine );

			scene.Initialize();
		}

	private:
		static void AssembleBackground( dae::scenes::Scene& scene )
		{
			auto background{ std::make_unique<dae::core::GameObject>() };
			background->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "background.png" );
			scene.AddGameObject( std::move( background ) );
		}

		static void AssembleFPSCounter( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 12 ) };

			using Object = dae::core::GameObject;
			using Descriptor = dae::core::TransformDescriptor;
			auto fpsCounter
			{
				std::make_unique<Object>( Descriptor{.localPosition = { 5.F, 5.F, 0.F } } )
			};

			fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font );
			fpsCounter->AddComponent<common::FPSComponent>();
			scene.AddGameObject( std::move( fpsCounter ) );
		}

		static void AssembleBackToMainMenuUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 12 ) };

			using Object = dae::core::GameObject;
			using Descriptor = dae::core::TransformDescriptor;
			auto instructions
			{
				std::make_unique<Object>( Descriptor{.localPosition = { 25.F, 50.F, 0.F } } )
			};

			instructions->AddComponent<dae::graphics::TextComponent>( "Press F to go back", font );

			auto* inputComp = instructions->AddComponent<dae::input::PlayerInputComponent>();

			auto pushStateCommand = std::make_unique<common::PushStateCommand<main_menu::MainMenuState>>( stateMachine );

			inputComp->AddBinding( dae::input::ScopedInputBinding{ dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down, std::move( pushStateCommand ) } );

			scene.AddGameObject( std::move( instructions ) );
		}
	};
}
#endif
