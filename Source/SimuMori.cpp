#include <SimuMori/Common.hpp>
#include <SimuMori/Data/StudyEventDataFile.hpp>
#include <SimuMori/Data/StudyItemDataFile.hpp>
#include <SimuMori/Time.hpp>

#include "CrashHandler.hpp"

using namespace SimuMori;

auto main(int argc, const char** argv) -> int {
	CrashHandler::Initialize();

	try {
		StudyEventDataFile history("Data/kanjisrshistory-anon.json");
		StudyItemDataFile mainStudylist("Data/studylist.json");

		{
			ZoneTimer("Kanji SRS History Loaded");
			history.Import();
		}
		{
			ZoneTimer("Main Study List Loaded");
			mainStudylist.Import();
		}
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
