#ifndef BVI_GRAVITYBENDERBLUEPRINTS_H
#define BVI_GRAVITYBENDERBLUEPRINTS_H

#include <string>

// Inspired by Type Object.

// TODO bvi_gravity_bender - Move all tweakable properties to this file.

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
