#ifndef BVI_GRAVITYBENDERBLUEPRINTS_H
#define BVI_GRAVITYBENDERBLUEPRINTS_H

#include <cstdint>

// Inspired by Type Object.

namespace bvi::gravity_bender::config
{
	struct CoreColor
	{
		uint8_t r{ 255 };
		uint8_t g{ 255 };
		uint8_t b{ 255 };
		uint8_t a{ 255 };
	};

	struct GravityFieldBlueprint
	{
		float durationSeconds{ 3.F };
		float cooldownSeconds{ 1.F };

		float effectRadius{ 250.F };
		float pullForce{ 1'500'000.F };
	};

	struct PlayerBlueprint
	{
		float startX{ 400.F };
		float startY{ 350.F };
		float radius{ 10.F };
		float colliderSize{ 5.F };

		float baseSpeed{ 250.F };
		float speedMultiplier{ 2.F };
		float inputForce{ 400.F };

		float gravityRadius{ 100.F };
		float gravityStrength{ 1'500'000.F };

		CoreColor color{ .r = 255, .g = 204, .b = 0, .a = 255 };
	};

	struct PortalBlueprint
	{
		float portalSize{ 20.F };
		uint32_t portalLineThickness{ 3 };
		CoreColor portalColor{ .r = 255, .g = 120, .b = 0, .a = 255 };

		float spawnDelay{ 0.15F };
		float cooldownSeconds{ 15.F };
		float anticipationSeconds{ 3.F };
		uint32_t spawnCount{ 10 };

		float unitSpeed{ 200.F };
		float unitSize{ 5.F };
		CoreColor unitColor{ .r = 255, .g = 50, .b = 50, .a = 255 };
	};

	struct UIConfig
	{
		float fpsStartX{ 10.F };
		float fpsStartY{ 10.F };
		uint8_t fpsFontSize{ 18 };
		CoreColor fpsColor{ .r = 150, .g = 200, .b = 150, .a = 255 };

		float instructionsStartY{ 10.F };
		float instructionsLineSpacing{ 20.F };
		uint8_t instructionsFontSize{ 16 };
		CoreColor instructionsColor{ .r = 255, .g = 255, .b = 255, .a = 255 };
	};

	struct ViewportConfig
	{
		float width{ 1024.F };
		float height{ 576.F };

		uint32_t borderThickness{ 2 };
		CoreColor borderColor{ .r = 138, .g = 43, .b = 226, .a = 255 };

		CoreColor backgroundColor{ .r = 20, .g = 10, .b = 30, .a = 255 };
	};

	inline constexpr struct GlobalConfig
	{
		GravityFieldBlueprint gravityField{};
		PlayerBlueprint player{};
		PortalBlueprint portal{};
		ViewportConfig viewport{};
		UIConfig ui{};
	} Config;
}
#endif
