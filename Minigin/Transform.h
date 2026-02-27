#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform() = default;
		Transform(const glm::vec3& position);
		Transform operator+(const Transform& other) const;
		Transform operator-(const Transform& other) const;

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(const float x, const float y, const float z = 0);
		void SetPosition(const glm::vec3& position);

	private:
		glm::vec3 m_position;
	};
}
#endif
