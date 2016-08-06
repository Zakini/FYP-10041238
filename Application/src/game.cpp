#include "game.h"

namespace jw
{
	game::game()
		: gameWindow(sf::VideoMode(800, 600), "FYP - 10041238", sf::Style::Close | sf::Style::Titlebar)
		, gameWorld("C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/map.json")
		, pather()
		, entities()
		, frameTimer()
	{
		string carsJsonPath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/entities/entities.json";
		gameWorld.attachToGraph(&pather);
		auto cars = car::loadCars(carsJsonPath, &pather);
		entities.assign(cars.begin(), cars.end());
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

			// TODO on resize, resize view?

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