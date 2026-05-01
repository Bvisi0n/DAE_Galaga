#ifndef DAE_TRANSFORMCOMPONENT_H
#define DAE_TRANSFORMCOMPONENT_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace dae::core
{
	class GameObject;

	struct TransformDescriptor
	{
		glm::vec3 localPosition{ 0.F, 0.F, 0.F };
		glm::vec3 localScale{ 1.F, 1.F, 1.F };
		float localRotation{ 0.F };
	};

	class Transform final
	{
	public:
		explicit Transform( GameObject* owner, const TransformDescriptor& descriptor = {} );

		void SetLocalPosition( const glm::vec3& pos );
		void SetLocalPosition( float xPos, float yPos, float zPos = 0.F );
		[[nodiscard]] const glm::vec3& GetLocalPosition() const;

		void SetLocalRotation( float angleInRadians );
		[[nodiscard]] float GetLocalRotation() const;

		void SetLocalScale( const glm::vec3& scale );
		void SetLocalScale( float xScale, float yScale, float zScale = 1.F );
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
