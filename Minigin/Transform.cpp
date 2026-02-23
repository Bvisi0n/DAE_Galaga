#include "Transform.h"

dae::Transform::Transform(glm::vec3 pos)
	: m_position(pos) {}

dae::Transform dae::Transform::operator+(const Transform& other) const
{
	return Transform{ m_position + other.m_position };
}

dae::Transform dae::Transform::operator-(const Transform& other) const
{
	return Transform{ m_position - other.m_position };
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& position)
{ 
	m_position = position; 
}