#pragma once

#include "ArrayDataFile.hpp"
#include "Common.hpp"
#include "StudyEvent.hpp"

namespace SimuMori {
class StudyEventHistory : public ArrayDataFile<StudyEvent> {
 public:
	StudyEventHistory(const std::filesystem::path& filePath) : ArrayDataFile(filePath) {}
};
}  // namespace SimuMori
