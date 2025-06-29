#include "game.hpp"

int main(int argc, char **argv)
{
	sand::Game game{lich::ConsoleArgs{argc, argv}};
	return game.run();
}
