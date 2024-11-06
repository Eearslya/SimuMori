#include <SimuMori/Simulation.hpp>
#include <SimuMori/StudyEvent.hpp>
#include <SimuMori/StudyList.hpp>
#include <SimuMori/Time.hpp>
#include <SimuMori/User.hpp>

namespace SimuMori {
Simulation::Simulation() noexcept {}

Simulation::~Simulation() noexcept {}

auto Simulation::AddStudyList(std::string_view listName) -> StudyList& {
	auto newList = std::make_unique<StudyList>(listName);
	_studyLists.emplace_back(std::move(newList));

	return *_studyLists.back();
}

auto Simulation::Reset() -> void {
	_studyEventDiscrepancyCount = 0;
	_studyItemData.clear();
	_users.clear();
}

auto Simulation::RunEvents(const std::span<StudyEvent> events) -> void {
	if (events.empty()) { return; }

	ElapsedTime simTimer;
	Log::Info("Processing {} study events...", events.size());

	std::vector<StudyEvent> studyEvents(events.begin(), events.end());
	{
		ElapsedTime sortTimer;
		std::sort(studyEvents.begin(), studyEvents.end(), [](const auto& a, const auto& b) -> bool {
			return a.Timestamp < b.Timestamp;
		});
		sortTimer.Update();
		Log::Debug("Events list sorted in {}ms.", sortTimer.Get().AsMilliseconds());
	}

	// To remain consistent with the simulator side of things, we need to know which "hour" each
	// of these events occurs in.
	const auto firstTimestamp = Time::Milliseconds(studyEvents[0].Timestamp);

	_studyEventDiscrepancyCount = 0;
	for (const auto& event : studyEvents) { ProcessEvent(event); }

	if (_studyEventDiscrepancyCount > 0) {
		Log::Warning("Found {} discrepancies when parsing data.", _studyEventDiscrepancyCount);
	}

	simTimer.Update();
	Log::Info("Processed {} study events in {}ms.", events.size(), simTimer.Get().AsMilliseconds());

	Log::Debug("{} users created.", _users.size());
}

auto Simulation::GetItemData(StudyItem::Identifier id) -> StudyItemData& {
	return _studyItemData[id];
}

auto Simulation::GetItemData(StudyItem::Identifier id) const -> const StudyItemData& {
	return _studyItemData.at(id);
}

auto Simulation::GetUser(U32 id) -> User& {
	if (id >= _users.size()) { _users.resize(id + 1); }
	auto& userPtr = _users[id];
	if (!userPtr) { userPtr.reset(new User(id)); }

	return *userPtr;
}

auto Simulation::GetUser(U32 id) const -> const User& {
	return *_users.at(id);
}

auto Simulation::ProcessEvent(const StudyEvent& event) -> void {
	// First, gather up all of the data structures we're going to need.
	auto& itemData           = GetItemData(event.StudyItemId);
	auto& user               = GetUser(event.UserId);
	auto& userProgress       = user.GetItemProgress(event.StudyItemId);
	const auto originalLevel = userProgress.Level;

	// Now we determine what level the study item is supposed to be after this event.
	if (event.Action == StudyEventAction::Lesson) {
		userProgress.Learn();
		itemData.UsersStudying.insert(event.UserId);
	} else if (event.Action == StudyEventAction::LevelUp) {
		userProgress.Pass();
		itemData.PassCount++;
	} else if (event.Action == StudyEventAction::LevelDown) {
		userProgress.Fail();
		itemData.FailCount++;

		// During the early days of MaruMori, there was a bug where the logic that determined how many levels
		// you lost on a failed review was reversed. For accurate results, we need to simulate that bug as well.
		if (event.Timestamp <= 1678890905182ULL) {
			// Ensure we don't cause an unsigned integer underflow by trying to subtract 2 from 1.
			if (originalLevel > 1) { userProgress.Level = std::max(originalLevel - (originalLevel >= 5 ? 1 : 2), 1); }
		}
	}

	// The event log isn't quite perfect, as it does not include things such as users manually
	// demoting items. If for some reason the event log and our internal state gets desynced,
	// we simply force the item to match the event log.
	if (userProgress.Level != event.Level) {
		_studyEventDiscrepancyCount++;
		userProgress.Level = event.Level;
	}
}
}  // namespace SimuMori
