#include "game.h"

// ###############################
// ### TODO const-correctness! ###
// ### TODO updatable class?   ###
// ### TODO style clean up     ###
// ### TODO #include clean up  ###
// ###############################

int main(int argc, char** argv)
{
	jw::game* gameInstance = jw::game::make_game();
	return gameInstance->run();
}