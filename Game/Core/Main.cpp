#include <filesystem>
#include <memory>
#include <string>
#include <utility>

#include <glm/fwd.hpp>

#include <SDL3/SDL_main.h> // Required

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h> // Required
#endif

#include "Game/Commands/DieCommand.h"
#include "Game/Commands/MoveCommand.h"
#include "Game/Commands/ScoreCommand.h"

#include "Game/Components/FPSComponent.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/RotatorComponent.h"
#include "Game/Components/ScoreComponent.h"
#include "Game/Components/UIValueObserver.h"

#include "Minigin/Core/ColliderComponent.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Minigin.h"
#include "Minigin/Core/SDBMHash.h"

#include "Minigin/Events/GameEvent.h"

#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/TextureComponent.h"

#include "Minigin/Input/Gamepad.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/Keyboard.h"

#include "Minigin/Resources/ResourceManager.h"

#include "Minigin/Scene/Scene.h"
#include "Minigin/Scene/SceneManager.h"

// TODO L: Split engine and game into dll & exe.

static void LoadMainMenu()
{
	using namespace dae;
	using namespace bvi;

	auto& scene{ scene::SceneManager::GetInstance().CreateScene() };

	auto background{ std::make_unique<core::GameObject>() };
	background->AddComponent<graphics::TextureComponent>()->SetTexture( "background.png" );
	scene.AddGameObject( std::move( background ) );

	auto logo{ std::make_unique<core::GameObject>( 358.f, 180.f ) };
	logo->AddComponent<graphics::TextureComponent>()->SetTexture( "logo.png" );
	scene.AddGameObject( std::move( logo ) );

	auto font{ resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 ) };
	auto title{ std::make_unique<core::GameObject>( 292.f, 20.f ) };
	title->AddComponent<graphics::TextComponent>( "Programming 4 Assignment", font )->SetColor( { 255, 255, 0, 255 } );
	scene.AddGameObject( std::move( title ) );

	font = resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 );
	auto fps{ std::make_unique<core::GameObject>( 20.f, 20.f ) };
	fps->AddComponent<graphics::TextComponent>( "FPS", font );
	fps->AddComponent<FPSComponent>();
	scene.AddGameObject( std::move( fps ) );


	font = resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 20 );
	auto tutorialWhite{ std::make_unique<core::GameObject>( 20.f, 75.f ) };
	tutorialWhite->AddComponent<graphics::TextComponent>( "Use the WASD to move the white starfighter, spacebar to inflict damage and Q & E to score points.", font );
	scene.AddGameObject( std::move( tutorialWhite ) );

	auto tutorialRed{ std::make_unique<core::GameObject>( 20.f, 100.f ) };
	tutorialRed->AddComponent<graphics::TextComponent>( "Use the D-Pad to move the red starfighter, left shoulder to inflict damage and X & Y to score points.", font );
	scene.AddGameObject( std::move( tutorialRed ) );


	auto player1{ std::make_unique<core::GameObject>( 400.f, 350.f ) };
	player1->AddComponent<core::ColliderComponent>( 32.f, 32.f, 0 );
	player1->AddComponent<graphics::TextureComponent>()->SetTexture( "starfighter.png" );
	auto player1Health = player1->AddComponent<HealthComponent>( 3 );
	auto player1Score = player1->AddComponent<ScoreComponent>();
	auto* player1Ptr = player1.get();
	scene.AddGameObject( std::move( player1 ) );

	auto player2{ std::make_unique<core::GameObject>( 600.f, 350.f ) };
	player2->AddComponent<core::ColliderComponent>( 32.f, 32.f, 0 );
	player2->AddComponent<graphics::TextureComponent>()->SetTexture( "starfighter_captured.png" );
	auto player2Health = player2->AddComponent<HealthComponent>( 3 );
	auto player2Score = player2->AddComponent<ScoreComponent>();
	auto* player2Ptr = player2.get();
	scene.AddGameObject( std::move( player2 ) );

	scene.GetCollisionSystem().RegisterCallback( [] ( dae::core::GameObject* actorA, dae::core::GameObject* actorB ) { actorA->MarkForDeletion(); actorB->MarkForDeletion(); } );

	font = resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 16 );
	auto player1LivesUI{ std::make_unique<core::GameObject>( 20.f, 125.f ) };
	player1LivesUI->AddComponent<graphics::TextComponent>( "Lives:", font );
	auto playerObserver = player1LivesUI->AddComponent<UIValueObserver>( events::GameEvent{ core::make_sdbm_hash( "PlayerDied" ) }, [] ( int v ) {
		return "White Lives:  " + std::to_string( v ) + " (spacebar)";
 } );
	player1Health->AttachObserver( playerObserver );
	scene.AddGameObject( std::move( player1LivesUI ) );

	auto player2LivesUI{ std::make_unique<core::GameObject>( 20.f, 175.f ) };
	player2LivesUI->AddComponent<graphics::TextComponent>( "Lives:", font );
	playerObserver = player2LivesUI->AddComponent<UIValueObserver>( events::GameEvent{ core::make_sdbm_hash( "PlayerDied" ) }, [] ( int v ) {
		return "Red Lives:     " + std::to_string( v ) + " (left shoulder)";
 } );
	player2Health->AttachObserver( playerObserver );
	scene.AddGameObject( std::move( player2LivesUI ) );

	auto player1ScoreUI{ std::make_unique<core::GameObject>( 20.f, 150.f ) };
	player1ScoreUI->AddComponent<graphics::TextComponent>( "Lives:", font );
	playerObserver = player1ScoreUI->AddComponent<UIValueObserver>( events::GameEvent{ core::make_sdbm_hash( "ScoreChanged" ) }, [] ( int v ) { return "White Score: " + std::to_string( v ) + " (Q & E)"; } );
	player1Score->AttachObserver( playerObserver );
	scene.AddGameObject( std::move( player1ScoreUI ) );

	auto player2ScoreUI{ std::make_unique<core::GameObject>( 20.f, 200.f ) };
	player2ScoreUI->AddComponent<graphics::TextComponent>( "Lives:", font );
	playerObserver = player2ScoreUI->AddComponent<UIValueObserver>( events::GameEvent{ core::make_sdbm_hash( "ScoreChanged" ) }, [] ( int v ) {
		return "Red Score:    " + std::to_string( v ) + " (X & Y)";
} );
	player2Score->AttachObserver( playerObserver );
	scene.AddGameObject( std::move( player2ScoreUI ) );

	auto rotator{ std::make_unique<core::GameObject>() };
	rotator->AddComponent<graphics::TextureComponent>()->SetTexture( "starfighter.png" );
	rotator->AddComponent<RotatorComponent>( 40.0f, 5.f );
	rotator->SetParent( player1Ptr, true );
	scene.AddGameObject( std::move( rotator ) );


	auto& input = input::InputManager::GetInstance();
	const float keyboardSpeed = 100.0f;
	const float gamepadSpeed = 200.0f;

	input.BindCommand( input::Keyboard::Key::W, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player1Ptr, glm::vec2{ 0, -1 }, keyboardSpeed ) );

	input.BindCommand( input::Keyboard::Key::S, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player1Ptr, glm::vec2{ 0, 1 }, keyboardSpeed ) );

	input.BindCommand( input::Keyboard::Key::A, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player1Ptr, glm::vec2{ -1, 0 }, keyboardSpeed ) );

	input.BindCommand( input::Keyboard::Key::D, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player1Ptr, glm::vec2{ 1, 0 }, keyboardSpeed ) );

	input.BindCommand( input::Keyboard::Key::Space, input::InputManager::KeyState::Up, std::make_unique<DieCommand>( player1Health ) );

	input.BindCommand( input::Keyboard::Key::Q, input::InputManager::KeyState::Up, std::make_unique<ScoreCommand>( player1Score, 10 ) );

	input.BindCommand( input::Keyboard::Key::E, input::InputManager::KeyState::Up, std::make_unique<ScoreCommand>( player1Score, 100 ) );

	input.BindCommand( input::Gamepad::Button::DPadUp, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player2Ptr, glm::vec2{ 0, -1 }, gamepadSpeed ), 0 );

	input.BindCommand( input::Gamepad::Button::DPadDown, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player2Ptr, glm::vec2{ 0, 1 }, gamepadSpeed ), 0 );

	input.BindCommand( input::Gamepad::Button::DPadLeft, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player2Ptr, glm::vec2{ -1, 0 }, gamepadSpeed ), 0 );

	input.BindCommand( input::Gamepad::Button::DPadRight, input::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>( player2Ptr, glm::vec2{ 1, 0 }, gamepadSpeed ), 0 );

	input.BindCommand( input::Gamepad::Button::LeftShoulder, input::InputManager::KeyState::Up, std::make_unique<DieCommand>( player2Health ) );

	input.BindCommand( input::Gamepad::Button::X, input::InputManager::KeyState::Up, std::make_unique<ScoreCommand>( player2Score, 10 ) );

	input.BindCommand( input::Gamepad::Button::Y, input::InputManager::KeyState::Up, std::make_unique<ScoreCommand>( player2Score, 100 ) );

	scene.Initialize();
}

int main( int, char* [] )
{
#if __EMSCRIPTEN__
	std::filesystem::path dataLocation = "";
#else
	std::filesystem::path dataLocation = "./Data/";
	if ( !std::filesystem::exists( dataLocation ) )
		dataLocation = "../Data/";
#endif

	dae::core::Minigin engine(
		{
			.dataPath = dataLocation,
			.windowTitle = "Programming 4 assignment",
			.windowWidth = 1024,
			.windowHeight = 576
		} );
	engine.Run( LoadMainMenu );

	return 0;
}
