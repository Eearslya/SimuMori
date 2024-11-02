#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/ArrayDataFile.hpp>
#include <SimuMori/StudyItem.hpp>

namespace SimuMori {
class StudyItemDataFile : public ArrayDataFile<StudyItem::Identifier> {
 public:
	StudyItemDataFile(const std::filesystem::path& filePath) : ArrayDataFile(filePath) {}
};
}  // namespace SimuMori
