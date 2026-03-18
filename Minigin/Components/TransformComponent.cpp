#include <glm/gtc/matrix_transform.hpp>

#include "Components/TransformComponent.h"

namespace dae
{
    TransformComponent::TransformComponent(GameObject* pOwner, float xPos, float yPos, float xScale, float yScale)
        : Component(pOwner)
        , m_matrix(1.0f)
        , m_position(xPos, yPos, 0.f)
        , m_scale(xScale, yScale, 1.0f)
    {
        UpdateMatrix();
    }

    const glm::mat4& TransformComponent::GetTransformMatrix() const
    {
        return m_matrix;
    }

    void TransformComponent::SetPosition(float x, float y, float z)
    {
        m_position = { x, y, z };
        UpdateMatrix();
    }

    const glm::vec3& TransformComponent::GetPosition() const
    {
        return m_position;
    }

    void TransformComponent::SetScale(float x, float y, float z)
    {
        m_scale = { x, y, z };
        UpdateMatrix();
    }

    const glm::vec3& TransformComponent::GetScale() const
    {
        return m_scale;
    }

    void TransformComponent::UpdateMatrix()
    {
        m_matrix = glm::translate(glm::mat4(1.0f), m_position);
        m_matrix = glm::scale(m_matrix, m_scale);
    }
}
