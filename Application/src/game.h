#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include "world.h"
#include "gameObject.h"
#include "car.h"
#include "pathEngine.h"
#include <memory>

using std::vector;
using std::shared_ptr;

namespace jw
{
	class game
	{
	public:
		game(shared_ptr<world::graph_type> gameWorldGraph);
		static game* make_game();

		int run();	// POSSIBLE UT?

	private:
		sf::RenderWindow gameWindow;
		world gameWorld;
		shared_ptr<pathEngine> pather;
		vector<gameObject*> entities;
		sf::Clock frameTimer;

		void update(sf::Time timeSinceLastFrame);
		void draw();
	};
}