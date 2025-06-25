#ifndef SAND_GAME_HPP
#define SAND_GAME_HPP

#include <lich/app.hpp>

namespace sand {

class Game final : public lich::App {
public:
	Game(const lich::Console_Args &console_args);
	virtual ~Game(void);
};

}

#endif
