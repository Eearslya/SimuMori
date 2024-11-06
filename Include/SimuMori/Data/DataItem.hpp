#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/DataBuffer.hpp>

namespace SimuMori {
class DataItem {
 public:
	virtual auto Export() const -> nlohmann::json               = 0;
	virtual auto Import(const nlohmann::json& jsonData) -> bool = 0;
	virtual auto Load(const DataBuffer& buf) -> bool            = 0;
	virtual auto Save(DataBuffer& buf) const -> void            = 0;
};
}  // namespace SimuMori
