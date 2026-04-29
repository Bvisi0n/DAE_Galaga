#ifndef BVI_SPAWNGRAVITYFIELDCOMMAND_H
#define BVI_SPAWNGRAVITYFIELDCOMMAND_H

#include <chrono>

#include <Minigin/Input/ICommand.h>

namespace dae::core
{
	class GameObject;
}

namespace bvi::gravity_bender
{
	class SpawnGravityFieldCommand final : public dae::input::ICommand
	{
	public:
		explicit SpawnGravityFieldCommand( const dae::core::GameObject* contextPlayer );

		void Execute( const float /*deltaTime*/ ) override;

		[[nodiscard]] const void* GetTargetContext() const override;

	private:
		const dae::core::GameObject* m_playerContext;
		std::chrono::steady_clock::time_point m_lastSpawnTime{};
	};
}
#endif
