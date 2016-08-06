#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include "world.h"
#include "gameObject.h"
#include "car.h"
#include "pathEngine.h"

using std::vector;

namespace jw
{
	class game
	{
	public:
		game();

		int run();	// TODO UT?

	private:
		sf::RenderWindow gameWindow;
		world gameWorld;
		pathEngine pather;
		vector<gameObject*> entities;
		sf::Clock frameTimer;

		void update(sf::Time timeSinceLastFrame);
		void draw();
	};
}