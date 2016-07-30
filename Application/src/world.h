#pragma once

#include <string>
#include <set>
#include <SFML/Graphics.hpp>
#include "../lib/json-master/src/json.hpp"
#include "graph.h"
#include "location.h"
#include "road.h"
#include "gameObject.h"

using std::string;
using std::set;

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
		void draw(sf::RenderWindow& renderTarget);	// TODO UT?

	private:
		graph<location, road> worldGraph;
		set<gameObject*> gameObjects;
	};
}