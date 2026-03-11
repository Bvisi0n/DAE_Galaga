#include <filesystem>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Commands/MoveCommand.h"
#include "Components/FPSComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/TextureComponent.h"
#include "Components/TextComponent.h"
#include "Input/InputManager.h"
#include "Singletons/ResourceManager.h"
#include "Singletons/SceneManager.h"
#include "GameObject.h"
#include "Minigin.h"
#include "Scene.h"

// Priority:	0 Breaks compilation
//				1 Next Deadline
//				2 Based on feedback
//				3 Galaga related
//				4 Other improvements
//				5 Needs investigation

// TODO 2: Encapsulate all transform logic into the Transform class, transition it to use matrices.
// TODO 2: Add a component to a game object, should check for doubles of same type, Text and Texture cannot coexist.
// TODO 2: Remove a component should use a flag.
// TODO 2: Ensure GetComponent isn't called in the hotpath.
// TODO 2: Add Scale to transform & add support in rendering.

// TODO 2: When removing a GameObject from the scene, use a flag to mark it for deletion.
// TODO 2: GameObjects should not update when marked for deletion.
// TODO 2: GameObject::IsChild() should check the whole chain.
// TODO 2: In GameObject a parent should own all children.
// TODO 2: In GameObject if a parent gets deleted, then all kids should be deleted as well. (RAII if ownership is right)

// TODO 2: Review the entire update flow.
// TODO 2: Review game loop, what happens when frames start to take to long? Should we cap the max delta time?
// TODO 2: Encapsulate coldpath data in a struct and use a pointer to it in the GameObject.

// TODO 3: Add CollisionComponent and a way to check for collisions between GameObjects.

// TODO 4: Add concept to dae::GameObject::AddComponent, HasComponent, GetComponent, RemoveComponent.
// TODO 4: Split engine and game into dll & exe.
// TODO 4: Improve folder structure.
// TODO 4: Make resolution, name etc. more accessible and less hard coded.
// TODO 4: RotatorComponent should support ellipses.
// TODO 4: Diagonal movement is currently double speed.

// TODO 5: Should a ScoreComponent be part of the engine or game?
			// Part of game, logic is game specific, it should use be a GameObject with TextComponent and custom score logic defined in game code.
// TODO 5: In GameObject::SetParent, if keepWorldPosition is false, should the local position be reset to something specific?
			// Yes, depends on intention of the game, to the hand? In what rotation? Optional paramenter?

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

	auto starfighter{ std::make_unique<dae::GameObject>() };
	starfighter->AddComponent<dae::TextureComponent>()->SetTexture("starfighter.png");
	starfighter->SetLocalPosition(400, 350);
	auto* pStarfighter = starfighter.get();
	scene.Add(std::move(starfighter));

	auto starfighter_captured{ std::make_unique<dae::GameObject>() };
	starfighter_captured->AddComponent<dae::TextureComponent>()->SetTexture("starfighter_captured.png");
	starfighter_captured->SetLocalPosition(600, 350);
	auto* pStarfighter_captured = starfighter_captured.get();
	scene.Add(std::move(starfighter_captured));

	auto& input = dae::InputManager::GetInstance();
	const float keyboard_speed = 100.0f;
	const float gamepad_speed = 200.0f;

	input.BindCommand(dae::Keyboard::Key::W, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter, glm::vec2{ 0, -1 }, keyboard_speed));

	input.BindCommand(dae::Keyboard::Key::S, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter, glm::vec2{ 0, 1 }, keyboard_speed));

	input.BindCommand(dae::Keyboard::Key::A, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter, glm::vec2{ -1, 0 }, keyboard_speed));
	//input.UnbindCommand(dae::Keyboard::Key::A, dae::InputManager::KeyState::Pressed);
	// Unbind works

	input.BindCommand(dae::Keyboard::Key::D, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter, glm::vec2{ 1, 0 }, keyboard_speed));

	input.BindCommand(dae::Gamepad::Button::DPadUp, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter_captured, glm::vec2{ 0, -1 }, gamepad_speed), 0);

	input.BindCommand(dae::Gamepad::Button::DPadDown, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter_captured, glm::vec2{ 0, 1 }, gamepad_speed), 0);

	input.BindCommand(dae::Gamepad::Button::DPadLeft, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter_captured, glm::vec2{ -1, 0 }, gamepad_speed), 0);

	input.BindCommand(dae::Gamepad::Button::DPadRight, dae::InputManager::KeyState::Pressed, std::make_unique<MoveCommand>(pStarfighter_captured, glm::vec2{ 1, 0 }, gamepad_speed), 0);
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
