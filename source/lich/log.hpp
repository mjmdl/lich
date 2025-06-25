#ifndef LICH_LOG_HPP
#define LICH_LOG_HPP

#include <spdlog/logger.h>

namespace lich {

enum class Log_Level {
	Off = spdlog::level::off,
	Trace = spdlog::level::trace,
	Debug = spdlog::level::debug,
	Info = spdlog::level::info,
	Warn = spdlog::level::warn,
	Error = spdlog::level::err,
	Fatal = spdlog::level::critical,
};

#define GEN_METHOD(INT, IMPL) \
	template<typename ...Args> \
	void INT(spdlog::format_string_t<Args...> format, Args &&...args) { \
		_logger->IMPL(std::move(format), std::forward<Args>(args)...); \
	}

class Logger {
public:
	Logger(const std::string &name = "Logger",
		Log_Level level = Log_Level::Trace);
	const std::string &name(void) const;
	Log_Level level(void) const;
	void set_level(Log_Level level);

	GEN_METHOD(trace, trace)
	GEN_METHOD(debug, debug)
	GEN_METHOD(info, info)
	GEN_METHOD(warn, warn)
	GEN_METHOD(error, error)
	GEN_METHOD(fatal, critical)
	
private:
	std::unique_ptr<spdlog::logger> _logger;
};

#undef GEN_METHOD

}

#endif
