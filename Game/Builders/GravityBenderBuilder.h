#ifndef GRAVITYBENDERBUILDER_H
#define GRAVITYBENDERBUILDER_H

#include <memory>

#include "Game/Components/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/Scene.h"

namespace bvi::builders
{
	class GravityBenderBuilder final
	{
	public:
		GravityBenderBuilder() = delete;

		static void Build( dae::scenes::Scene& scene )
		{
			BuildBackground( scene );
			BuildFPSCounter( scene );
			scene.Initialize();
		}

	private:
		static void BuildBackground( dae::scenes::Scene& scene )
		{
			auto background{ std::make_unique<dae::core::GameObject>() };
			background->AddComponent<dae::graphics::TextureComponent>()->SetTexture( "background.png" );
			scene.AddGameObject( std::move( background ) );
		}

		static void BuildFPSCounter( dae::scenes::Scene& scene )
		{
			auto font{ dae::resources::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 18 ) };
			auto fpsCounter{ std::make_unique<dae::core::GameObject>( 10.f, 10.f ) };
			fpsCounter->AddComponent<dae::graphics::TextComponent>( "00.0 FPS", font )->SetColor( SDL_Color{ 255, 0, 0, 255 } );
			fpsCounter->AddComponent<components::FPSComponent>();
			scene.AddGameObject( std::move( fpsCounter ) );
		}
	};
}
#endif
