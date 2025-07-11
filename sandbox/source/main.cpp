#include "game.hpp"

int main(int argc, char **argv) {
	sand::Game game{lich::Console_Args{argc, argv}};
	return game.run();
}
