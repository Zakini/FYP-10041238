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
#include <memory>

using std::string;
using std::set;
using std::shared_ptr;

namespace jw
{
	class world
	{
	public:
		using graph_type = graph<location, road>;

		world(shared_ptr<graph_type> p_worldGraph);

		static graph_type* loadWorld(string filepath);
		static graph_type* loadWorld(nlohmann::json mapJson);

		void update(sf::Time timeSinceLastFrame);	// POSSIBLE UT?
		void draw(sf::RenderWindow& renderTarget);	// POSSIBLE UT?

	private:
		shared_ptr<graph_type> worldGraph;
		set<gameObject*> worldObjects;
	};
}