#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/DataBuffer.hpp>

namespace SimuMori {
class StudyItem {
 public:
	enum class Type {
		Kanji          = 0,
		VocabularyForm = 1,
	};

	struct Identifier {
		constexpr Identifier() noexcept = default;
		constexpr Identifier(U32 id) noexcept : Value(id) {}
		constexpr Identifier(Type type, U32 id) noexcept {
			Value = ((static_cast<U32>(type) << 24) | (id & 0xffffff));
		}
		Identifier(std::string_view stringId);

		constexpr auto ID() const noexcept -> U32 {
			return Value & 0xffffff;
		}
		constexpr auto Type() const noexcept -> Type {
			return static_cast<StudyItem::Type>((Value >> 24) & 0xff);
		}

		[[nodiscard]] auto ToString() const -> std::string;

		auto Export() const -> nlohmann::json;
		auto Import(const nlohmann::json&) -> bool;
		auto Load(const DataBuffer&) -> bool;
		auto Save(DataBuffer&) const -> void;

		constexpr auto operator==(const Identifier& other) const noexcept -> bool {
			return Value == other.Value;
		}

		U32 Value = 0;
	};
};
}  // namespace SimuMori
