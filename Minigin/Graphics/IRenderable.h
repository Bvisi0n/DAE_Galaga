#ifndef IRENDERABLE_H
#define IRENDERABLE_H

namespace dae::graphics
{
	class IRenderable
	{
	public:
		IRenderable() = default;
		virtual ~IRenderable() = default;

		IRenderable( const IRenderable& ) = delete;
		IRenderable( IRenderable&& ) = delete;
		IRenderable& operator=( const IRenderable& ) = delete;
		IRenderable& operator=( IRenderable&& ) = delete;

		virtual void Render() const = 0;
	};
}
#endif
