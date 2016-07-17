#pragma once

#include <SFML/Graphics.hpp>
#include "world.h"

namespace jw
{
	class game
	{
	public:
		game() :
			gameWindow(sf::VideoMode(800, 600), "FYP - 10041238", sf::Style::Close | sf::Style::Titlebar),
			gameWorld("C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/map.json")
			{}

		int run();

	private:
		sf::RenderWindow gameWindow;
		world gameWorld;

		void update();
		void draw();
	};
}