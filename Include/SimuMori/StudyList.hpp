#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
class StudyList {
 public:
	StudyList(std::string_view name);

	[[nodiscard]] auto GetName() const -> const std::string&;
	[[nodiscard]] auto ItemCount() const -> U32;
	[[nodiscard]] auto Items() const -> const std::vector<StudyItem::Identifier>&;

	auto AddItem(StudyItem::Identifier id) -> void;
	auto AddItems(std::span<StudyItem::Identifier> ids) -> void;

 private:
	std::string _name;
	std::vector<StudyItem::Identifier> _items;
};
}  // namespace SimuMori
