#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>

#include "Transform.h"

namespace dae
{
	class Texture2D;
	class GameObject 
	{
	public:
		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other)			   = delete;
		GameObject(GameObject&& other)				   = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other)	   = delete;

		virtual void FixedUpdate([[maybe_unused]] const float deltaTime);
		virtual void Update([[maybe_unused]] const float deltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

	private:
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
	};
}
#endif
