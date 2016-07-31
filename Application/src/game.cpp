#include "game.h"

namespace jw
{
	int game::run()
	{
		frameTimer.restart();

		while (gameWindow.isOpen())
		{
			update(frameTimer.getElapsedTime());
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

			// TODO on resize, resize view?

			default:
				break;
			}
		}

		// World and entities
		gameWorld.update(timeSinceLastFrame);
		// TODO
	}

	void game::draw()
	{
		gameWindow.clear();

		gameWorld.draw(gameWindow);

		gameWindow.display();
	}
}