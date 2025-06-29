#ifndef LICH_LOG_HPP
#define LICH_LOG_HPP

#include <spdlog/logger.h>

namespace lich {

enum class LogLevel {
	Off = spdlog::level::off,
	Trace = spdlog::level::trace,
	Debug = spdlog::level::debug,
	Info = spdlog::level::info,
	Warn = spdlog::level::warn,
	Error = spdlog::level::err,
	Fatal = spdlog::level::critical,
};

#define GEN_MEMBER_FUNCTION(INT, IMPL) \
	template<typename ...Args> \
	void INT(spdlog::format_string_t<Args...> format, Args &&...args) \
	{ \
		_logger->IMPL(std::move(format), std::forward<Args>(args)...); \
	}

class Logger {
public:
	static Logger engine_logger;
	static Logger client_logger;

	Logger(const std::string &name = "Logger", LogLevel level = LogLevel::Trace);
	const std::string &name() const;
	LogLevel level() const;
	void set_level(LogLevel level);

	GEN_MEMBER_FUNCTION(trace, trace)
	GEN_MEMBER_FUNCTION(debug, debug)
	GEN_MEMBER_FUNCTION(info, info)
	GEN_MEMBER_FUNCTION(warn, warn)
	GEN_MEMBER_FUNCTION(error, error)
	GEN_MEMBER_FUNCTION(fatal, critical)
	
private:
	std::unique_ptr<spdlog::logger> _logger;
};

#ifdef LICH_COMPILE_STEP
#   define SELECTED_LOGGER Logger::engine_logger
#else
#   define SELECTED_LOGGER Logger::client_logger
#endif

#define GEN_FUNCTION(NAME) \
	template<typename ...Args> \
	inline void log_##NAME(spdlog::format_string_t<Args...> format, Args &&...args) \
	{ \
		SELECTED_LOGGER.NAME(std::move(format), std::forward<Args>(args)...); \
	}

GEN_FUNCTION(trace)
GEN_FUNCTION(debug)
GEN_FUNCTION(info)
GEN_FUNCTION(warn)
GEN_FUNCTION(error)
GEN_FUNCTION(fatal)

#define LICH_ASSERT(EXPRESSION, ...) \
	do { \
		if (not (EXPRESSION)) { \
			lich::log_fatal("Assertion failure at {}:{}: {}", \
				__FILE__, __LINE__, #EXPRESSION); \
			lich::log_fatal(__VA_ARGS__); \
			std::exit(EXIT_FAILURE); \
		} \
	} while (0)
#define LICH_EXPECT(EXPRESSION, ...) \
	do { \
		if (not (EXPRESSION)) { \
			lich::log_error("Unexpected failure at {}:{}: {}", \
				__FILE__, __LINE__, #EXPRESSION); \
			lich::log_error(__VA_ARGS__); \
		} \
	} while (0)

#undef GEN_FUNCTION
#undef SELECTED_LOGGER
#undef GEN_MEMBER_FUNCTION

}

#endif
