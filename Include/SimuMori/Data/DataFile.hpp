#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/DataBuffer.hpp>

namespace SimuMori {
class DataFile {
 public:
	DataFile(std::string_view fileName);

	auto Export() -> bool;
	auto Import(bool forceJson = false) -> bool;

	auto BinaryPath() const noexcept -> std::filesystem::path;
	auto JsonPath() const noexcept -> std::filesystem::path;

 protected:
	virtual auto LoadBinary(const DataBuffer&) -> bool   = 0;
	virtual auto LoadJson(const nlohmann::json&) -> bool = 0;
	virtual auto SaveBinary(DataBuffer&) const -> void   = 0;
	virtual auto SaveJson() const -> nlohmann::json      = 0;

 private:
	auto ExportBinary() noexcept -> bool;
	auto ExportJson() noexcept -> bool;
	auto ImportBinary() noexcept -> bool;
	auto ImportJson() noexcept -> bool;

	std::string _fileName;
};
}  // namespace SimuMori
