#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
StudyItem::Identifier::Identifier(std::string_view stringId) {
	const auto slash = stringId.find_first_of('/');
	if (slash == std::string::npos) { throw std::runtime_error("Invalid Item ID"); }
	const auto typeStr = stringId.substr(0, slash);
	const std::string id(stringId.substr(slash + 1));

	StudyItem::Type type;
	if (typeStr == std::string_view("Kanji")) {
		type = StudyItem::Type::Kanji;
	} else if (typeStr == std::string_view("VocabularyForms")) {
		type = StudyItem::Type::VocabularyForm;
	} else {
		throw std::runtime_error("Invalid Item Type");
	}

	Value = ((static_cast<U32>(type) << 24) | (std::stoi(id) & 0xffffff));
}

auto StudyItem::Identifier::ToString() const -> std::string {
	std::string str;

	switch (Type()) {
		case StudyItem::Type::Kanji:
			str = "Kanji/";
			break;
		case StudyItem::Type::VocabularyForm:
			str = "VocabularyForm/";
			break;
	}

	str += std::to_string(ID());

	return str;
}

auto StudyItem::Identifier::Export() const -> nlohmann::json {
	return nlohmann::json(ToString());
}

auto StudyItem::Identifier::Import(const nlohmann::json& jsonData) -> bool {
	if (!jsonData.is_string()) { return false; }

	const auto str = jsonData.template get<std::string>();
	*this          = StudyItem::Identifier(str);

	return true;
}

auto StudyItem::Identifier::Load(const DataBuffer& buf) -> bool {
	Value = buf.ReadU32();

	return true;
}

auto StudyItem::Identifier::Save(DataBuffer& buf) const -> void {
	buf.WriteU32(Value);
}
}  // namespace SimuMori
