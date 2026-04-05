#include <string>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"
#include "Minigin/Graphics/IRenderable.h"
#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Resources/ResourceManager.h"

namespace dae::core
{
	GameObject::GameObject(const float x, const float y)
		: m_transform{ this, x, y } {}

	GameObject::~GameObject()
	{
		// TODO L: Works but doesn't feel right, research is required. Who owns what, use smart pointers?
		for (auto* pChild : m_pChildren)
		{
			if (pChild)
			{
				pChild->m_pParent = nullptr;
			}
		}

		if (m_pParent && !m_pParent->IsPendingDeletion())
		{
			m_pParent->RemoveChild(this);
		}
	}
	
	void GameObject::MarkForDeletion()
	{
		m_isPendingDeletion = true;

		for (auto* pChild : m_pChildren)
		{
			if (pChild)
			{
				pChild->MarkForDeletion();
			}
		}
	}

	bool GameObject::IsPendingDeletion() const noexcept
	{
		return m_isPendingDeletion;
	}

	void GameObject::InitializeLinkage()
	{
		for (auto& comp : m_pComponents)
		{
			comp->InitializeLinkage();
		}
	}

	void GameObject::InitializeState()
	{
		for (auto& comp : m_pComponents)
		{
			comp->InitializeState();
		}
	}

	void GameObject::Update(const float deltaTime)
	{
		if (m_isPendingDeletion)
		{
			return;
		}

		// Don't change to ranged based for loop:
		// std::vector iterator invalidation https://en.cppreference.com/w/cpp/container/vector.html
		const size_t component_count = m_pComponents.size();
		for (size_t i = 0; i < component_count; ++i)
		{
			if (!m_pComponents[i]->IsPendingDeletion())
			{
				m_pComponents[i]->Update(deltaTime);
			}
		}

		CleanupComponents();
	}

	void GameObject::Render() const
	{
		if (m_pRenderable)
		{
			m_pRenderable->Render();
		}
	}

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		if (IsChild(parent) || parent == this || m_pParent == parent)
		{
			return;
		}

		if (keepWorldPosition)
		{
			if (parent == nullptr)
			{
				m_transform.SetLocalPosition(m_transform.GetWorldPosition());
			}
			else
			{
				m_transform.SetLocalPosition(m_transform.GetWorldPosition() - parent->GetTransform().GetWorldPosition());
			}
		}
		//else if (!keepWorldPosition) the current LocalPosition is preserved as is

		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}

		m_pParent = parent;

		if (m_pParent)
		{
			m_pParent->AddChild(this);
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

	bool GameObject::IsChild(GameObject* candidate)
	{
		if (candidate == nullptr) return false;

		for (auto child : m_pChildren)
		{
			if (child == candidate) return true;
			if (child->IsChild(candidate)) return true;
		}
		return false;
	}

	void GameObject::AddChild(GameObject* pChild)
	{
		if (pChild == nullptr)
		{
			return;
		}
		else
		{
			if (std::ranges::find(m_pChildren, pChild) == m_pChildren.end())
			{
				m_pChildren.emplace_back(pChild);
			}
		}
	}

	void GameObject::RemoveChild(GameObject* pChild)
	{
		if (pChild == nullptr)
		{
			return;
		}
		else
		{
			std::erase(m_pChildren, pChild);

			if (pChild->GetParent() == this)
			{
				pChild->m_pParent = nullptr;
			}
		}
	}

	void GameObject::CleanupComponents()
	{
		std::erase_if(m_pComponents,
			[](const auto& component)
			{
				return component->IsPendingDeletion();
			});
	}
}
