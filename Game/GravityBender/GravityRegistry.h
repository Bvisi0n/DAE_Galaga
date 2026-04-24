#ifndef BVI_GRAVITYREGISTRY_H
#define BVI_GRAVITYREGISTRY_H

#include <algorithm>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <glm/ext/vector_float3.hpp>

#include <SDL3/SDL_pixels.h>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/PrimitiveRenderComponent.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::gravity_bender
{
	struct GravityNode
	{
		glm::vec3 position;
		float strength;
		float radiusSquared;
		float lifeTimeRemaining{ 3.0f };
		dae::core::GameObject* visualRoot{ nullptr };
		bool isPlayer{ false };
	};

	class GravityRegistry final
	{
	public:
		GravityRegistry() = delete;

		static void Update( float deltaTime )
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			for ( auto& node : s_nodes )
			{
				if ( !node.isPlayer )
				{
					node.lifeTimeRemaining -= deltaTime;
				}

				if ( node.lifeTimeRemaining <= 0.0f )
				{
					if ( node.visualRoot )
					{
						scene.RemoveGameObject( *node.visualRoot );
					}

					node.visualRoot = nullptr;
				}
			}

			std::erase_if( s_nodes,
						   [] ( const GravityNode& node )
						   {
							   return node.lifeTimeRemaining <= 0.0f;
						   } );
		}

		static void AddNode( const GravityNode& node )
		{
			s_nodes.push_back( node );
			auto& storedNode = s_nodes.back();

			using PrimitiveRenderComponent = dae::graphics::PrimitiveRenderComponent;
			using PrimitiveShape = dae::graphics::PrimitiveShape;
			using CircleShape = dae::graphics::CircleShape;

			auto visualRoot = std::make_unique<dae::core::GameObject>();
			visualRoot->GetTransform().SetLocalPosition( node.position );
			visualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );

			auto visualOuter = std::make_unique<dae::core::GameObject>();
			// TODO bvi_gravity_bender - Hardcoded the radius here.
			visualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

			visualOuter->SetParent( visualRoot.get(), false );
			storedNode.visualRoot = visualRoot.get();

			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();
			scene.AddGameObject( std::move( visualRoot ) );
			scene.AddGameObject( std::move( visualOuter ) );
		}

		static void SetPlayerNode( const GravityNode& node )
		{
			auto it = std::find_if( s_nodes.begin(), s_nodes.end(),
									[] ( const GravityNode& n )
									{
										return n.isPlayer;
									} );

			if ( it == s_nodes.end() )
			{
				GravityNode tmp = node;
				tmp.lifeTimeRemaining = std::numeric_limits<float>::infinity();
				tmp.isPlayer = true;

				AddNode( tmp );
			}
			else
			{
				it->position = node.position;
				it->strength = node.strength;
				it->radiusSquared = node.radiusSquared;

				if ( it->visualRoot )
				{
					it->visualRoot->GetTransform().SetLocalPosition( node.position );
				}
			}
		}

		static void RemovePlayerNode()
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			auto it = std::find_if( s_nodes.begin(), s_nodes.end(),
									[] ( const GravityNode& n )
									{
										return n.isPlayer;
									} );

			if ( it != s_nodes.end() )
			{
				if ( it->visualRoot )
				{
					scene.RemoveGameObject( *( it->visualRoot ) );
				}

				s_nodes.erase( it );
			}
		}

		static void ClearNodes()
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			for ( const auto& node : s_nodes )
			{
				if ( node.visualRoot )
				{
					scene.RemoveGameObject( *node.visualRoot );
				}
			}

			s_nodes.clear();
		}

		[[nodiscard]] static const std::vector<GravityNode>& GetActiveNodes() noexcept
		{
			return s_nodes;
		}

	private:
		static inline std::vector<GravityNode> s_nodes{};
	};
}
#endif
