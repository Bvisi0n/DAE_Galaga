#include <filesystem>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
	#include <vld.h>
#endif

#include "Game/Commands/DieCommand.h"
#include "Game/Commands/MoveCommand.h"
#include "Game/Commands/ScoreCommand.h"

#include "Game/Components/FPSComponent.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/RotatorComponent.h"
#include "Game/Components/ScoreComponent.h"
#include "Game/Components/UIValueObserver.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Minigin.h"
#include "Minigin/Core/SDBMHash.h"
#include "Minigin/Core/Transform.h"

#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Graphics/TextComponent.h"

#include "Minigin/Input/InputManager.h"

#include "Minigin/Resources/ResourceManager.h"

#include "Minigin/Scene/SceneManager.h"
#include "Minigin/Scene/Scene.h"

// TODO L: Split engine and game into dll & exe.

static void loadMainMenu()
{
	using namespace dae;

	auto& scene{ scene::SceneManager::GetInstance().CreateScene() };

	auto background{ std::make_unique<core::GameObject>() };
	background->AddComponent<TextureComponent>()->SetTexture("background.png");
	scene.Add(std::move(background));

	auto logo{ std::make_unique<core::GameObject>(358.f, 180.f) };
	logo->AddComponent<TextureComponent>()->SetTexture("logo.png");
	scene.Add(std::move(logo));

	auto font{ resources::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	auto title{ std::make_unique<core::GameObject>(292.f, 20.f) };
	title->AddComponent<TextComponent>("Programming 4 Assignment", font)->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(title));
	
	font = resources::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto fps{ std::make_unique<core::GameObject>(20.f, 20.f) };
	auto* pText = fps->AddComponent<TextComponent>("FPS", font);
	fps->AddComponent<FPSComponent>(pText);
	scene.Add(std::move(fps));


	font = resources::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto tutorial_white{ std::make_unique<core::GameObject>(20.f, 75.f) };
	tutorial_white->AddComponent<TextComponent>("Use the WASD to move the white starfighter, spacebar to inflict damage and Q & E to score points.", font);
	scene.Add(std::move(tutorial_white));

	auto tutorial_red{ std::make_unique<core::GameObject>(20.f, 100.f) };
	tutorial_red->AddComponent<TextComponent>("Use the D-Pad to move the red starfighter, left shoulder to inflict damage and X & Y to score points.", font);
	scene.Add(std::move(tutorial_red));


	auto player_1{ std::make_unique<core::GameObject>(400.f, 350.f) };
	player_1->AddComponent<TextureComponent>()->SetTexture("starfighter.png");
	auto player_1_health = player_1->AddComponent<HealthComponent>(3);
	auto player_1_score = player_1->AddComponent<ScoreComponent>();
	auto* pPlayer_1 = player_1.get();
	scene.Add(std::move(player_1));

	auto player_2{ std::make_unique<core::GameObject>(600.f, 350.f) };
	player_2->AddComponent<TextureComponent>()->SetTexture("starfighter_captured.png");
	auto player_2_health = player_2->AddComponent<HealthComponent>(3);
	auto player_2_score = player_2->AddComponent<ScoreComponent>();
	auto* pPlayer_2 = player_2.get();
	scene.Add(std::move(player_2));


	font = resources::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto player_1_lives_UI{ std::make_unique<core::GameObject>(20.f, 125.f) };
	pText = player_1_lives_UI->AddComponent<TextComponent>("Lives:", font);
	auto pPlayer_observer = player_1_lives_UI->AddComponent<UIValueObserver>(pText, events::GameEvent{ core::make_sdbm_hash("PlayerDied") }, [](int v) { return "White Lives:  " + std::to_string(v) + " (spacebar)"; });
	player_1_health->AttachObserver(pPlayer_observer);
	// TODO N: Initialize should be called by the engine, not manually like this.
	player_1_health->Initialize();
	scene.Add(std::move(player_1_lives_UI));

	auto player_2_lives_UI{ std::make_unique<core::GameObject>(20.f, 175.f) };
	pText = player_2_lives_UI->AddComponent<TextComponent>("Lives:", font);
	pPlayer_observer = player_2_lives_UI->AddComponent<UIValueObserver>(pText, events::GameEvent{ core::make_sdbm_hash("PlayerDied") }, [](int v) { return "Red Lives:     " + std::to_string(v) + " (left shoulder)"; });
	player_2_health->AttachObserver(pPlayer_observer);
	player_2_health->Initialize();
	scene.Add(std::move(player_2_lives_UI));

	auto player_1_score_UI{ std::make_unique<core::GameObject>(20.f, 150.f) };
	pText = player_1_score_UI->AddComponent<TextComponent>("Lives:", font);
	pPlayer_observer = player_1_score_UI->AddComponent<UIValueObserver>(pText, events::GameEvent{ core::make_sdbm_hash("ScoreChanged") }, [](int v) { return "White Score: " + std::to_string(v) + " (Q & E)"; });
	player_1_score->AttachObserver(pPlayer_observer);
	player_1_score->Initialize();
	scene.Add(std::move(player_1_score_UI));

	auto player_2_score_UI{ std::make_unique<core::GameObject>(20.f, 200.f) };
	pText = player_2_score_UI->AddComponent<TextComponent>("Lives:", font);
	pPlayer_observer = player_2_score_UI->AddComponent<UIValueObserver>(pText, events::GameEvent{ core::make_sdbm_hash("ScoreChanged") }, [](int v){ return "Red Score:    " + std::to_string(v) + " (X & Y)"; });
	player_2_score->AttachObserver(pPlayer_observer);
	player_2_score->Initialize();
	scene.Add(std::move(player_2_score_UI));

	auto rotator{ std::make_unique<core::GameObject>() };
	rotator->AddComponent<TextureComponent>()->SetTexture("starfighter.png");
	rotator->AddComponent<RotatorComponent>(20.0f, 2.5f);
	rotator->SetParent(pPlayer_1, true);
	scene.Add(std::move(rotator));


	auto& input = InputManager::GetInstance();
	const float keyboard_speed = 100.0f;
	const float gamepad_speed = 200.0f;

	input.BindCommand(Keyboard::Key::W, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_1, glm::vec2{ 0, -1 }, keyboard_speed));

	input.BindCommand(Keyboard::Key::S, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_1, glm::vec2{ 0, 1 }, keyboard_speed));

	input.BindCommand(Keyboard::Key::A, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_1, glm::vec2{ -1, 0 }, keyboard_speed));

	input.BindCommand(Keyboard::Key::D, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_1, glm::vec2{ 1, 0 }, keyboard_speed));

	input.BindCommand(Keyboard::Key::Space, InputManager::KeyState::Up, std::make_unique<DieCommand>(player_1_health));

	input.BindCommand(Keyboard::Key::Q, InputManager::KeyState::Up, std::make_unique<ScoreCommand>(player_1_score, 10));

	input.BindCommand(Keyboard::Key::E, InputManager::KeyState::Up, std::make_unique<ScoreCommand>(player_1_score, 100));

	input.BindCommand(Gamepad::Button::DPadUp, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_2, glm::vec2{ 0, -1 }, gamepad_speed), 0);

	input.BindCommand(Gamepad::Button::DPadDown, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_2, glm::vec2{ 0, 1 }, gamepad_speed), 0);

	input.BindCommand(Gamepad::Button::DPadLeft, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_2, glm::vec2{ -1, 0 }, gamepad_speed), 0);

	input.BindCommand(Gamepad::Button::DPadRight, InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pPlayer_2, glm::vec2{ 1, 0 }, gamepad_speed), 0);

	input.BindCommand(Gamepad::Button::LeftShoulder, InputManager::KeyState::Up, std::make_unique<DieCommand>(player_2_health));

	input.BindCommand(Gamepad::Button::X, InputManager::KeyState::Up, std::make_unique<ScoreCommand>(player_2_score, 10));

	input.BindCommand(Gamepad::Button::Y, InputManager::KeyState::Up, std::make_unique<ScoreCommand>(player_2_score, 100));
}

int main(int, char*[])
{
	#if __EMSCRIPTEN__
		std::filesystem::path data_location = "";
	#else
		std::filesystem::path data_location = "./Data/";
		if(!std::filesystem::exists(data_location))
			data_location = "../Data/";
	#endif

	dae::core::Minigin engine(data_location, "Programming 4 assignment", 1024, 576);
	engine.Run(loadMainMenu);
	
	return 0;
}
