#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/DataBuffer.hpp>
#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
enum StudyEventAction : U8 {
	Lesson    = 0,
	LevelUp   = 1,
	LevelDown = 2,
};

struct StudyEvent {
	StudyEventAction Action;
	U8 Level;
	StudyItem::Identifier StudyItemId;
	U64 Timestamp;
	U16 UserId;

	auto Export() const -> nlohmann::json;
	auto Import(const nlohmann::json& jsonData) -> bool;
	auto Load(const DataBuffer& buf) -> bool;
	auto Save(DataBuffer& buf) const -> void;

	static auto FromJson(const nlohmann::json& jsonData) -> StudyEvent;
};
}  // namespace SimuMori
