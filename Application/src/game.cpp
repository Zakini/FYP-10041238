#include "game.h"

using std::make_shared;

const string worldJsonPath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/map.json";
const string carsJsonPath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/entities/entities.json";

namespace jw
{
	game::game(shared_ptr<world::graph_type> gameWorldGraph)
		: gameWindow(sf::VideoMode(800, 600), "FYP - 10041238", sf::Style::Close | sf::Style::Titlebar)
		, gameWorld(gameWorldGraph)
		, pather(make_shared<pathEngine>(gameWorldGraph))
		, entities()
		, frameTimer()
	{
		auto cars = car::loadCars(carsJsonPath, pather);
		entities.assign(cars.begin(), cars.end());
	}

	game* game::make_game()
	{
		shared_ptr<world::graph_type> gameWorldGraph(world::loadWorld(worldJsonPath));
		return new game(gameWorldGraph);
	}

	int game::run()
	{
		frameTimer.restart();

		while (gameWindow.isOpen())
		{
			update(frameTimer.getElapsedTime());
			draw();

			frameTimer.restart();
		}

		return 0;
	}

	void game::update(sf::Time timeSinceLastFrame)
	{
		sf::Event event;

		// Window events
		while (gameWindow.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				gameWindow.close();
				break;

			// POSSIBLE on resize, resize view?

			default:
				break;
			}
		}

		// World and entities
		gameWorld.update(timeSinceLastFrame);
		
		for (auto entity : entities)
		{
			entity->update(timeSinceLastFrame);
		}
	}

	void game::draw()
	{
		gameWindow.clear();

		gameWorld.draw(gameWindow);

		for (auto entity : entities)
		{
			entity->draw(gameWindow);
		}

		gameWindow.display();
	}
}