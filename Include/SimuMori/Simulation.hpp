#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
class StudyEvent;
class StudyList;
class User;

class Simulation {
 public:
	Simulation() noexcept;
	Simulation(const Simulation&) = delete;
	Simulation(Simulation&&)      = delete;
	~Simulation() noexcept;

	void operator=(const Simulation&) = delete;
	void operator=(Simulation&&)      = delete;

	auto AddStudyList(std::string_view listName) -> StudyList&;
	auto Reset() -> void;
	auto RunEvents(const std::span<StudyEvent> events) -> void;

 private:
	struct StudyItemData {
		U64 PassCount = 0;
		U64 FailCount = 0;
		std::set<U16> UsersStudying;
	};

	auto GetItemData(StudyItem::Identifier id) -> StudyItemData&;
	auto GetItemData(StudyItem::Identifier id) const -> const StudyItemData&;
	auto GetUser(U32 id) -> User&;
	auto GetUser(U32 id) const -> const User&;
	auto ProcessEvent(const StudyEvent& event) -> void;

	U64 _studyEventDiscrepancyCount = 0;
	std::unordered_map<StudyItem::Identifier, StudyItemData> _studyItemData;
	std::vector<std::unique_ptr<StudyList>> _studyLists;
	std::vector<std::unique_ptr<User>> _users;
};
}  // namespace SimuMori
