#ifndef MENUBUILDER_H
#define MENUBUILDER_H

#include <memory>

#include "Game/Components/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"

// TODO L: Review builder pattern properly.
	// https://refactoring.guru/design-patterns/builder

namespace bvi::builders
{
	class MenuBuilder final
	{
	public:
		MenuBuilder() = delete;

		static void Build( dae::scenes::Scene& scene )
		{
			BuildBackground( scene );
			BuildLogo( scene );
			BuildTitle( scene );
			BuildFPS( scene );
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

		static void BuildFPS( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 24 ) };
			auto fps{ std::make_unique<dae::core::GameObject>( 20.f, 20.f ) };
			fps->AddComponent<dae::graphics::TextComponent>( "FPS", font );
			fps->AddComponent<components::FPSComponent>();
			scene.AddGameObject( std::move( fps ) );
		}
	};
}
#endif
