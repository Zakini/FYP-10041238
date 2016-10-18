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
		, cDetector(make_shared<collisionDetector>())
		, entities()
		, frameTimer()
		, displayFont()
		, timerDisplay()
	{
		auto cars = car::loadCars(carsJsonPath, pather, cDetector);
		entities.assign(cars.begin(), cars.end());

		for (collidable* car : cars)
		{
			cDetector->addCollisionTarget(car);
		}

		displayFont.loadFromFile("C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/resources/fonts/arial.ttf");
		timerDisplay.setFont(displayFont);
		timerDisplay.setColor(sf::Color::Blue);
		timerDisplay.setCharacterSize(20);
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
			update(frameTimer.restart());
			draw();
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

		int currentFps = round(1 / timeSinceLastFrame.asSeconds());
		timerDisplay.setString(std::to_string(currentFps) + " FPS");
	}

	void game::draw()
	{
		gameWindow.clear();

		gameWindow.draw(gameWorld);

		for (auto entity : entities)
		{
			gameWindow.draw(*entity);
		}

		gameWindow.draw(timerDisplay);

		gameWindow.display();
	}
}