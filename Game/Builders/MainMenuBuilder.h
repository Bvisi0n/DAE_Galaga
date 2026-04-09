#ifndef MENUBUILDER_H
#define MENUBUILDER_H

#include <memory>

#include "Game/Components/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"

// TODO L: Review builder pattern.
	// https://refactoring.guru/design-patterns/builder
	// https://www.geeksforgeeks.org/system-design/builder-pattern-c-design-patterns/
	// Both sources show a director and don't use static methods. This seems better for my needs tho. 
	// More research is needed. Maybe this is another pattern? Feels a bit like it's director and builder mashed in a single class.

namespace bvi::builders
{
	class MainMenuBuilder final
	{
	public:
		MainMenuBuilder() = delete;

		static void Build( dae::scenes::Scene& scene )
		{
			BuildBackground( scene );
			BuildLogo( scene );
			BuildTitle( scene );
			BuildFPSCounter( scene );
			BuildMenu( scene );
			scene.Initialize();
		}

	private:
		static void BuildBackground( dae::scenes::Scene& scene )
		{
			auto background{ std::make_unique<dae::core::GameObject>() };
			background->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "background.png" );
			scene.AddGameObject( std::move( background ) );
		}

		static void BuildLogo( dae::scenes::Scene& scene )
		{
			auto logo{ std::make_unique<dae::core::GameObject>( 358.f, 180.f ) };
			logo->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "logo.png" );
			scene.AddGameObject( std::move( logo ) );
		}

		static void BuildTitle( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 ) };
			auto title{ std::make_unique<dae::core::GameObject>( 292.f, 20.f ) };
			title->AddComponent<dae::graphics::TextComponent>( "Programming 4 Assignment", font )->SetColor( { 255, 255, 0, 255 } );
			scene.AddGameObject( std::move( title ) );
		}

		static void BuildFPSCounter( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };
			auto fpsCounter{ std::make_unique<dae::core::GameObject>( 20.f, 20.f ) };
			fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font );
			fpsCounter->AddComponent<components::FPSComponent>();
			scene.AddGameObject( std::move( fpsCounter ) );
		}

		static void BuildMenu( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };
			auto instructions{ std::make_unique<dae::core::GameObject>( 425.f, 375.f ) };
			instructions->AddComponent<dae::graphics::TextComponent>( "Press F to start", font );
			scene.AddGameObject( std::move( instructions ) );
		}
	};
}
#endif
