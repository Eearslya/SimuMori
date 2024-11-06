#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/ArrayDataFile.hpp>
#include <SimuMori/StudyEvent.hpp>

namespace SimuMori {
class StudyEventDataFile : public ArrayDataFile<StudyEvent> {
 public:
	StudyEventDataFile(std::string_view fileName) : ArrayDataFile(fileName) {}
};
}  // namespace SimuMori
