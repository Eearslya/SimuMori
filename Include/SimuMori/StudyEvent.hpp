#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/DataBuffer.hpp>
#include <SimuMori/Data/DataItem.hpp>
#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
enum StudyEventAction : U8 {
	Lesson    = 0,
	LevelUp   = 1,
	LevelDown = 2,
};

struct StudyEvent : public DataItem {
	StudyEventAction Action;
	U8 Level;
	StudyItem::Identifier StudyItemId;
	U64 Timestamp;
	U16 UserId;

	auto Export() const -> nlohmann::json override;
	auto Import(const nlohmann::json& jsonData) -> bool override;
	auto Load(const DataBuffer& buf) -> bool override;
	auto Save(DataBuffer& buf) const -> void override;

	static auto FromJson(const nlohmann::json& jsonData) -> StudyEvent;
};
}  // namespace SimuMori
