#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

namespace dae
{
	class TransformComponent final
	{
	public:
		TransformComponent() = default;
		TransformComponent(const glm::vec3& position);
		TransformComponent operator+(const TransformComponent& other) const;
		TransformComponent operator-(const TransformComponent& other) const;

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(const float x, const float y, const float z = 0);
		void SetPosition(const glm::vec3& position);

	private:
		glm::vec3 m_position;
	};
}
#endif
