#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

namespace dae
{
	class TransformComponent final
	{
	public:
		TransformComponent() = default;
		TransformComponent(glm::vec3 pos);
		TransformComponent operator+(const TransformComponent& other) const;
		TransformComponent operator-(const TransformComponent& other) const;

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);

	private:
		glm::vec3 m_position;
	};
}
#endif
