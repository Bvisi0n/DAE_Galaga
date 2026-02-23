#include <numbers>

#include "GameObject.h"
#include "RotatorComponent.h"

dae::RotatorComponent::RotatorComponent(GameObject* pOwner, float range, float speed)
    : BaseComponent(pOwner), m_Range(range), m_RotationSpeed(speed) {}

void dae::RotatorComponent::Update(float deltaTime)
{
    m_Angle += m_RotationSpeed * deltaTime;

    const float double_pi{ std::numbers::pi_v<float> *2.0f };

    if (m_Angle > double_pi)
    {
        m_Angle -= double_pi;
    }

    // x = cos(a) * r, y = sin(a) * r
    glm::vec3 local_offset{ cos(m_Angle) * m_Range, sin(m_Angle) * m_Range, 0.0f };

    GetOwner()->SetLocalPosition(local_offset.x, local_offset.y);
}