#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

#include "Components/Component.h"

namespace dae
{
    class TransformComponent final : public Component
	{
		// DAEN: Encapsulate all transform logic into the Transform class, refactor it to use matrices.
	public:
		TransformComponent(GameObject* pOwner, float xPos, float yPos, float xScale = 1.f, float yScale = 1.f);

		void Update(float) override {}
		void Render() const override {}

		const glm::mat4& GetTransformMatrix() const;

		void SetPosition(float x, float y, float z = 0.f);
		const glm::vec3& GetPosition() const;

		void SetScale(float x, float y, float z = 1.f);
		const glm::vec3& GetScale() const;

	private:
		void UpdateMatrix();

		glm::mat4 m_matrix;
		glm::vec3 m_position;
		glm::vec3 m_scale;
	};
}
#endif
