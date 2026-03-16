#include <filesystem>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Commands/DieCommand.h"
#include "Commands/MoveCommand.h"
#include "Components/FPSComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TextureComponent.h"
#include "Components/TextComponent.h"
#include "Components/UIValueObserver.h"
#include "Input/InputManager.h"
#include "Singletons/ResourceManager.h"
#include "Singletons/SceneManager.h"
#include "GameObject.h"
#include "Minigin.h"
#include "Scene.h"

// DAEN: Split engine and game into dll & exe.

static void loadMainMenu()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene() };

	auto background{ std::make_unique<dae::GameObject>() };
	background->AddComponent<dae::TextureComponent>()->SetTexture("background.png");
	scene.Add(std::move(background));

	auto logo{ std::make_unique<dae::GameObject>() };
	logo->SetLocalPosition(358, 180);
	logo->AddComponent<dae::TextureComponent>()->SetTexture("logo.png");
	scene.Add(std::move(logo));

	auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	auto title{ std::make_unique<dae::GameObject>() };
	title->SetLocalPosition(292, 20);
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", font)->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(title));
	
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto fps{ std::make_unique<dae::GameObject>() };
	fps->SetLocalPosition(20, 20);
	auto* pText = fps->AddComponent<dae::TextComponent>("FPS", font);
	fps->AddComponent<dae::FPSComponent>(pText);
	scene.Add(std::move(fps));


	auto player_1{ std::make_unique<dae::GameObject>() };
	player_1->AddComponent<dae::TextureComponent>()->SetTexture("starfighter.png");
	auto player_1_subject = player_1->AddComponent<dae::HealthComponent>(3);
	player_1->SetLocalPosition(400, 350);
	auto* pPlayer_1 = player_1.get();
	scene.Add(std::move(player_1));

	auto player_2{ std::make_unique<dae::GameObject>() };
	player_2->AddComponent<dae::TextureComponent>()->SetTexture("starfighter_captured.png");
	auto player_2_subject = player_2->AddComponent<dae::HealthComponent>(3);
	player_2->SetLocalPosition(600, 350);
	auto* pPlayer_2 = player_2.get();
	scene.Add(std::move(player_2));


	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto player_1_lives{ std::make_unique<dae::GameObject>() };
	pText = player_1_lives->AddComponent<dae::TextComponent>("Lives:", font);
	auto pPlayer_1_observer = player_1_lives->AddComponent<dae::UIValueObserver>(pText, dae::GameEvent::PlayerDied, player_1_subject->GetLives());
    player_1_subject->AttachObserver(pPlayer_1_observer);
	player_1_lives->SetLocalPosition(20, 100);
	scene.Add(std::move(player_1_lives));

	auto player_2_lives{ std::make_unique<dae::GameObject>() };
	pText = player_2_lives->AddComponent<dae::TextComponent>("Lives:", font);
	auto pPlayer_2_observer = player_2_lives->AddComponent<dae::UIValueObserver>(pText, dae::GameEvent::PlayerDied, player_2_subject->GetLives());
    player_2_subject->AttachObserver(pPlayer_2_observer);
	player_2_lives->SetLocalPosition(20, 150);
	scene.Add(std::move(player_2_lives));


	auto& input = dae::InputManager::GetInstance();
	const float keyboard_speed = 100.0f;
	const float gamepad_speed = 200.0f;

	input.BindCommand(dae::Keyboard::Key::W, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_1, glm::vec2{ 0, -1 }, keyboard_speed));

	input.BindCommand(dae::Keyboard::Key::S, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_1, glm::vec2{ 0, 1 }, keyboard_speed));

	input.BindCommand(dae::Keyboard::Key::A, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_1, glm::vec2{ -1, 0 }, keyboard_speed));

	input.BindCommand(dae::Keyboard::Key::D, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_1, glm::vec2{ 1, 0 }, keyboard_speed));

	input.BindCommand(dae::Keyboard::Key::Space, dae::InputManager::KeyState::Up, std::make_unique<dae::DieCommand>(player_1_subject));

	input.BindCommand(dae::Gamepad::Button::DPadUp, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_2, glm::vec2{ 0, -1 }, gamepad_speed), 0);

	input.BindCommand(dae::Gamepad::Button::DPadDown, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_2, glm::vec2{ 0, 1 }, gamepad_speed), 0);

	input.BindCommand(dae::Gamepad::Button::DPadLeft, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_2, glm::vec2{ -1, 0 }, gamepad_speed), 0);

	input.BindCommand(dae::Gamepad::Button::DPadRight, dae::InputManager::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pPlayer_2, glm::vec2{ 1, 0 }, gamepad_speed), 0);

	input.BindCommand(dae::Gamepad::Button::LeftShoulder, dae::InputManager::KeyState::Up, std::make_unique<dae::DieCommand>(player_2_subject));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	std::filesystem::path data_location = "";
#else
	std::filesystem::path data_location = "./Data/";
	if(!std::filesystem::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(loadMainMenu);
    return 0;
}
