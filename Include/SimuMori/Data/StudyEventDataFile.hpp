#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/ArrayDataFile.hpp>
#include <SimuMori/StudyEvent.hpp>

namespace SimuMori {
class StudyEventDataFile : public ArrayDataFile<StudyEvent> {
 public:
	StudyEventDataFile(const std::filesystem::path& filePath) : ArrayDataFile(filePath) {}
};
}  // namespace SimuMori
