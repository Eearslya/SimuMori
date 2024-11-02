#include <SimuMori/StudyEvent.hpp>

namespace SimuMori {
auto StudyEvent::Export() const -> nlohmann::json {
	auto jsonData = nlohmann::json::object();

	switch (Action) {
		case StudyEventAction::Lesson:
			jsonData["action"] = "lesson";
			break;
		case StudyEventAction::LevelUp:
			jsonData["action"] = "level-up";
			break;
		case StudyEventAction::LevelDown:
			jsonData["action"] = "level-down";
			break;
	}

	jsonData["itemId"]    = StudyItemId.ToString();
	jsonData["level"]     = static_cast<int>(Level);
	jsonData["timestamp"] = Timestamp;

	const auto userKey  = std::string("anon-") + std::to_string(UserId);
	jsonData["userKey"] = userKey;

	return jsonData;
}

auto StudyEvent::Import(const nlohmann::json& jsonData) -> bool {
	if (!jsonData.is_object()) { return false; }

	if (jsonData.contains("action")) {
		const auto& action = jsonData.at("action");
		if (!action.is_string()) { return false; }

		const auto actionStr = action.template get<std::string>();
		if (actionStr == "lesson") {
			Action = StudyEventAction::Lesson;
		} else if (actionStr == "level-up") {
			Action = StudyEventAction::LevelUp;
		} else if (actionStr == "level-down") {
			Action = StudyEventAction::LevelDown;
		} else {
			std::cerr << "Invalid StudyEventAction: " << actionStr << std::endl;
			return false;
		}
	} else {
		std::cerr << "Invalid StudyEvent: Missing 'action'." << std::endl;
		return false;
	}

	if (jsonData.contains("itemId")) {
		const auto& itemId = jsonData.at("itemId");
		if (!itemId.is_string()) { return false; }

		const auto itemStr = itemId.template get<std::string>();
		StudyItemId        = StudyItem::Identifier(itemStr);
	} else {
		std::cerr << "Invalid StudyEvent: Missing 'itemId'." << std::endl;
		return false;
	}

	if (jsonData.contains("level")) {
		const auto& level = jsonData.at("level");
		if (!level.is_number()) { return false; }

		Level = level.template get<U8>();
	} else {
		std::cerr << "Invalid StudyEvent: Missing 'level'." << std::endl;
		return false;
	}

	if (jsonData.contains("timestamp")) {
		const auto& timestamp = jsonData.at("timestamp");
		if (!timestamp.is_number()) { return false; }

		Timestamp = timestamp.template get<U64>();
	} else {
		std::cerr << "Invalid StudyEvent: Missing 'timestamp'." << std::endl;
		return false;
	}

	if (jsonData.contains("userKey")) {
		const auto& userKey = jsonData.at("userKey");
		if (!userKey.is_string()) { return false; }

		const auto userStr = userKey.template get<std::string>();
		if (userStr.starts_with("anon-")) {
			const auto userId = userStr.substr(5);
			UserId            = std::stoi(userId);
		} else {
			std::cerr << "Invalid StudyEvent: User Key '" << userStr << "' invalid." << std::endl;
			return false;
		}
	} else {
		std::cerr << "Invalid StudyEvent: Missing 'userKey'." << std::endl;
		return false;
	}

	return true;
}

auto StudyEvent::Load(const DataBuffer& buf) -> bool {
	Action      = static_cast<StudyEventAction>(buf.ReadU8());
	StudyItemId = StudyItem::Identifier(buf.ReadU32());
	Level       = buf.ReadU8();
	Timestamp   = buf.ReadU64();
	UserId      = buf.ReadU16();

	return true;
}

auto StudyEvent::Save(DataBuffer& buf) const -> void {
	buf.WriteU8(static_cast<U8>(Action));
	buf.WriteU32(StudyItemId.Value);
	buf.WriteU8(Level);
	buf.WriteU64(Timestamp);
	buf.WriteU16(UserId);
}

auto StudyEvent::FromJson(const nlohmann::json& jsonData) -> StudyEvent {
	StudyEvent event;
	if (!event.Import(jsonData)) { throw std::runtime_error("Failed to import StudyEvent."); }

	return event;
}
}  // namespace SimuMori
