#include <algorithm>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <glm/detail/type_vec3.hpp>

#include <SDL3/SDL_pixels.h>

#include <Minigin/Core/GameObject.h>
#include <Minigin/Graphics/PrimitiveRenderComponent.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/GravityBender/GravityRegistry.h>

namespace bvi::gravity_bender
{
	void GravityRegistry::Update( float deltaTime )
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

	void GravityRegistry::AddNode( const GravityNode& node )
	{
		s_nodes.push_back( node );
		auto& storedNode = s_nodes.back();

		using PrimitiveRenderComponent = dae::graphics::PrimitiveRenderComponent;
		using PrimitiveShape = dae::graphics::PrimitiveShape;
		using CircleShape = dae::graphics::CircleShape;

		auto centerVisualRoot = std::make_unique<dae::core::GameObject>();
		centerVisualRoot->GetTransform().SetLocalPosition( node.position );
		// TODO bvi_gravity_bender - Hardcoded the radius here multiple times.
		centerVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto centerVisualOuter = std::make_unique<dae::core::GameObject>();
		centerVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

		centerVisualOuter->SetParent( centerVisualRoot.get(), false );
		storedNode.visualRoot = centerVisualRoot.get();

		// TODO bvi_gravity_bender - Make this dry.

		// TODO bvi_gravity_bender - Missing some edge cases for the corners.

		// TODO bvi_gravity_bender - Missing the actual gravity wells.

		// TODO bvi_gravity_bender - Fetch viewport dimensions and use them. (1024x576)
		auto northVisualRoot = std::make_unique<dae::core::GameObject>( 0.f, -576.f );
		northVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto northVisualOuter = std::make_unique<dae::core::GameObject>();
		northVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

		northVisualRoot->SetParent( centerVisualRoot.get(), false );
		northVisualOuter->SetParent( northVisualRoot.get(), false );

		auto eastVisualRoot = std::make_unique<dae::core::GameObject>( 1024.f, 0.f );
		eastVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto eastVisualOuter = std::make_unique<dae::core::GameObject>();
		eastVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

		eastVisualRoot->SetParent( centerVisualRoot.get(), false );
		eastVisualOuter->SetParent( eastVisualRoot.get(), false );

		auto southVisualRoot = std::make_unique<dae::core::GameObject>( 0.f, 576.f );
		southVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto southVisualOuter = std::make_unique<dae::core::GameObject>();
		southVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

		southVisualRoot->SetParent( centerVisualRoot.get(), false );
		southVisualOuter->SetParent( southVisualRoot.get(), false );

		auto westVisualRoot = std::make_unique<dae::core::GameObject>( -1024.f, 0.f );
		westVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.0f, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto westVisualOuter = std::make_unique<dae::core::GameObject>();
		westVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.0f, false } }, SDL_Color{ 0, 255, 255, 128 } );

		westVisualRoot->SetParent( centerVisualRoot.get(), false );
		westVisualOuter->SetParent( westVisualRoot.get(), false );

		auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();
		scene.AddGameObject( std::move( centerVisualRoot ) );
		scene.AddGameObject( std::move( centerVisualOuter ) );
		scene.AddGameObject( std::move( northVisualRoot ) );
		scene.AddGameObject( std::move( northVisualOuter ) );
		scene.AddGameObject( std::move( eastVisualRoot ) );
		scene.AddGameObject( std::move( eastVisualOuter ) );
		scene.AddGameObject( std::move( southVisualRoot ) );
		scene.AddGameObject( std::move( southVisualOuter ) );
		scene.AddGameObject( std::move( westVisualRoot ) );
		scene.AddGameObject( std::move( westVisualOuter ) );
	}

	void GravityRegistry::SetPlayerNode( const GravityNode& node )
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

	void GravityRegistry::RemovePlayerNode()
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

	void GravityRegistry::ClearNodes()
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

	const std::vector<GravityNode>& GravityRegistry::GetActiveNodes() noexcept
	{
		return s_nodes;
	}
}