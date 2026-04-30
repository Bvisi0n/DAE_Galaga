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

			if ( node.lifeTimeRemaining <= 0.0F )
			{
				if ( node.visualRoot != nullptr )
				{
					scene.RemoveGameObject( *node.visualRoot );
				}

				node.visualRoot = nullptr;
			}
		}

		std::erase_if( s_nodes,
					   [] ( const GravityNode& node )
					   {
						   return node.lifeTimeRemaining <= 0.0F;
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
		centerVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.0F, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto centerVisualOuter = std::make_unique<dae::core::GameObject>();
		centerVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.0F, false } }, SDL_Color{ 0, 255, 255, 128 } );

		centerVisualOuter->SetParent( centerVisualRoot.get(), false );
		storedNode.visualRoot = centerVisualRoot.get();

		// TODO bvi_gravity_bender - Make this dry.

		// TODO bvi_gravity_bender - Missing some edge cases for the corners.

		// TODO bvi_gravity_bender - Missing the actual gravity wells.

		// TODO bvi_gravity_bender - Fetch viewport dimensions and use them.
		auto northVisualRoot = std::make_unique<dae::core::GameObject>( 0.F, -576.F );
		northVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.F, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto northVisualOuter = std::make_unique<dae::core::GameObject>();
		northVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.F, false } }, SDL_Color{ 0, 255, 255, 128 } );

		northVisualRoot->SetParent( centerVisualRoot.get(), false );
		northVisualOuter->SetParent( northVisualRoot.get(), false );

		auto eastVisualRoot = std::make_unique<dae::core::GameObject>( 1024.F, 0.F );
		eastVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.F, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto eastVisualOuter = std::make_unique<dae::core::GameObject>();
		eastVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.F, false } }, SDL_Color{ 0, 255, 255, 128 } );

		eastVisualRoot->SetParent( centerVisualRoot.get(), false );
		eastVisualOuter->SetParent( eastVisualRoot.get(), false );

		auto southVisualRoot = std::make_unique<dae::core::GameObject>( 0.F, 576.F );
		southVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.F, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto southVisualOuter = std::make_unique<dae::core::GameObject>();
		southVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.F, false } }, SDL_Color{ 0, 255, 255, 128 } );

		southVisualRoot->SetParent( centerVisualRoot.get(), false );
		southVisualOuter->SetParent( southVisualRoot.get(), false );

		auto westVisualRoot = std::make_unique<dae::core::GameObject>( -1024.F, 0.F );
		westVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 5.F, true } }, SDL_Color{ 0, 255, 255, 255 } );
		auto westVisualOuter = std::make_unique<dae::core::GameObject>();
		westVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ 100.F, false } }, SDL_Color{ 0, 255, 255, 128 } );

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
		auto iterator = std::ranges::find_if( s_nodes,
								[] ( const GravityNode& n )
								{
									return n.isPlayer;
								} );

		if ( iterator == s_nodes.end() )
		{
			GravityNode playerNode = node;
			playerNode.lifeTimeRemaining = std::numeric_limits<float>::infinity();
			playerNode.isPlayer = true;

			AddNode( playerNode );
		}
		else
		{
			iterator->position = node.position;
			iterator->strength = node.strength;
			iterator->radiusSquared = node.radiusSquared;

			if ( iterator->visualRoot != nullptr )
			{
				iterator->visualRoot->GetTransform().SetLocalPosition( node.position );
			}
		}
	}

	void GravityRegistry::RemovePlayerNode()
	{
		auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

		auto iterator = std::ranges::find_if( s_nodes,
								[] ( const GravityNode& n )
								{
									return n.isPlayer;
								} );

		if ( iterator != s_nodes.end() )
		{
			if ( iterator->visualRoot != nullptr )
			{
				scene.RemoveGameObject( *( iterator->visualRoot ) );
			}

			s_nodes.erase( iterator );
		}
	}

	void GravityRegistry::ClearNodes()
	{
		auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

		for ( const auto& node : s_nodes )
		{
			if ( node.visualRoot != nullptr )
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