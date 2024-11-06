#include <SimuMori/User.hpp>

namespace SimuMori {
auto User::ItemProgress::Learn() noexcept -> void {
	Level     = 1;
	PassCount = 0;
	FailCount = 0;
}

auto User::ItemProgress::Pass() noexcept -> void {
	if (Level == 0) { return; }

	Level = std::min<U8>(Level + 1, StudyItem::MaxLevel);
	PassCount++;
}

auto User::ItemProgress::Fail() noexcept -> void {
	if (Level == 0) { return; }

	Level = std::max(Level - (Level >= 5 ? 2 : 1), 1);
	FailCount++;
}

User::User(U32 id) noexcept : _userId(id) {}

auto User::GetItemProgress(StudyItem::Identifier id) -> ItemProgress& {
	return _progress[id];
}

auto User::GetItemProgress(StudyItem::Identifier id) const -> const ItemProgress& {
	return _progress.at(id);
}
}  // namespace SimuMori
