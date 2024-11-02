#pragma once

#include "ArrayDataFile.hpp"
#include "Common.hpp"
#include "StudyItem.hpp"

namespace SimuMori {
class StudyItemDataFile : public ArrayDataFile<StudyItem::Identifier> {
 public:
	StudyItemDataFile(const std::filesystem::path& filePath) : ArrayDataFile(filePath) {}
};
}  // namespace SimuMori
