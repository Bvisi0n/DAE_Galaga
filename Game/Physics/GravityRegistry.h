#ifndef GRAVITYREGISTRY_H
#define GRAVITYREGISTRY_H

#include <span>
#include <vector>
#include <memory>

#include <glm/ext/vector_float3.hpp>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_pixels.h>

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
			auto visual = std::make_unique<dae::core::GameObject>();
			visual->GetTransform().SetLocalPosition( node.position );
			SDL_FRect bounds{ node.position.x - 1.f, node.position.y - 1.f , 2.f, 2.f };
			SDL_Color color{ 255, 0, 0, 255 };
			visual->AddComponent<dae::graphics::PrimitiveRenderComponent>( bounds, color, 2 );
			auto* rawPtr = visual.get();
			s_visualObjects.push_back( rawPtr );
			scene.AddGameObject( std::move( visual ) );
		}

		static void ClearNodes()
		{
			s_nodes.clear();
			if ( !s_visualObjects.empty() )
			{
				auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();
				for ( auto* obj : s_visualObjects )
				{
					if ( obj )
					{
						scene.RemoveGameObject( *obj );
					}
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
		static inline std::vector<dae::core::GameObject*> s_visualObjects{};
	};
}
#endif
