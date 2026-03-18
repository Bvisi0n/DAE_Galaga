#include <string>

#include "Components/TransformComponent.h"
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(const float deltaTime)
{
	// DAEN: GameObjects should not update when marked for deletion.
	// DAEN: If a parent gets deleted, then all kids should be deleted as well.
	for (auto& comp : m_pComponents)
	{
		comp->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (const auto& comp : m_pComponents)
	{
		comp->Render();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_pParent == parent)
	{
		return;
	}

	if (parent == nullptr)
	{
		// DAEH: Fix me! (Transform)
		//SetLocalPosition(GetGlobalPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			// DAEH: Fix me! (Transform)
			//SetLocalPosition(GetGlobalPosition() - parent->GetGlobalPosition());
		}
		// DAEH: Fix me! (Transform)
		//SetPositionDirty();
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

const std::vector<dae::GameObject*>& dae::GameObject::GetChildren() const
{
	return m_pChildren;
}

// DAEH: Fix me! (Transform)
//void dae::GameObject::SetLocalPosition(float x, float y)
//{
//	m_localPosition.SetPosition(x, y, 0.0f);
//	SetPositionDirty();
//}

// DAEH: Fix me! (Transform)
//void dae::GameObject::SetLocalPosition(TransformComponent pos)
//{
//	m_localPosition = pos;
//	SetPositionDirty();
//}

// DAEH: Fix me! (Transform)
//dae::TransformComponent& dae::GameObject::GetGlobalPosition()
//{
//	if (m_positionIsDirty)
//	{
//		UpdateWorldPosition();
//	}
//
//	return m_globalPosition;
//}

// DAEH: Fix me! (Transform)
//dae::TransformComponent& dae::GameObject::GetLocalPosition()
//{
//	return m_localPosition;
//}

bool dae::GameObject::IsChild(GameObject* candidate)
{
	if (candidate == nullptr) return false;

	for (auto child : m_pChildren)
	{
		if (child == candidate) return true;
		if (child->IsChild(candidate)) return true;
	}
	return false;
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild), m_pChildren.end());
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.emplace_back(pChild);
}

// DAEH: Fix me! (Transform)
//void dae::GameObject::UpdateWorldPosition()
//{
//	if (m_positionIsDirty)
//	{
//		if (m_pParent == nullptr)
//		{
//			m_globalPosition = m_localPosition;
//		}
//		else
//		{
//			m_globalPosition = m_pParent->GetGlobalPosition() + m_localPosition;
//		}
//	}
//
//	m_positionIsDirty = false;
//}

// DAEH: Fix me! (Transform)
//void dae::GameObject::SetPositionDirty()
//{
//	m_positionIsDirty = true;
//	for (const auto& child : m_pChildren)
//	{
//		child->SetPositionDirty();
//	}
//}
