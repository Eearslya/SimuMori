#pragma once

#include <SimuMori/Common.hpp>

namespace SimuMori {
class DecodeException : public std::runtime_error {
 public:
	DecodeException() : std::runtime_error("DecodeException") {}
	DecodeException(const char* what) : std::runtime_error(what) {}
};

class EncodeException : public std::runtime_error {
 public:
	EncodeException() : std::runtime_error("EncodeException") {}
	EncodeException(const char* what) : std::runtime_error(what) {}
};

class DataBuffer {
	using DataT = std::vector<U8>;

 public:
	DataBuffer() = default;
	DataBuffer(std::size_t initialSize);
	DataBuffer(const std::filesystem::path& dataFilePath);
	DataBuffer(const DataBuffer& other);
	DataBuffer(const DataBuffer& other, std::size_t offset);
	DataBuffer(DataBuffer&& other) noexcept;

	auto operator=(const DataBuffer& other) -> DataBuffer&;
	auto operator=(DataBuffer&& other) noexcept -> DataBuffer&;

	[[nodiscard]] auto Data() noexcept -> U8*;
	[[nodiscard]] auto Data() const noexcept -> const U8*;
	[[nodiscard]] auto GetReadCursor() const noexcept -> std::size_t;
	[[nodiscard]] auto GetRemaining() const noexcept -> std::size_t;
	[[nodiscard]] auto GetSize() const noexcept -> std::size_t;
	[[nodiscard]] auto IsEmpty() const noexcept -> bool;
	[[nodiscard]] auto IsFinished() const noexcept -> bool;

	auto Append(const DataBuffer& other) -> void;
	auto Chomp() -> void;
	auto Clear() -> void;
	auto Consume(DataBuffer& other) -> void;
	auto Resize(std::size_t newSize) -> void;
	auto RestoreCursor() -> void;
	auto SaveCursor() -> void;
	auto WriteToFile(const std::filesystem::path& dataFilePath) -> void;

	auto Read(void* data, std::size_t dataSize) const -> void;
	[[nodiscard]] auto ReadU64() const -> U64;
	[[nodiscard]] auto ReadU32() const -> U32;
	[[nodiscard]] auto ReadU16() const -> U16;
	[[nodiscard]] auto ReadU8() const -> U8;
	[[nodiscard]] auto ReadI64() const -> I64;
	[[nodiscard]] auto ReadI32() const -> I32;
	[[nodiscard]] auto ReadI16() const -> I16;
	[[nodiscard]] auto ReadI8() const -> I8;
	[[nodiscard]] auto ReadF64() const -> F64;
	[[nodiscard]] auto ReadF32() const -> F32;
	[[nodiscard]] auto ReadString() const -> std::string;

	auto Write(const void* data, std::size_t dataSize) -> DataBuffer&;
	auto WriteU64(U64 value) -> DataBuffer&;
	auto WriteU32(U32 value) -> DataBuffer&;
	auto WriteU16(U16 value) -> DataBuffer&;
	auto WriteU8(U8 value) -> DataBuffer&;
	auto WriteI64(I64 value) -> DataBuffer&;
	auto WriteI32(I32 value) -> DataBuffer&;
	auto WriteI16(I16 value) -> DataBuffer&;
	auto WriteI8(I8 value) -> DataBuffer&;
	auto WriteF64(F64 value) -> DataBuffer&;
	auto WriteF32(F32 value) -> DataBuffer&;
	auto WriteString(std::string_view value) -> DataBuffer&;

 private:
	auto EnsureRemain(std::size_t size) const -> void;
	auto EnsureWrite(std::size_t size) -> void;

	DataT _data;
	mutable std::size_t _readCursor = 0;
	std::size_t _savedReadCursor    = 0;
	std::size_t _writeCursor        = 0;
};
}  // namespace SimuMori
