#ifndef GRAVITYBENDERBLUEPRINTS_H
#define GRAVITYBENDERBLUEPRINTS_H

#include <string>

#include <glm/vec2.hpp>

// Inspired by Type Object.

// TODO GRAV: Move all tweakable properties to this file.

namespace bvi::gravity_bender
{
	struct UnitData
	{
		std::string filename{ "zako.png" };
		float speed{ 200.0f };
		float spawnDelay{ 0.15f };
		int spawnCount{ 10 };
	};
}
#endif
