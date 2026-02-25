#include <numbers>

#include "GameObject.h"
#include "RotatorComponent.h"

dae::RotatorComponent::RotatorComponent(GameObject* pOwner, float range, float speed)
    : BaseComponent(pOwner), m_Range(range), m_RotationSpeed(speed) {}

void dae::RotatorComponent::Update(float deltaTime)
{
    m_Angle += m_RotationSpeed * deltaTime;

    // x = cos(a) * r, y = sin(a) * r
    GetOwner()->SetLocalPosition(cos(m_Angle) * m_Range, sin(m_Angle) * m_Range);
}