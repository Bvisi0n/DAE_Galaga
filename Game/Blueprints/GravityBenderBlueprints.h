#ifndef GRAVITYBENDERBLUEPRINTS_H
#define GRAVITYBENDERBLUEPRINTS_H

#include <string>

#include<glm/ext/vector_float2.hpp>

// Inspired by Type Object.

namespace bvi::blueprints
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
