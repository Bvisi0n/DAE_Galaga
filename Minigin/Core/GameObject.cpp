#include <algorithm>
#include <vector>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/Transform.h>
#include <Minigin/Graphics/IRenderable.h>

namespace dae::core
{
	GameObject::GameObject( const TransformDescriptor& transformConfig )
		: m_transform{ this, transformConfig }
	{}

	GameObject::~GameObject()
	{
		// TODO dae_core - Works but doesn't feel right, research is required. Who owns what, use smart pointers?
		for ( auto* child : m_children )
		{
			if ( child != nullptr )
			{
				child->m_parent = nullptr;
			}
		}

		if ( m_parent != nullptr )
		{
			m_parent->RemoveChild( this );
		}
	}

	void GameObject::MarkForDeletion()
	{
		m_isPendingDeletion = true;

		for ( auto* child : m_children )
		{
			if ( child != nullptr )
			{
				child->MarkForDeletion();
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
		const size_t componentCount = m_components.size();
		for ( size_t i = 0; i < componentCount; ++i )
		{
			m_components[ i ]->AdvanceState();
		}
	}

	void GameObject::Update( const float deltaTime )
	{
		if ( m_isPendingDeletion )
		{
			return;
		}

		// std::vector iterator invalidation
		const size_t componentCount = m_components.size();
		for ( size_t i = 0; i < componentCount; ++i )
		{
			auto* component = m_components[ i ].get();

			if ( component->IsPendingDeletion() )
			{
				continue;
			}

			if ( component->GetExecutionState() != ComponentState::Ready )
			{
				if ( component->AdvanceState() != ComponentState::Ready )
				{
					continue;
				}
			}

			component->Update( deltaTime );
		}

		CleanupComponents();
	}

	void GameObject::Render() const
	{
		if ( m_renderable != nullptr )
		{
			m_renderable->Render();
		}
	}

	void GameObject::SetParent( GameObject* parent, bool keepWorldPosition )
	{
		if ( IsChild( parent ) || parent == this || m_parent == parent )
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

		if ( m_parent != nullptr )
		{
			m_parent->RemoveChild( this );
		}

		m_parent = parent;

		if ( m_parent != nullptr )
		{
			m_parent->AddChild( this );
		}
	}

	GameObject* GameObject::GetParent() const
	{
		return m_parent;
	}

	const std::vector<GameObject*>& GameObject::GetChildren() const
	{
		return m_children;
	}

	Transform& GameObject::GetTransform()
	{
		return m_transform;
	}

	const Transform& GameObject::GetTransform() const
	{
		return m_transform;
	}

	bool GameObject::IsChild( GameObject* candidate ) const
	{
		if ( candidate == nullptr )
		{
			return false;
		}

		return std::ranges::any_of
		(
			m_children,
			[ candidate ] ( auto* child )
			{
				return ( child == candidate ) || child->IsChild( candidate );
			}
		);
	}

	void GameObject::AddChild( GameObject* child )
	{
		if ( child == nullptr )
		{
			return;
		}

		if ( std::ranges::find( m_children, child ) == m_children.end() )
		{
			m_children.emplace_back( child );
		}
	}

	void GameObject::RemoveChild( GameObject* child )
	{
		if ( child == nullptr )
		{
			return;
		}

		std::erase( m_children, child );

		if ( child->GetParent() == this )
		{
			child->m_parent = nullptr;
		}
	}

	void GameObject::CleanupComponents()
	{
		std::erase_if( m_components,
			[] ( const auto& component )
			{
				return component->IsPendingDeletion();
			} );
	}
}
