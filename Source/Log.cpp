#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <SimuMori/Log.hpp>

namespace SimuMori {
static auto MainLogger = Logger::Create("SimuMori");

class LogLevelFormatter : public spdlog::custom_flag_formatter {
 public:
	LogLevelFormatter() noexcept : spdlog::custom_flag_formatter() {}
	~LogLevelFormatter() noexcept override = default;

	auto format(const spdlog::details::log_msg& message, const std::tm&, spdlog::memory_buf_t& dest) noexcept
		-> void override {
		switch (message.level) {
			case spdlog::level::trace:
				dest.append(std::string_view("Trace"));
				break;
			case spdlog::level::debug:
				dest.append(std::string_view("Debug"));
				break;
			case spdlog::level::info:
				dest.append(std::string_view("Info"));
				break;
			case spdlog::level::warn:
				dest.append(std::string_view("Warning"));
				break;
			case spdlog::level::err:
				dest.append(std::string_view("Error"));
				break;
			case spdlog::level::critical:
				dest.append(std::string_view("Fatal"));
				break;
			case spdlog::level::off:
				dest.append(std::string_view("Off"));
				break;
			default:
				dest.append(std::string_view("Unknown"));
				break;
		}
	}

	auto clone() const noexcept -> std::unique_ptr<spdlog::custom_flag_formatter> override {
		return spdlog::details::make_unique<LogLevelFormatter>();
	}
};

auto Logger::Create(std::string_view name) noexcept -> Logger {
	auto logger    = Logger();
	auto sink      = spdlog::stdout_color_mt(name.data());
	auto formatter = std::make_unique<spdlog::pattern_formatter>();
	formatter->add_flag<LogLevelFormatter>('L');
	formatter->set_pattern("%^[%Y-%m-%d %T.%e] [%n] [%L]:%$ %v");
	sink->set_level(spdlog::level::trace);
	sink->set_formatter(std::move(formatter));

	logger._logger = std::move(sink);

	return logger;
}

auto GetMainLogger() noexcept -> Logger& {
	return MainLogger;
}
}  // namespace SimuMori
