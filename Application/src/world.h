#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "graph.h"
#include "location.h"
#include "road.h"

using std::string;

namespace jw
{
	class world
	{
	public:
		world() : worldGraph() {}
		world(string filename);

		void update();
		void draw(sf::RenderWindow& targetWindow);

	private:
		graph<location, road> worldGraph;
	};
}