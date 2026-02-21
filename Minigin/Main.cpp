#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include <filesystem>

#include "Components/FPSComponent.h"
#include "Components/TextureComponent.h"
#include "Components/TextComponent.h"
#include "ResourceManager.h"
#include "Scene.h"

// Priority:	0 Breaks compilation
//				1 New manditory feature
//				2 Based on feedback
//				3 Optimize
//				4 Tweak
//				5 Question

// TODO 1: Add code to your GameObject class enabling users to create a hierarchical scenegraph.
//          It must be possible to attach a child to a parent.
//          It must be possible to detach a child from a parent.
//          It must be possible to iterate over the children of a parent.
// TODO 1: Apply the dirty flag pattern when the position of a gameobject is changed.
// TODO 1: Prove that your implementation works.
//          Have a character from your game move in a circle on screen.
//          Have another character from your game rotate around the first one.
//          If implemented correctly, all you need is one extra component that is in charge of this rotation.

// TODO 2: When removing a GameObject from the scene, use a flag to mark it for deletion.
// TODO 2: FPSComponent should own a reference to a TextComponent instead of owning one.
// TODO 2: TextComponent should own a reference to a TextureComponent instead of owning one.
// TODO 2: Create RenderComponent and move all rendering logic there.
// TODO 2: In FPSComponent SetText() & Update() should be happenign in the same method.
// TODO 2: Create a TransformComponent.
// TODO 2: Review the component management flow.
//			Add a component to a game object.
//			Remove a component from a game object in a safe manner.
//			Get a component from a game object.
//			Check whether a component has been added.
// TODO 2: Review the entire update flow.
// TODO 2: Review game loop, what happens when frames start to take to long? Should we cap the max delta time?

// TODO 3: Split engine and game into dll & exe.
// TODO 3: Improve folder structure.

// TODO 5: Is using std::ostringstream in FPSComponent::Update expensive?
// TODO 5: Is there a place for string_view when passing all these strings around?
// TODO 5: Should Texture2D exist outside of TextureComponent?
// TODO 5: Should Transform exist outside of TransformComponent?
// TODO 5: Should a GameObject be able to have the same component multiple times?

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto background = std::make_unique<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>()->SetTexture("background.png");
	scene.Add(std::move(background));

	auto logo = std::make_unique<dae::GameObject>();
	logo->SetPosition(358, 180);
	logo->AddComponent<dae::TextureComponent>()->SetTexture("logo.png");
	scene.Add(std::move(logo));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto title = std::make_unique<dae::GameObject>();
	title->SetPosition(292, 20);
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", font)->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(title));
	
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto fps = std::make_unique<dae::GameObject>();
	fps->SetPosition(20, 20);
	fps->AddComponent<dae::FPSComponent>(font);
	scene.Add(std::move(fps));
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
	engine.Run(load);
    return 0;
}
