#include <SimuMori/Common.hpp>
#include <SimuMori/Data/StudyEventDataFile.hpp>
#include <SimuMori/Data/StudyItemDataFile.hpp>
#include <SimuMori/Simulation.hpp>
#include <SimuMori/StudyList.hpp>
#include <SimuMori/Time.hpp>

#include "CrashHandler.hpp"

using namespace SimuMori;

auto main(int argc, const char** argv) -> int {
	CrashHandler::Initialize();

	try {
		Simulation sim;
		auto mainStudyList = sim.AddStudyList("MaruMori Main Study List");

		StudyEventDataFile history("kanjisrshistory-anon");
		StudyItemDataFile studyListData("studylist");

		history.Import();
		studyListData.Import();

		mainStudyList.AddItems(studyListData.All());

		sim.RunEvents(history.All());
	} catch (const std::exception& e) {
		std::cerr << std::endl << std::endl;
		std::cerr << "===================================" << std::endl;
		std::cerr << "=== FATAL EXCEPTION ENCOUNTERED ===" << std::endl;
		std::cerr << "===================================" << std::endl;
		std::cerr << e.what() << std::endl << std::endl;

		return 1;
	}

	CrashHandler::Shutdown();

	return 0;
}
