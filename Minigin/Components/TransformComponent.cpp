#include "Components/TransformComponent.h"

dae::TransformComponent::TransformComponent(const glm::vec3& position)
	: m_position(position) {}

dae::TransformComponent dae::TransformComponent::operator+(const TransformComponent& other) const
{
	return TransformComponent{ m_position + other.m_position };
}

dae::TransformComponent dae::TransformComponent::operator-(const TransformComponent& other) const
{
	return TransformComponent{ m_position - other.m_position };
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{ 
	m_position = position; 
}