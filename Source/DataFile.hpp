#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

#include "Common.hpp"

namespace SimuMori {
class DataFile {
 public:
	DataFile(const std::filesystem::path& filePath);

	auto Export() -> bool;
	auto Import() -> bool;

	auto BinaryPath() const noexcept -> std::filesystem::path;
	auto JSONPath() const noexcept -> std::filesystem::path;

 protected:
	virtual auto LoadBinary(std::istream&) -> bool       = 0;
	virtual auto LoadJSON(const nlohmann::json&) -> bool = 0;
	virtual auto SaveBinary(std::ostream&) const -> void = 0;
	virtual auto SaveJSON(nlohmann::json&) const -> void = 0;

 private:
	std::filesystem::path _filePath;
};
}  // namespace SimuMori
