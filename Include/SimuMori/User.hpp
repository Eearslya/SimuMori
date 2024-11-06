#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
class User {
 public:
	struct ItemProgress {
		U64 PassCount = 0;
		U64 FailCount = 0;
		U8 Level      = 0;

		auto Learn() noexcept -> void;
		auto Pass() noexcept -> void;
		auto Fail() noexcept -> void;
	};

	User(U32 id) noexcept;

	auto GetItemProgress(StudyItem::Identifier id) -> ItemProgress&;
	auto GetItemProgress(StudyItem::Identifier id) const -> const ItemProgress&;

 private:
	U32 _userId;
	std::unordered_map<StudyItem::Identifier, ItemProgress> _progress;
};
}  // namespace SimuMori
