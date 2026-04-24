#ifndef DAE_TRANSFORMCOMPONENT_H
#define DAE_TRANSFORMCOMPONENT_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace dae::core
{
	class GameObject;

	// TODO dae_core - Use a struct to bundle ctor parameters together.

	class Transform final
	{
	public:
		Transform( GameObject* owner, float xPos, float yPos, float xScale = 1.f, float yScale = 1.f );

		void SetLocalPosition( const glm::vec3& pos );
		void SetLocalPosition( float x, float y, float z = 0.f );
		[[nodiscard]] const glm::vec3& GetLocalPosition() const;

		void SetLocalRotation( float angleInRadians );
		[[nodiscard]] float GetLocalRotation() const;

		void SetLocalScale( const glm::vec3& scale );
		void SetLocalScale( float x, float y, float z = 1.f );
		[[nodiscard]] const glm::vec3& GetLocalScale() const;

		[[nodiscard]] const glm::mat4& GetWorldMatrix() const;
		[[nodiscard]] const glm::vec3& GetWorldPosition() const;

		void UpdateWorldMatrix( const glm::mat4& parentWorldMatrix );
		void SetDirty();

	private:
		GameObject* m_owner;

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
