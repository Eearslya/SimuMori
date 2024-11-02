#include "DataFile.hpp"

namespace SimuMori {
DataFile::DataFile(const std::filesystem::path& filePath)
		: _filePath(std::filesystem::path(filePath).replace_extension()) {}

auto DataFile::Export() -> bool {
	nlohmann::json dataRoot = SaveJSON();

	std::ofstream outFile(JSONPath());
	if (!outFile.is_open()) { return false; }
	outFile << dataRoot;
	outFile.close();

	return true;
}

auto DataFile::Import(bool forceJson) -> bool {
	const auto jsonPath = JSONPath();
	const auto binPath  = BinaryPath();

	const auto jsonExist = std::filesystem::exists(jsonPath);
	const auto binExist  = std::filesystem::exists(binPath);
	if (!jsonExist && !binExist) {
		std::cerr << "Failed to open data file: No JSON or Binary found!" << std::endl;
		return false;
	}

	bool loadBinary = binExist;
	if (jsonExist && binExist) {
		// If the JSON and Binary versions both exist, make sure we use the newer one.
		const auto jsonTime = std::filesystem::last_write_time(jsonPath);
		const auto binTime  = std::filesystem::last_write_time(binPath);
		if (binTime < jsonTime) { loadBinary = false; }
	}

	if (forceJson) { loadBinary = false; }
	if (loadBinary) {
		DataBuffer buffer(binPath);

		return LoadBinary(buffer);
	} else {
		std::ifstream jsonFile(jsonPath);
		if (!jsonFile.is_open()) { return false; }

		const auto jsonData = nlohmann::json::parse(jsonFile);
		if (!LoadJSON(jsonData)) { return false; }
		jsonFile.close();

		// Save the data to binary format after loading, for faster subsequent loads.
		DataBuffer buffer;
		SaveBinary(buffer);
		buffer.WriteToFile(binPath);
	}

	return true;
}

auto DataFile::BinaryPath() const noexcept -> std::filesystem::path {
	return std::filesystem::path(_filePath).replace_extension(".bin");
}

auto DataFile::JSONPath() const noexcept -> std::filesystem::path {
	return std::filesystem::path(_filePath).replace_extension(".json");
}
}  // namespace SimuMori
