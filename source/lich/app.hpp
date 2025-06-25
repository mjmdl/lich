#ifndef LICH_APP_HPP
#define LICH_APP_HPP

namespace lich {

struct App_Spec {
	std::string name = "Lich Engine";
	U32 width = 960;
	U32 height = 540;
};

struct Console_Args {
	int argc = 0;
	char **argv = nullptr;
};

class App {
public:
	App(const App_Spec &app_spec = {}, const Console_Args &console_args = {});
	virtual ~App(void) = default;
	int run(void);

	const App_Spec &app_spec(void) const;
	const Console_Args &console_args(void) const;
	bool success(void) const;
	bool running(void) const;
	
private:
	App_Spec _app_spec;
	Console_Args _console_args;
	bool _success;
	bool _running;
};

}

#endif
