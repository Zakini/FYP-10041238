#include "game.h"

// ###############################
// ### TODO const-correctness! ###
// ###############################

int main(int argc, char** argv)
{
	jw::game* gameInstance = jw::game::make_game();
	return gameInstance->run();
}