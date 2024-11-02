#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

#include "Common.hpp"
#include "DataBuffer.hpp"

namespace SimuMori {
class DataFile {
 public:
	DataFile(const std::filesystem::path& filePath);

	auto Export() -> bool;
	auto Import(bool forceJson = false) -> bool;

	auto BinaryPath() const noexcept -> std::filesystem::path;
	auto JSONPath() const noexcept -> std::filesystem::path;

 protected:
	virtual auto LoadBinary(const DataBuffer&) -> bool   = 0;
	virtual auto LoadJSON(const nlohmann::json&) -> bool = 0;
	virtual auto SaveBinary(DataBuffer&) const -> void   = 0;
	virtual auto SaveJSON() const -> nlohmann::json      = 0;

 private:
	std::filesystem::path _filePath;
};
}  // namespace SimuMori
