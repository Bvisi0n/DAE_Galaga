#ifndef GRAVITYBENDERBUILDER_H
#define GRAVITYBENDERBUILDER_H

#include <memory>

#include "SDL3/SDL.h"

#include "Game/Components/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/PrimitiveRenderComponent.h"
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
			BuildViewportBorder( scene );
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

		static void BuildViewportBorder( dae::scenes::Scene& scene )
		{
			// TODO H: Fetch screen dimensions and use them.
			constexpr SDL_FRect screenBounds{ 0.f, 0.f, 1024.f, 576.f };
			constexpr SDL_Color neonPurple{ 128, 0, 128, 255 };

			auto borderObject{ std::make_unique<dae::core::GameObject>() };
			borderObject->AddComponent<dae::graphics::PrimitiveRenderComponent>( screenBounds, neonPurple );

			scene.AddGameObject( std::move( borderObject ) );
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
