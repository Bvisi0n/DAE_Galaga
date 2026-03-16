#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <glm/glm.hpp>

#include "Commands/Command.h"
#include "GameObject.h"

// DAEN: Split into .h & .cpp.

namespace dae
{
    class MoveCommand final : public dae::Command
    {
    public:
        MoveCommand(dae::GameObject* pGameObject, const glm::vec2& direction, float speed)
            : m_pGameObject(pGameObject)
            , m_direction(direction)
            , m_speed(speed) {}

        void Execute(const float deltaTime) override
        {
            // DAEN: Diagonal movement is currently double speed.
            auto position = m_pGameObject->GetLocalPosition().GetPosition();

            position.x += m_direction.x * m_speed * deltaTime;
            position.y += m_direction.y * m_speed * deltaTime;

            m_pGameObject->SetLocalPosition(position.x, position.y);
        }

    private:
        dae::GameObject* m_pGameObject;
        glm::vec2 m_direction;
        float m_speed;
    };
}
#endif
