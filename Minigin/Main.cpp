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
#include "TextObject.h"
#include "Scene.h"

namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//auto background = std::make_unique<dae::GameObject>();
	//background->AddComponent<dae::RenderComponent>()->SetTexture("background.png");
	//scene.Add(std::move(background));

	//auto logo = std::make_unique<dae::GameObject>();
	//logo->SetPosition(358, 180);
	//logo->AddComponent<dae::RenderComponent>()->SetTexture("logo.png");
	//scene.Add(std::move(logo));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto title = std::make_unique<dae::GameObject>();
	title->SetPosition(292, 20);
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", font)->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(title));
	
	auto fps = std::make_unique<dae::GameObject>();
	fps->SetPosition(20, 20);
	fps->AddComponent<dae::FPSComponent>(font);
	scene.Add(std::move(fps));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
