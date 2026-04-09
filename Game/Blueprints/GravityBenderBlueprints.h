#ifndef GRAVITYBENDERBLUEPRINTS_H
#define GRAVITYBENDERBLUEPRINTS_H

#include <string>

#include<glm/ext/vector_float2.hpp>

namespace bvi::blueprints
{
	struct ZakoData
	{
		std::string filename{ "zako.png" };
		glm::vec2 initialVelocity{ 0.0f, 150.0f };
		float spawnDelay{ 0.2f };
		int spawnCount{ 10 };
	};
}
#endif
