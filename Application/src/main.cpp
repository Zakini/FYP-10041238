#include "game.h"

// ###############################
// ### TODO const-correctness! ###
// ### TODO updatable class?   ###
// ###############################

int main(int argc, char** argv)
{
	jw::game* gameInstance = jw::game::make_game();
	return gameInstance->run();
}