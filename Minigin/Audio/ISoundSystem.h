#ifndef DAE_ISOUNDSYSTEM_H
#define DAE_ISOUNDSYSTEM_H

#include <cstdint>
#include <filesystem>

namespace dae::audio
{
	using SoundID = uint16_t;

	struct PlayMessage
	{
		SoundID soundID;
		float volume;
	};

	class ISoundSystem
	{
	public:
		ISoundSystem() = default;
		virtual ~ISoundSystem() = default;

		ISoundSystem( const ISoundSystem& ) = delete;
		ISoundSystem( ISoundSystem&& ) = default;
		ISoundSystem& operator=( const ISoundSystem& ) = delete;
		ISoundSystem& operator=( ISoundSystem&& ) = default;

		virtual void Play( const PlayMessage& message ) = 0;

		virtual void RegisterSound( SoundID soundID, const std::filesystem::path& filepath ) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		NullSoundSystem() = default;
		~NullSoundSystem() override = default;

		NullSoundSystem( const NullSoundSystem& ) = delete;
		NullSoundSystem( NullSoundSystem&& ) = default;
		NullSoundSystem& operator=( const NullSoundSystem& ) = delete;
		NullSoundSystem& operator=( NullSoundSystem&& ) = default;

		void Play( const PlayMessage& /*message*/ ) override
		{}
		void RegisterSound( SoundID /*soundID*/, const std::filesystem::path& /*filepath*/ ) override
		{}
	};
}
#endif
