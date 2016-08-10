#pragma once

#include <string>
#include <set>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include "../lib/json-master/src/json.hpp"
#include "graph.h"
#include "location.h"
#include "road.h"
#include "gameObject.h"
#include "pathEngine.h"

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

		static graph_type loadWorld(string filepath);
		static graph_type loadWorld(nlohmann::json mapJson);

		void update(sf::Time timeSinceLastFrame);	// TODO UT?
		void draw(sf::RenderWindow& renderTarget);	// TODO UT?

		void attachToGraph(pathEngine* pather);

	private:
		graph<location, road> worldGraph;
		set<gameObject*> worldObjects;
	};
}