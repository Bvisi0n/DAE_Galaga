#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include <glm/glm.hpp>

#include "Components/Component.h"

namespace dae
{
    class GameObject;

    // Allows a GameObject to rotate around a parent in a circular path.
    class RotatorComponent final : public Component
    {
    public:
        RotatorComponent(GameObject* pOwner, float range, float speed);
        ~RotatorComponent() override = default;

        void Update(const float deltaTime) override;

    private:
        float m_angle{ 0.0f };
        float m_range{ 0.0f };
        float m_rotationSpeed{ 0.0f };
    };
}
#endif
