#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <glm/glm.hpp>

#include "Commands/Command.h"
#include "GameObject.h"

class MoveCommand final : public dae::Command
{
public:
    MoveCommand(dae::GameObject* pGameObject, const glm::vec2& direction, float speed)
        : m_pGameObject(pGameObject)
        , m_direction(direction)
        , m_speed(speed) {}

    void Execute() override
    {
        auto position = m_pGameObject->GetLocalPosition().GetPosition();

        const float dt = 0.016f; // TODO 1: Use deltaTime here!
        position.x += m_direction.x * m_speed * dt;
        position.y += m_direction.y * m_speed * dt;

        m_pGameObject->SetLocalPosition(position.x, position.y);
    }

private:
    dae::GameObject* m_pGameObject;
    glm::vec2 m_direction;
    float m_speed;
};

#endif