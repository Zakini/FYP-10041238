#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "../lib/json-master/src/json.hpp"
#include "graph.h"
#include "location.h"
#include "road.h"

using std::string;

namespace jw
{
	class world
	{
	public:
		using graph_type = graph<location, road>;

		world() : worldGraph() {}
		world(string filepath);

		static graph_type loadGraph(string filepath);
		static graph_type loadGraph(nlohmann::json mapJson);

		void update();	// TODO UT?
		void draw(sf::RenderWindow& targetWindow);	// TODO UT?

	private:
		graph<location, road> worldGraph;
	};
}