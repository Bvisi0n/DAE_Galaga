#include <string>

#include "Components/TransformComponent.h"
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"

namespace dae
{
	GameObject::~GameObject() = default;

	void GameObject::Update(const float deltaTime)
	{
		// DAEN: GameObjects should not update when marked for deletion.
		// DAEN: If a parent gets deleted, then all kids should be deleted as well.
		for (auto& comp : m_pComponents)
		{
			comp->Update(deltaTime);
		}
	}

	void GameObject::Render() const
	{
		for (const auto& comp : m_pComponents)
		{
			comp->Render();
		}
	}

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		if (IsChild(parent) || parent == this || m_pParent == parent)
		{
			return;
		}

		// DAEH: What if there is no TransformComponent?
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
}
