#pragma once

#include <SimuMori/Common.hpp>
#include <SimuMori/Data/DataBuffer.hpp>
#include <SimuMori/Data/DataItem.hpp>

namespace SimuMori {
class StudyItem {
 public:
	enum class Type {
		Kanji          = 0,
		VocabularyForm = 1,
	};

	struct Identifier : public DataItem {
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

		auto Export() const -> nlohmann::json override;
		auto Import(const nlohmann::json&) -> bool override;
		auto Load(const DataBuffer&) -> bool override;
		auto Save(DataBuffer&) const -> void override;

		constexpr operator U32() const noexcept {
			return Value;
		}

		constexpr auto operator==(const Identifier& other) const noexcept -> bool {
			return Value == other.Value;
		}

		U32 Value = 0;
	};

	constexpr static inline U8 MaxLevel = 9;

	constexpr StudyItem(Identifier id) noexcept : _identifier(id) {}
	constexpr StudyItem(Type type, U32 id) noexcept : StudyItem(Identifier(type, id)) {}
	virtual ~StudyItem() = default;

 private:
	Identifier _identifier;
};
}  // namespace SimuMori

namespace std {
template <>
struct hash<SimuMori::StudyItem::Identifier> {
	auto operator()(const SimuMori::StudyItem::Identifier& v) const noexcept -> std::size_t {
		return std::hash<std::uint32_t>{}(static_cast<std::uint32_t>(v));
	}
};
}  // namespace std
