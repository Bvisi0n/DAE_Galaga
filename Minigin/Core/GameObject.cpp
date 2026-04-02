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
	
	void GameObject::Update(const float deltaTime)
	{
		// TODO N: GameObjects should not update when marked for deletion.
		for (auto& comp : m_pComponents)
		{
			if (!comp->IsPendingDeletion())
			{
				comp->Update(deltaTime);
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

		if (parent == nullptr)
		{
			m_transform.SetLocalPosition(m_transform.GetWorldPosition());
		}
		else
		{
			if (keepWorldPosition)
			{
				m_transform.SetLocalPosition(m_transform.GetWorldPosition() - parent->GetTransform().GetWorldPosition());
			}
		}

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

	void GameObject::RemoveChild(GameObject* pChild)
	{
		m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild), m_pChildren.end());
	}

	void GameObject::AddChild(GameObject* pChild)
	{
		m_pChildren.emplace_back(pChild);
	}
	void GameObject::CleanupComponents()
	{
		std::erase_if(m_pComponents, [](const auto& component){ return component->IsPendingDeletion(); });
	}
}
