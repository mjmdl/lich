#include <spdlog/sinks/stdout_color_sinks.h>

#include "log.hpp"

namespace lich {

Logger Logger::engine_logger{"Lich"};
Logger Logger::client_logger{"Game"};

inline static spdlog::level::level_enum to_spdlog_level_(Log_Level level) {
	return static_cast<spdlog::level::level_enum>(level);
}

inline static Log_Level to_our_level_(spdlog::level::level_enum level) {
	return static_cast<Log_Level>(level);
}

Logger::Logger(const std::string &name, Log_Level level) {
	auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	_logger = std::make_unique<spdlog::logger>(name, sink);
	_logger->set_pattern("%^%T | %l [%n]: %v%$");
	_logger->set_level(to_spdlog_level_(level));
}

const std::string &Logger::name(void) const {
	return _logger->name();
}

Log_Level Logger::level(void) const {
	return to_our_level_(_logger->level());
}

void Logger::set_level(Log_Level level) {
	_logger->set_level(to_spdlog_level_(level));
}

}
