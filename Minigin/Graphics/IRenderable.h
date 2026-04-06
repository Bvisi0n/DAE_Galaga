#ifndef IRENDERABLE_H
#define IRENDERABLE_H

namespace dae::graphics
{
	class IRenderable
	{
	public:
		virtual ~IRenderable()
		{};

		virtual void Render() const = 0;
	};
}
#endif
