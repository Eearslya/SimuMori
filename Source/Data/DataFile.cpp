#include <SimuMori/Data/DataFile.hpp>
#include <SimuMori/Time.hpp>

namespace SimuMori {
constexpr const U8 DataFileMagic[]            = {'M', 'A', 'R', 'U'};
constexpr const std::size_t DataFileMagicSize = sizeof(DataFileMagic) / sizeof(DataFileMagic[0]);

DataFile::DataFile(std::string_view fileName) : _fileName(fileName) {}

auto DataFile::Export() -> bool {
	ElapsedTime exportTimer;
	Log::Info("Exporting data file '{}'...", _fileName);
	if (ExportJson()) {
		exportTimer.Update();
		Log::Info("Data file exported in {}ms.", exportTimer.Get().AsMilliseconds());

		return true;
	}

	return false;
}

auto DataFile::Import(bool forceJson) -> bool {
	ElapsedTime importTimer;
	Log::Info("Importing data file '{}'...", _fileName);

	const auto jsonPath = JsonPath();
	const auto binPath  = BinaryPath();

	const auto jsonExist = std::filesystem::exists(jsonPath);
	const auto binExist  = std::filesystem::exists(binPath);
	if (!jsonExist && !binExist) {
		Log::Error("Failed to load data file: No JSON or Binary file found.");
		return false;
	}

	if (jsonExist && binExist) {
		// If the Json and Binary versions both exist, make sure we use the newer one.
		const auto jsonTime = std::filesystem::last_write_time(jsonPath);
		const auto binTime  = std::filesystem::last_write_time(binPath);
		if (binTime < jsonTime) {
			Log::Debug("JSON data is newer than binary. Ignoring binary file.");
			forceJson = true;
		}
	}

	bool loaded = false;
	if (binExist && !forceJson) { loaded = ImportBinary(); }
	if (!loaded) {
		loaded = ImportJson();

		// If we successfully loaded the JSON, export a binary version for faster loading later.
		if (loaded) { ExportBinary(); }
	}

	if (loaded) {
		importTimer.Update();
		Log::Info("Data file imported in {}ms.", importTimer.Get().AsMilliseconds());
	}

	return loaded;
}

auto DataFile::BinaryPath() const noexcept -> std::filesystem::path {
	return (std::filesystem::path("Data") / _fileName).replace_extension(".bin");
}

auto DataFile::JsonPath() const noexcept -> std::filesystem::path {
	return (std::filesystem::path("Data") / _fileName).replace_extension(".json");
}

auto DataFile::ExportBinary() noexcept -> bool {
	ElapsedTime exportTimer;

	try {
		DataBuffer buffer;
		buffer.Write(DataFileMagic, sizeof(DataFileMagic) / sizeof(DataFileMagic[0]));
		SaveBinary(buffer);
		buffer.WriteToFile(BinaryPath());
	} catch (const std::exception& e) {
		Log::Error("Failed to export binary data: {}", e.what());
		return false;
	}

	exportTimer.Update();
	Log::Debug("Binary data exported in {}ms.", exportTimer.Get().AsMilliseconds());

	return true;
}

auto DataFile::ExportJson() noexcept -> bool {
	ElapsedTime exportTimer;

	try {
		nlohmann::json dataRoot = SaveJson();

		std::ofstream outFile(JsonPath());
		if (!outFile.is_open()) { return false; }
		outFile << dataRoot;
		outFile.close();
	} catch (const std::exception& e) {
		Log::Error("Failed to export JSON data: {}", e.what());
		return false;
	}

	exportTimer.Update();
	Log::Debug("JSON data exported in {}ms.", exportTimer.Get().AsMilliseconds());

	return true;
}

auto DataFile::ImportBinary() noexcept -> bool {
	ElapsedTime importTimer;

	try {
		DataBuffer buffer(BinaryPath());
		// Verify our binary file is large enough to be valid.
		if (buffer.GetRemaining() < DataFileMagicSize) {
			Log::Debug("Binary data invalid: Not enough data.");
			return false;
		}

		// Verify our binary file has the magic header.
		U8 fileMagic[DataFileMagicSize];
		std::memset(fileMagic, 0, DataFileMagicSize);
		buffer.Read(fileMagic, DataFileMagicSize);
		if (std::memcmp(fileMagic, DataFileMagic, DataFileMagicSize) != 0) {
			Log::Debug("Binary data invalid: Incorrect magic value.");
			return false;
		}

		// Looks good, attempt to load the binary file.
		if (!LoadBinary(buffer)) { return false; }
	} catch (const std::exception& e) {
		Log::Debug("Failed to load binary data file: {}", e.what());
		return false;
	}

	importTimer.Update();
	Log::Debug("Binary data imported in {}ms.", importTimer.Get().AsMilliseconds());

	return true;
}

auto DataFile::ImportJson() noexcept -> bool {
	ElapsedTime importTimer;

	try {
		std::ifstream jsonFile(JsonPath());
		if (!jsonFile.is_open()) { return false; }

		const auto jsonData = nlohmann::json::parse(jsonFile);
		if (!LoadJson(jsonData)) {
			Log::Error("Failed to load data file: JSON data failed to parse.");
			return false;
		}
		jsonFile.close();
	} catch (const std::exception& e) {
		Log::Error("Failed to load JSON data file: {}", e.what());
		return false;
	}

	importTimer.Update();
	Log::Debug("JSON data imported in {}ms.", importTimer.Get().AsMilliseconds());

	return true;
}
}  // namespace SimuMori
