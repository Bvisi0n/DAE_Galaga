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
#include "Singletons/InputManager.h"
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

// TODO 1: Add Input support for controllers, keyboard and mouse, use SDL, Xinput. Pimpl away the implementation and use Command Pattern.

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

	auto rotation_center{ std::make_unique<dae::GameObject>() };
	rotation_center->SetLocalPosition(512, 350);
	auto* pCenter = rotation_center.get();
	scene.Add(std::move(rotation_center));

	auto rotating_inner{ std::make_unique<dae::GameObject>() };
	rotating_inner->AddComponent<dae::TextureComponent>()->SetTexture("starfighter.png");
	rotating_inner->AddComponent<dae::RotatorComponent>(20.0f, 2.5f);
	rotating_inner->SetParent(pCenter, false);
	auto* pInner = rotating_inner.get();
	scene.Add(std::move(rotating_inner));

	auto rotating_outer{ std::make_unique<dae::GameObject>() };
	rotating_outer->AddComponent<dae::TextureComponent>()->SetTexture("starfighter_captured.png");
	rotating_outer->AddComponent<dae::RotatorComponent>(25.0f, -5.0f);
	rotating_outer->SetParent(pInner, false);
	scene.Add(std::move(rotating_outer));

	const float speed = 200.0f;
	auto& input = dae::InputManager::GetInstance();

	// Up
	input.BindCommand(0, dae::Gamepad::Button::DPadUp, dae::KeyState::Pressed,
		std::make_unique<MoveCommand>(pCenter, glm::vec2{ 0, -1 }, speed));

	// Down
	input.BindCommand(0, dae::Gamepad::Button::DPadDown, dae::KeyState::Pressed,
		std::make_unique<MoveCommand>(pCenter, glm::vec2{ 0, 1 }, speed));

	// Left
	input.BindCommand(0, dae::Gamepad::Button::DPadLeft, dae::KeyState::Pressed,
		std::make_unique<MoveCommand>(pCenter, glm::vec2{ -1, 0 }, speed));

	// Right
	input.BindCommand(0, dae::Gamepad::Button::DPadRight, dae::KeyState::Pressed,
		std::make_unique<MoveCommand>(pCenter, glm::vec2{ 1, 0 }, speed));
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
