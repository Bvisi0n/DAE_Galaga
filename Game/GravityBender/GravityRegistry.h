#ifndef GRAVITYREGISTRY_H
#define GRAVITYREGISTRY_H

#include <limits>
#include <memory>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include <SDL3/SDL_pixels.h>

#include <vector>
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
	};

	class GravityRegistry final
	{
	public:
		GravityRegistry() = delete;

		static void Update( float deltaTime )
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			// TODO GRAV: Old for loop, why?
			for ( size_t i = 0; i < s_nodes.size(); )
			{
				s_nodes[ i ].lifeTimeRemaining -= deltaTime;

				if ( s_nodes[ i ].lifeTimeRemaining <= 0.0f )
				{
					auto& [innerObj, outerObj] = s_visualObjects[ i ];
					if ( innerObj ) scene.RemoveGameObject( *innerObj );
					if ( outerObj ) scene.RemoveGameObject( *outerObj );

					s_visualObjects.erase( s_visualObjects.begin() + i );
					s_nodes.erase( s_nodes.begin() + i );

					if ( s_playerNodeIndex != -1 )
					{
						if ( static_cast<size_t>( s_playerNodeIndex ) == i )
						{
							s_playerNodeIndex = -1;
						}
						else if ( static_cast<size_t>( s_playerNodeIndex ) > i )
						{
							--s_playerNodeIndex;
						}
					}
				}
				else
				{
					++i;
				}
			}
		}

		static void AddNode( const GravityNode& node )
		{
			s_nodes.push_back( node );
			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			auto visualInner = std::make_unique<dae::core::GameObject>();
			auto VisualOuter = std::make_unique<dae::core::GameObject>();

			visualInner->GetTransform().SetLocalPosition( node.position );
			VisualOuter->GetTransform().SetLocalPosition( node.position );

			visualInner->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );

			// TODO GRAV: Hardcoded the radius here.
			VisualOuter->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

			// TODO GRAV: Use the scene graph to parent these.
			s_visualObjects.emplace_back( visualInner.get(), VisualOuter.get() );

			scene.AddGameObject( std::move( visualInner ) );
			scene.AddGameObject( std::move( VisualOuter ) );
		}

		static void SetPlayerNode( const GravityNode& node )
		{
			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			if ( s_playerNodeIndex == -1 )
			{
				GravityNode tmp = node;
				tmp.lifeTimeRemaining = std::numeric_limits<float>::infinity();

				s_nodes.push_back( tmp );

				auto visualInner = std::make_unique<dae::core::GameObject>();
				auto visualOuter = std::make_unique<dae::core::GameObject>();

				visualInner->GetTransform().SetLocalPosition( node.position );
				visualOuter->GetTransform().SetLocalPosition( node.position );

				visualInner->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );
				visualOuter->AddComponent<dae::graphics::PrimitiveRenderComponent>( dae::graphics::PrimitiveShape{ dae::graphics::CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

				s_visualObjects.emplace_back( visualInner.get(), visualOuter.get() );

				s_playerNodeIndex = static_cast<int>( s_nodes.size() - 1 );

				scene.AddGameObject( std::move( visualInner ) );
				scene.AddGameObject( std::move( visualOuter ) );
			}
			else
			{
				s_nodes[ s_playerNodeIndex ].position = node.position;
				s_nodes[ s_playerNodeIndex ].strength = node.strength;
				s_nodes[ s_playerNodeIndex ].radiusSquared = node.radiusSquared;
				s_nodes[ s_playerNodeIndex ].lifeTimeRemaining = std::numeric_limits<float>::infinity();

				auto& [innerObj, outerObj] = s_visualObjects[ s_playerNodeIndex ];
				if ( innerObj ) innerObj->GetTransform().SetLocalPosition( node.position );
				if ( outerObj ) outerObj->GetTransform().SetLocalPosition( node.position );
			}
		}

		static void RemovePlayerNode()
		{
			if ( s_playerNodeIndex == -1 ) return;

			auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

			auto& [innerObj, outerObj] = s_visualObjects[ s_playerNodeIndex ];
			if ( innerObj ) scene.RemoveGameObject( *innerObj );
			if ( outerObj ) scene.RemoveGameObject( *outerObj );

			s_visualObjects.erase( s_visualObjects.begin() + s_playerNodeIndex );
			s_nodes.erase( s_nodes.begin() + s_playerNodeIndex );

			s_playerNodeIndex = -1;
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
			s_playerNodeIndex = -1;
		}

		[[nodiscard]] static const std::vector<GravityNode>& GetActiveNodes() noexcept
		{
			return s_nodes;
		}

	private:
		static inline std::vector<GravityNode> s_nodes{};
		static inline std::vector<std::pair<dae::core::GameObject*, dae::core::GameObject*>> s_visualObjects{};
		static inline int s_playerNodeIndex{ -1 };
	};
}
#endif
