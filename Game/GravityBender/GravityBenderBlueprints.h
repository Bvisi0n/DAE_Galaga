#ifndef BVI_GRAVITYBENDERBLUEPRINTS_H
#define BVI_GRAVITYBENDERBLUEPRINTS_H

#include <cstdint>

// Inspired by Type Object.
namespace bvi::gravity_bender::config
{
	struct CoreColor
	{
		float r{ 1.F };
		float g{ 1.F };
		float b{ 1.F };
		float a{ 1.F };
	};

	struct GravityFieldBlueprint
	{
		float durationSeconds{ 6.F };
		float cooldownSeconds{ 2.F };

		float effectRadius{ 100.F };
		float pullForce{ 2'500'000.F };

		float coreSize{ 10.F };
		CoreColor color{ .r = 0.F, .g = 1.F, .b = 1.F, .a = 1.F };
	};

	struct PlayerBlueprint
	{
		float startX{ 400.F };
		float startY{ 350.F };
		float radius{ 12.F };

		float baseSpeed{ 250.F };
		float speedMultiplier{ 2.F };
		float inputForce{ 400.F };

		float gravityRadius{ 150.F };
		float gravityStrength{ 1'500'000.F };

		CoreColor color{ .r = 1.F, .g = 204.F / 255.F, .b = 0.F, .a = 1.F };
	};

	struct PortalBlueprint
	{
		float portalSize{ 20.F };
		uint32_t portalLineThickness{ 3 };
		CoreColor portalColor{ .r = 1.F, .g = 50.F / 255.F, .b = 50.F / 255.F, .a = 1.F };

		float spawnDelay{ 0.15F };
		float cooldownSeconds{ 15.F };
		float anticipationSeconds{ 3.F };
		uint32_t spawnCount{ 10 };

		float unitSpeed{ 200.F };
		float unitSize{ 5.F };
		CoreColor unitColor{ .r = 1.F, .g = 0.F, .b = 0.F, .a = 1.F };
	};

	struct UIConfig
	{
		float fpsStartX{ 10.F };
		float fpsStartY{ 10.F };
		uint8_t fpsFontSize{ 18 };
		CoreColor fpsColor{ .r = 150.F / 255.F, .g = 200.F / 255.F, .b = 150.F / 255.F, .a = 1.F };

		float instructionsStartY{ 10.F };
		float instructionsLineSpacing{ 20.F };
		uint8_t instructionsFontSize{ 16 };
		CoreColor instructionsColor{ .r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F };
	};

	struct ViewportConfig
	{
		float width{ 1024.F };
		float height{ 576.F };

		uint32_t borderThickness{ 2 };
		CoreColor borderColor{ .r = 138.F / 255.F, .g = 43.F / 255.F, .b = 226.F / 255.F, .a = 1.F };

		CoreColor backgroundColor{ .r = 20.F / 255.F, .g = 10.F / 255.F, .b = 30.F / 255.F, .a = 1.F };
	};

	inline constexpr struct GlobalConfig
	{
		GravityFieldBlueprint gravityField{};
		PlayerBlueprint player{};
		PortalBlueprint portal{};
		ViewportConfig viewport{};
		UIConfig ui{};
	} c_GlobalConfig;
}
#endif
