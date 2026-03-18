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

		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalPosition(float x, float y, float z = 0.f);
		const glm::vec3& GetLocalPosition() const;

		void SetLocalRotation(float angleInRadians);
		float GetLocalRotation() const;

		void SetLocalScale(const glm::vec3& scale);
		void SetLocalScale(float x, float y, float z = 1.f);
		const glm::vec3& GetLocalScale() const;

		const glm::mat4& GetWorldMatrix() const;
		const glm::vec3& GetWorldPosition();

		void UpdateWorldMatrix(const glm::mat4& parentWorldMatrix);
		void SetDirty();

	private:
		glm::mat4 m_localMatrix;
		glm::mat4 m_worldMatrix;
		glm::vec3 m_worldPosition;

		glm::vec3 m_localPosition;
		glm::vec3 m_localScale;
		float m_localRotation;

		bool m_isDirty{ true };

		void UpdateLocalMatrix();
	};
}
#endif
