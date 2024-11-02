#pragma once

#include <chrono>
#include <iostream>

namespace SimuMori {
using namespace std::chrono_literals;

class Time {
 public:
	constexpr Time() noexcept = default;
	template <typename Rep, typename Period>
	constexpr Time(const std::chrono::duration<Rep, Period>& duration) noexcept
			: _value(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) {}

	[[nodiscard]] static Time Now() {
		static const auto epoch = std::chrono::high_resolution_clock::now();

		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - epoch);
	}

	template <typename Rep = float>
	[[nodiscard]] static constexpr auto Hour(const Rep hours) noexcept -> Time {
		return Minutes(hours * static_cast<Rep>(60));
	}
	template <typename Rep = float>
	[[nodiscard]] static constexpr auto Minutes(const Rep minutes) noexcept -> Time {
		return Seconds(minutes * static_cast<Rep>(60));
	}
	template <typename Rep = float>
	[[nodiscard]] static constexpr auto Seconds(const Rep seconds) noexcept -> Time {
		return Time(std::chrono::duration<Rep>(seconds));
	}
	template <typename Rep = uint32_t>
	[[nodiscard]] static constexpr auto Milliseconds(const Rep milliseconds) noexcept -> Time {
		return Time(std::chrono::duration<Rep, std::milli>(milliseconds));
	}
	template <typename Rep = uint64_t>
	[[nodiscard]] static constexpr auto Microseconds(const Rep microseconds) noexcept -> Time {
		return Time(std::chrono::duration<Rep, std::micro>(microseconds));
	}

	template <typename T = float>
	[[nodiscard]] constexpr auto AsSeconds() const noexcept -> T {
		return static_cast<T>(_value.count()) / static_cast<T>(1'000'000);
	}
	template <typename T = uint32_t>
	[[nodiscard]] constexpr auto AsMilliseconds() const noexcept -> T {
		return static_cast<T>(_value.count()) / static_cast<T>(1'000);
	}
	template <typename T = uint64_t>
	[[nodiscard]] constexpr auto AsMicroseconds() const noexcept -> T {
		return static_cast<T>(_value.count());
	}

	template <typename Rep, typename Period>
	[[nodiscard]] constexpr operator std::chrono::duration<Rep, Period>() const noexcept {
		return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(_value);
	}

	[[nodiscard]] constexpr auto operator<=>(const Time& other) const noexcept {
		return _value <=> other._value;
	}

	[[nodiscard]] friend constexpr auto operator+(const Time& a, const Time& b) noexcept -> Time {
		return a._value + b._value;
	}
	[[nodiscard]] friend constexpr auto operator-(const Time& a, const Time& b) noexcept -> Time {
		return a._value - b._value;
	}
	[[nodiscard]] friend constexpr auto operator*(const Time& a, float b) noexcept -> Time {
		return a._value * b;
	}
	[[nodiscard]] friend constexpr auto operator*(const Time& a, int64_t b) noexcept -> Time {
		return a._value * b;
	}
	[[nodiscard]] friend constexpr auto operator/(const Time& a, float b) noexcept -> Time {
		return a._value / b;
	}
	[[nodiscard]] friend constexpr auto operator/(const Time& a, int64_t b) noexcept -> Time {
		return a._value / b;
	}
	[[nodiscard]] friend constexpr auto operator/(const Time& a, const Time& b) noexcept -> double {
		return static_cast<double>(a._value.count()) / static_cast<double>(b._value.count());
	}

 private:
	std::chrono::microseconds _value = {};
};

class ElapsedTime {
 public:
	ElapsedTime() noexcept {
		_lastTime = Time::Now();
	}

	[[nodiscard]] auto Get() const noexcept -> const Time& {
		return _delta;
	}

	auto Update() noexcept -> void {
		_startTime = Time::Now();
		_delta     = _startTime - _lastTime;
		_lastTime  = _startTime;
	}

 private:
	Time _delta;
	Time _lastTime;
	Time _startTime;
};

struct ZoneTimerT {
	ZoneTimerT(const char* title) : Title(title) {
		Elapsed.Update();
	}

	~ZoneTimerT() noexcept {
		Elapsed.Update();
		std::cout << Title << " in " << Elapsed.Get().AsMilliseconds() << "ms" << std::endl;
	}

	const char* Title;
	ElapsedTime Elapsed;
};
}  // namespace SimuMori

#define ZoneTimer(name) ::SimuMori::ZoneTimerT _zoneTimer(name)
