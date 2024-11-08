#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/DataFile.hpp>
#include <SimuMori/Data/DataItem.hpp>

namespace SimuMori {
template <typename T>
	requires(std::is_base_of_v<DataItem, T>)
class ArrayDataFile : public DataFile {
 public:
	ArrayDataFile(std::string_view fileName) : DataFile(fileName) {}

	auto All() -> std::vector<T>& {
		return _dataItems;
	}
	auto All() const -> const std::vector<T>& {
		return _dataItems;
	}
	auto Clear() -> void {
		_dataItems.clear();
	}

 protected:
	virtual auto LoadBinary(const DataBuffer& buf) -> bool override {
		const auto arraySize = buf.ReadU32();
		_dataItems.resize(arraySize);

		for (U32 i = 0; i < arraySize; ++i) {
			if (!_dataItems[i].Load(buf)) { return false; }
		}

		return true;
	}

	virtual auto LoadJson(const nlohmann::json& jsonData) -> bool override {
		if (!jsonData.is_array()) {
			std::cerr << "Invalid data file array format." << std::endl;
			return false;
		}

		_dataItems.clear();
		for (const auto& itemData : jsonData) {
			auto& item = _dataItems.emplace_back();
			if (!item.Import(itemData)) { continue; }
		}

		return true;
	}

	virtual auto SaveBinary(DataBuffer& buf) const -> void override {
		buf.WriteU32(_dataItems.size());
		for (const auto& item : _dataItems) { item.Save(buf); }
	}

	virtual auto SaveJson() const -> nlohmann::json override {
		auto jsonData = nlohmann::json::array();
		for (const auto& item : _dataItems) { jsonData.push_back(item.Export()); }

		return jsonData;
	}

 private:
	std::vector<T> _dataItems;
};
}  // namespace SimuMori
