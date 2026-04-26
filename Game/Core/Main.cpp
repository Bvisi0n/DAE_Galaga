#include <filesystem>
#include <memory>

#include <SDL3/SDL_main.h> // Required

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h> // Required
#endif

#include "Game/MainMenu/MainMenuState.h"

#include "Minigin/Core/AppStateManager.h"
#include "Minigin/Core/Minigin.h"

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

	dae::core::AppStateManager::GetInstance().ChangeState( std::make_unique<bvi::main_menu::MainMenuState>() );

	engine.Run();

	return 0;
}
