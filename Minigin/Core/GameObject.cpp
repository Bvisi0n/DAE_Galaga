#include <algorithm>
#include <vector>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"
#include "Minigin/Graphics/IRenderable.h"

namespace dae::core
{
	GameObject::GameObject( const float x, const float y )
		: m_transform{ this, x, y }
	{}

	GameObject::~GameObject()
	{
		// TODO L: Works but doesn't feel right, research is required. Who owns what, use smart pointers?
		for ( auto* p_child : m_pChildren )
		{
			if ( p_child )
			{
				p_child->m_pParent = nullptr;
			}
		}

		if ( m_pParent )
		{
			m_pParent->RemoveChild( this );
		}
	}

	void GameObject::MarkForDeletion()
	{
		m_isPendingDeletion = true;

		for ( auto* p_child : m_pChildren )
		{
			if ( p_child )
			{
				p_child->MarkForDeletion();
			}
		}
	}

	bool GameObject::IsPendingDeletion() const noexcept
	{
		return m_isPendingDeletion;
	}

	void GameObject::AdvanceComponentStates() noexcept
	{
		// std::vector iterator invalidation
		const size_t component_count = m_pComponents.size();
		for ( size_t i = 0; i < component_count; ++i )
		{
			m_pComponents[ i ]->AdvanceState();
		}
	}

	void GameObject::Update( const float deltaTime )
	{
		if ( m_isPendingDeletion )
		{
			return;
		}

		// std::vector iterator invalidation
		const size_t component_count = m_pComponents.size();
		for ( size_t i = 0; i < component_count; ++i )
		{
			auto* p_component = m_pComponents[ i ].get();

			if ( p_component->IsPendingDeletion() )
			{
				continue;
			}

			if ( p_component->GetExecutionState() != ComponentState::Ready )
			{
				if ( p_component->AdvanceState() != ComponentState::Ready )
				{
					continue;
				}
			}

			p_component->Update( deltaTime );
		}

		CleanupComponents();
	}

	void GameObject::Render() const
	{
		if ( m_pRenderable )
		{
			m_pRenderable->Render();
		}
	}

	void GameObject::SetParent( GameObject* parent, bool keepWorldPosition )
	{
		if ( IsChild( parent ) || parent == this || m_pParent == parent )
		{
			return;
		}

		if ( keepWorldPosition )
		{
			if ( parent == nullptr )
			{
				m_transform.SetLocalPosition( m_transform.GetWorldPosition() );
			}
			else
			{
				m_transform.SetLocalPosition( m_transform.GetWorldPosition() - parent->GetTransform().GetWorldPosition() );
			}
		}
		//else if (!keepWorldPosition) the current LocalPosition is preserved as is

		if ( m_pParent )
		{
			m_pParent->RemoveChild( this );
		}

		m_pParent = parent;

		if ( m_pParent )
		{
			m_pParent->AddChild( this );
		}
	}

	GameObject* GameObject::GetParent() const
	{
		return m_pParent;
	}

	const std::vector<GameObject*>& GameObject::GetChildren() const
	{
		return m_pChildren;
	}

	Transform& GameObject::GetTransform()
	{
		return m_transform;
	}

	const Transform& GameObject::GetTransform() const
	{
		return m_transform;
	}

	bool GameObject::IsChild( GameObject* pCandidate )
	{
		if ( pCandidate == nullptr ) return false;

		for ( auto p_child : m_pChildren )
		{
			if ( p_child == pCandidate ) return true;
			if ( p_child->IsChild( pCandidate ) ) return true;
		}
		return false;
	}

	void GameObject::AddChild( GameObject* pChild )
	{
		if ( pChild == nullptr )
		{
			return;
		}
		else
		{
			if ( std::ranges::find( m_pChildren, pChild ) == m_pChildren.end() )
			{
				m_pChildren.emplace_back( pChild );
			}
		}
	}

	void GameObject::RemoveChild( GameObject* pChild )
	{
		if ( pChild == nullptr )
		{
			return;
		}
		else
		{
			std::erase( m_pChildren, pChild );

			if ( pChild->GetParent() == this )
			{
				pChild->m_pParent = nullptr;
			}
		}
	}

	void GameObject::CleanupComponents()
	{
		std::erase_if( m_pComponents,
			[] ( const auto& pComponent )
			{
				return pComponent->IsPendingDeletion();
			} );
	}
}
