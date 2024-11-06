#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/ArrayDataFile.hpp>
#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
class StudyItemDataFile : public ArrayDataFile<StudyItem::Identifier> {
 public:
	StudyItemDataFile(std::string_view fileName) : ArrayDataFile(fileName) {}
};
}  // namespace SimuMori
