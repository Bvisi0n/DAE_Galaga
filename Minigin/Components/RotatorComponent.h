#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include <glm/glm.hpp>

#include "BaseComponent.h"

namespace dae
{
    class RotatorComponent final : public BaseComponent
    {
    public:
        RotatorComponent(GameObject* pOwner, float range, float speed);
        virtual ~RotatorComponent() override = default;

        virtual void Update(float deltaTime) override;

    private:
        float m_Angle{ 0.0f };
        float m_Range{ 0.0f }; // Radius of the circle
        float m_RotationSpeed{ 0.0f }; // Radians or Degrees per second
    };
}

#endif
