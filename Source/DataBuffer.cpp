#include "DataBuffer.hpp"

namespace SimuMori {
template <typename T>
	requires(std::integral<T> || std::floating_point<T>)
[[nodiscard]] auto ReadNumeric(const DataBuffer& buffer) -> T {
	T value = static_cast<T>(0);
	buffer.Read(&value, sizeof(T));

	return value;
}

template <typename T>
	requires(std::integral<T> || std::floating_point<T>)
auto WriteNumeric(DataBuffer& buffer, T value) -> DataBuffer& {
	return buffer.Write(&value, sizeof(T));
}

DataBuffer::DataBuffer(std::size_t initialSize) {
	_data.resize(initialSize);
}

DataBuffer::DataBuffer(const std::filesystem::path& dataFilePath) {
	std::ifstream dataFile(dataFilePath, std::ios::binary);
	if (!dataFile.is_open()) { throw std::runtime_error("Unable to open DataBuffer file"); }

	dataFile.ignore(std::numeric_limits<std::streamsize>::max());
	const auto dataFileSize = dataFile.gcount();
	dataFile.clear();
	dataFile.seekg(0, std::ios::beg);

	_data.resize(dataFileSize);
	dataFile.read(reinterpret_cast<char*>(_data.data()), dataFileSize);

	dataFile.close();
}

DataBuffer::DataBuffer(const DataBuffer& other)
		: _data(other._data),
			_readCursor(other._readCursor),
			_savedReadCursor(other._savedReadCursor),
			_writeCursor(other._writeCursor) {}

DataBuffer::DataBuffer(const DataBuffer& other, std::size_t offset) {
	_data.resize(other.GetSize() - offset);
	std::copy(other._data.begin() + offset, other._data.end(), std::back_inserter(_data));
}

DataBuffer::DataBuffer(DataBuffer&& other) noexcept {
	*this = std::move(other);
}

auto DataBuffer::operator=(const DataBuffer& other) -> DataBuffer& {
	_data            = other._data;
	_readCursor      = other._readCursor;
	_savedReadCursor = other._savedReadCursor;
	_writeCursor     = other._writeCursor;

	return *this;
}

auto DataBuffer::operator=(DataBuffer&& other) noexcept -> DataBuffer& {
	_data            = std::move(other._data);
	_readCursor      = std::move(other._readCursor);
	_savedReadCursor = std::move(other._savedReadCursor);
	_writeCursor     = std::move(other._writeCursor);

	other._data.clear();
	other._readCursor      = 0;
	other._savedReadCursor = 0;
	other._writeCursor     = 0;

	return *this;
}

auto DataBuffer::Data() noexcept -> U8* {
	return _data.data();
}

auto DataBuffer::Data() const noexcept -> const U8* {
	return _data.data();
}

auto DataBuffer::GetReadCursor() const noexcept -> std::size_t {
	return _readCursor;
}

auto DataBuffer::GetRemaining() const noexcept -> std::size_t {
	return std::max<std::size_t>(0, GetSize() - GetReadCursor());
}

auto DataBuffer::GetSize() const noexcept -> std::size_t {
	return _data.size();
}

auto DataBuffer::IsEmpty() const noexcept -> bool {
	return _data.empty();
}

auto DataBuffer::IsFinished() const noexcept -> bool {
	return _readCursor >= _data.size();
}

auto DataBuffer::Append(const DataBuffer& other) -> void {
	if (other.GetSize() == 0) { return; }

	std::copy(other._data.begin(), other._data.end(), std::back_inserter(_data));
}

auto DataBuffer::Chomp() -> void {
	if (_readCursor == 0) { return; }

	if (_readCursor >= _data.size()) {
		Clear();
		return;
	}

	_data.erase(_data.begin(), _data.begin() + _readCursor);
	_readCursor      = 0;
	_savedReadCursor = 0;
}

auto DataBuffer::Clear() -> void {
	_data.clear();
	_readCursor      = 0;
	_savedReadCursor = 0;
	_writeCursor     = 0;
}

auto DataBuffer::Consume(DataBuffer& other) -> void {
	Append(other);
	other.Clear();
}

auto DataBuffer::Resize(std::size_t newSize) -> void {
	_data.resize(newSize);
}

auto DataBuffer::RestoreCursor() -> void {
	_readCursor      = _savedReadCursor;
	_savedReadCursor = 0;
}

auto DataBuffer::SaveCursor() -> void {
	_savedReadCursor = _readCursor;
}

auto DataBuffer::WriteToFile(const std::filesystem::path& dataFilePath) -> void {
	std::ofstream dataFile(dataFilePath, std::ios::binary);
	if (!dataFile.is_open()) { throw std::runtime_error("Failed to open data file for writing!"); }

	dataFile.write(reinterpret_cast<const char*>(_data.data()), _data.size());

	dataFile.close();
}

auto DataBuffer::Read(void* data, std::size_t dataSize) const -> void {
	if (dataSize <= 0) { return; }

	EnsureRemain(dataSize);
	std::memcpy(data, &_data[_readCursor], dataSize);
	_readCursor += dataSize;
}

auto DataBuffer::ReadU64() const -> U64 {
	return ReadNumeric<U64>(*this);
}

auto DataBuffer::ReadU32() const -> U32 {
	return ReadNumeric<U32>(*this);
}

auto DataBuffer::ReadU16() const -> U16 {
	return ReadNumeric<U16>(*this);
}

auto DataBuffer::ReadU8() const -> U8 {
	return ReadNumeric<U8>(*this);
}

auto DataBuffer::ReadI64() const -> I64 {
	return ReadNumeric<I64>(*this);
}

auto DataBuffer::ReadI32() const -> I32 {
	return ReadNumeric<I32>(*this);
}

auto DataBuffer::ReadI16() const -> I16 {
	return ReadNumeric<I16>(*this);
}

auto DataBuffer::ReadI8() const -> I8 {
	return ReadNumeric<I8>(*this);
}

auto DataBuffer::ReadF64() const -> F64 {
	return ReadNumeric<F64>(*this);
}

auto DataBuffer::ReadF32() const -> F32 {
	return ReadNumeric<F32>(*this);
}

auto DataBuffer::ReadString() const -> std::string {
	std::string value;
	const auto stringLength = ReadU16();
	value.resize(stringLength);
	Read(value.data(), stringLength);

	return value;
}

auto DataBuffer::Write(const void* data, std::size_t dataSize) -> DataBuffer& {
	EnsureWrite(dataSize);

	std::memcpy(&_data[_writeCursor], data, dataSize);
	_writeCursor += dataSize;

	return *this;
}

auto DataBuffer::WriteU64(U64 value) -> DataBuffer& {
	return WriteNumeric<U64>(*this, value);
}

auto DataBuffer::WriteU32(U32 value) -> DataBuffer& {
	return WriteNumeric<U32>(*this, value);
}

auto DataBuffer::WriteU16(U16 value) -> DataBuffer& {
	return WriteNumeric<U16>(*this, value);
}

auto DataBuffer::WriteU8(U8 value) -> DataBuffer& {
	return WriteNumeric<U8>(*this, value);
}

auto DataBuffer::WriteI64(I64 value) -> DataBuffer& {
	return WriteNumeric<I64>(*this, value);
}

auto DataBuffer::WriteI32(I32 value) -> DataBuffer& {
	return WriteNumeric<I32>(*this, value);
}

auto DataBuffer::WriteI16(I16 value) -> DataBuffer& {
	return WriteNumeric<I16>(*this, value);
}

auto DataBuffer::WriteI8(I8 value) -> DataBuffer& {
	return WriteNumeric<I8>(*this, value);
}

auto DataBuffer::WriteF64(F64 value) -> DataBuffer& {
	return WriteNumeric<F64>(*this, value);
}

auto DataBuffer::WriteF32(F32 value) -> DataBuffer& {
	return WriteNumeric<F32>(*this, value);
}

auto DataBuffer::WriteString(std::string_view value) -> DataBuffer& {
	return WriteU16(value.size()).Write(value.data(), value.size());
}

auto DataBuffer::EnsureRemain(std::size_t size) const -> void {
	if (_readCursor + size > _data.size()) { throw DecodeException("DataBuffer read out of range"); }
}

auto DataBuffer::EnsureWrite(std::size_t size) -> void {
	if (_writeCursor + size > _data.size()) { _data.resize(_writeCursor + size); }
}
}  // namespace SimuMori
