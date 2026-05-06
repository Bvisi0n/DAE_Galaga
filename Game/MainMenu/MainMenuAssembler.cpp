#include <memory>
#include <utility>

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

#include <Game/Common/FPSComponent.h>
#include <Game/Common/PushStateCommand.h>
#include <Game/Galaga/GalagaMenuState.h>
#include <Game/GravityBender/GravityBenderState.h>
#include <Game/MainMenu/MainMenuAssembler.h>

namespace bvi::main_menu
{
	void MainMenuAssembler::Assemble( dae::core::IAppState* stateMachine )
	{
		auto& scene = dae::scenes::SceneManager::GetInstance().CreateScene();

		AssembleBackground( scene );
		AssembleLogo( scene );
		AssembleFPSCounter( scene );

		AssembleGalagaUI( scene, stateMachine );

	#ifdef ENABLE_GRAVITY_BENDER
		AssembleGravityBenderUI( scene, stateMachine );
	#endif

		scene.Initialize();
	}

	void MainMenuAssembler::AssembleBackground( dae::scenes::Scene& scene )
	{
		auto background{ std::make_unique<dae::core::GameObject>() };
		background->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "background.png" );
		scene.AddGameObject( std::move( background ) );
	}

	void MainMenuAssembler::AssembleFPSCounter( dae::scenes::Scene& scene )
	{
		auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };

		using Object = dae::core::GameObject;
		using Descriptor = dae::core::TransformDescriptor;
		auto fpsCounter
		{
			std::make_unique<Object>( Descriptor{.localPosition = { 20.F, 20.F, 0.F } } )
		};

		fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font );
		fpsCounter->AddComponent<common::FPSComponent>();
		scene.AddGameObject( std::move( fpsCounter ) );
	}

	void MainMenuAssembler::AssembleGalagaUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine )
	{
		auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };

		using Object = dae::core::GameObject;
		using Descriptor = dae::core::TransformDescriptor;
		auto instructions
		{
			std::make_unique<Object>( Descriptor{.localPosition = { 400.F, 375.F, 0.F } } )
		};

		instructions->AddComponent<dae::graphics::TextComponent>( "Press G to start Galaga", font );

		auto* inputComp = instructions->AddComponent<dae::input::PlayerInputComponent>();

		auto pushStateCommand = std::make_unique<common::PushStateCommand<galaga::GalagaMenuState>>( stateMachine );

		inputComp->AddBinding( dae::input::ScopedInputBinding{ dae::input::Keyboard::Key::G, dae::input::InputManager::KeyState::Down, std::move( pushStateCommand ) } );

		scene.AddGameObject( std::move( instructions ) );
	}

	void MainMenuAssembler::AssembleGravityBenderUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine )
	{
		auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };

		using Object = dae::core::GameObject;
		using Descriptor = dae::core::TransformDescriptor;
		auto instructions
		{
			std::make_unique<Object>( Descriptor{.localPosition = { 400.F, 410.F, 0.F } } )
		};

		instructions->AddComponent<dae::graphics::TextComponent>( "Press F to start Gravity Bender", font );

		auto* inputComp = instructions->AddComponent<dae::input::PlayerInputComponent>();

		auto pushStateCommand = std::make_unique<common::PushStateCommand<gravity_bender::GravityBenderState>>( stateMachine );

		inputComp->AddBinding( dae::input::ScopedInputBinding{ dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down, std::move( pushStateCommand ) } );

		scene.AddGameObject( std::move( instructions ) );
	}

	void MainMenuAssembler::AssembleLogo( dae::scenes::Scene& scene )
	{
		using Object = dae::core::GameObject;
		using Descriptor = dae::core::TransformDescriptor;
		auto logo{ std::make_unique<Object>( Descriptor{.localPosition = { 358.F, 180.F, 0.F } } ) };

		logo->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "logo.png" );
		scene.AddGameObject( std::move( logo ) );
	}
}
