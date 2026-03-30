#include <string>

#include "Components/TransformComponent.h"
#include "Components/IRenderable.h"
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"

namespace dae
{
	void GameObject::Update(const float deltaTime)
	{
		// TODO N: GameObjects should not update when marked for deletion.
		// TODO N: If a parent gets deleted, then all kids should be deleted as well.
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

		// TODO N: What if there is no TransformComponent?
		if (parent == nullptr)
		{
			GetComponent<TransformComponent>()->SetLocalPosition(GetComponent<TransformComponent>()->GetWorldPosition());
		}
		else
		{
			if (keepWorldPosition)
			{
				GetComponent<TransformComponent>()->SetLocalPosition(GetComponent<TransformComponent>()->GetWorldPosition() - parent->GetComponent<TransformComponent>()->GetWorldPosition());
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
