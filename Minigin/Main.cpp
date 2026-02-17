#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include <filesystem>

#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"

namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto game_object = std::make_unique<dae::GameObject>();
	game_object->SetTexture("background.png");
	scene.Add(std::move(game_object));

	game_object = std::make_unique<dae::GameObject>();
	game_object->SetTexture("logo.png");
	game_object->SetPosition(358, 180);
	scene.Add(std::move(game_object));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto text_object = std::make_unique<dae::TextObject>("Programming 4 Assignment", font);
	text_object->SetColor({ 255, 255, 0, 255 });
	text_object->SetPosition(292, 20);
	scene.Add(std::move(text_object));
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
