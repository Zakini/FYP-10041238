#include "world.h"

#include <fstream>

using std::ifstream;

namespace jw
{
	world::world(string filepath)
	{
		worldGraph = loadGraph(filepath);
	}

	world::graph_type world::loadGraph(string filepath)
	{
		ifstream mapFile(filepath);
		nlohmann::json mapJson;

		mapFile >> mapJson;

		return loadGraph(mapJson);
	}

	world::graph_type world::loadGraph(nlohmann::json mapJson)
	{
		// TODO
		return world::graph_type();
	}

	void world::update()
	{
	}

	void world::draw(sf::RenderWindow& targetWindow)
	{
	}
}