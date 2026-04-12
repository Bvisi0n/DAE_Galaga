#ifndef GRAVITYREGISTRY_H
#define GRAVITYREGISTRY_H

#include <memory>
#include <span>
#include <utility>
#include <vector>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <glm/ext/vector_float3.hpp>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/PrimitiveRenderComponent.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::physics
{
	struct GravityNode
	{
		glm::vec3 position;
		float strength;
		float radiusSquared;
	};

	class GravityRegistry final
	{
	public:
		GravityRegistry() = delete;

		static void AddNode( const GravityNode& node )
		{
			s_nodes.push_back( node );
			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			auto visualInner = std::make_unique<dae::core::GameObject>();
			auto VisualOuter = std::make_unique<dae::core::GameObject>();

			visualInner->GetTransform().SetLocalPosition( node.position );
			VisualOuter->GetTransform().SetLocalPosition( node.position );

			visualInner->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 5.0f, true } }, SDL_Color{ 255, 165, 0, 255 } );

			// TODO L: Hardcoded the radius here.
			VisualOuter->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 150.0f, false } }, SDL_Color{ 255, 165, 0, 128 } );

			auto* rawInnerPtr = visualInner.get();
			auto* rawOuterPtr = VisualOuter.get();
			s_visualObjects.emplace_back( rawInnerPtr, rawOuterPtr );

			scene.AddGameObject( std::move( visualInner ) );
			scene.AddGameObject( std::move( VisualOuter ) );
		}

		static void ClearNodes()
		{
			s_nodes.clear();
			if ( !s_visualObjects.empty() )
			{
				auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

				for ( const auto& [innerObj, outerObj] : s_visualObjects )
				{
					if ( innerObj ) scene.RemoveGameObject( *innerObj );
					if ( outerObj ) scene.RemoveGameObject( *outerObj );
				}
				s_visualObjects.clear();
			}
		}

		[[nodiscard]] static std::span<const GravityNode> GetActiveNodes() noexcept
		{
			return s_nodes;
		}

	private:
		static inline std::vector<GravityNode> s_nodes{};
		static inline std::vector<std::pair<dae::core::GameObject*, dae::core::GameObject*>> s_visualObjects{};
	};
}
#endif
