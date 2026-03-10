#include <string>

#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(const float deltaTime)
{
	for (auto& comp : m_pComponents) comp->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	for (const auto& comp : m_pComponents) comp->Render();
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_pParent == parent)
	{
		return;
	}

	if (parent == nullptr)
	{
		SetLocalPosition(GetGlobalPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(GetGlobalPosition() - parent->GetGlobalPosition());
		}
		SetPositionDirty();
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

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_localPosition.SetPosition(x, y, 0.0f);
	SetPositionDirty();
}

void dae::GameObject::SetLocalPosition(Transform pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

dae::Transform& dae::GameObject::GetGlobalPosition()
{
	if (m_positionIsDirty)
	{
		UpdateWorldPosition();
	}

	return m_globalPosition;
}

dae::Transform& dae::GameObject::GetLocalPosition()
{
	return m_localPosition;
}

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

void dae::GameObject::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (m_pParent == nullptr)
		{
			m_globalPosition = m_localPosition;
		}
		else
		{
			m_globalPosition = m_pParent->GetGlobalPosition() + m_localPosition;
		}
	}

	m_positionIsDirty = false;
}

void dae::GameObject::SetPositionDirty()
{
	m_positionIsDirty = true;
	for (const auto& child : m_pChildren)
	{
		child->SetPositionDirty();
	}
}
