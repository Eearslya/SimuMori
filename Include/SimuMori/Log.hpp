#pragma once

#include <spdlog/spdlog.h>

#include <string_view>

namespace SimuMori {
class Logger {
 public:
	template <typename... Args>
	auto Trace(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
		_logger->trace(format, std::forward<Args>(args)...);
	}

	template <typename... Args>
	auto Debug(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
		_logger->debug(format, std::forward<Args>(args)...);
	}

	template <typename... Args>
	auto Info(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
		_logger->info(format, std::forward<Args>(args)...);
	}

	template <typename... Args>
	auto Warning(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
		_logger->warn(format, std::forward<Args>(args)...);
	}

	template <typename... Args>
	auto Error(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
		_logger->error(format, std::forward<Args>(args)...);
	}

	template <typename... Args>
	auto Fatal(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
		_logger->critical(format, std::forward<Args>(args)...);
	}

	[[nodiscard]] static auto Create(std::string_view name) noexcept -> Logger;

 private:
	std::shared_ptr<spdlog::logger> _logger;
};

auto GetMainLogger() noexcept -> Logger&;

namespace Log {
template <typename... Args>
auto Trace(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
	::SimuMori::GetMainLogger().Trace(format, std::forward<Args>(args)...);
}

template <typename... Args>
auto Debug(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
	::SimuMori::GetMainLogger().Debug(format, std::forward<Args>(args)...);
}

template <typename... Args>
auto Info(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
	::SimuMori::GetMainLogger().Info(format, std::forward<Args>(args)...);
}

template <typename... Args>
auto Warning(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
	::SimuMori::GetMainLogger().Warning(format, std::forward<Args>(args)...);
}

template <typename... Args>
auto Error(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
	::SimuMori::GetMainLogger().Error(format, std::forward<Args>(args)...);
}

template <typename... Args>
auto Fatal(fmt::format_string<Args...> format, Args&&... args) noexcept -> void {
	::SimuMori::GetMainLogger().Fatal(format, std::forward<Args>(args)...);
}
}  // namespace Log
}  // namespace SimuMori
