#include <SimuMori/StudyList.hpp>

namespace SimuMori {
StudyList::StudyList(std::string_view name) : _name(name) {}

auto StudyList::GetName() const -> const std::string& {
	return _name;
}

auto StudyList::ItemCount() const -> U32 {
	return _items.size();
}

auto StudyList::Items() const -> const std::vector<StudyItem::Identifier>& {
	return _items;
}

auto StudyList::AddItem(StudyItem::Identifier id) -> void {
	_items.push_back(id);
}

auto StudyList::AddItems(std::span<StudyItem::Identifier> ids) -> void {
	for (const auto id : ids) { AddItem(id); }
}
}  // namespace SimuMori
