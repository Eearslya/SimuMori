#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace SimuMori {
using U64 = std::uint64_t;
using U32 = std::uint32_t;
using U16 = std::uint16_t;
using U8  = std::uint8_t;
using I64 = std::int64_t;
using I32 = std::int32_t;
using I16 = std::int16_t;
using I8  = std::int8_t;
using F64 = double;
using F32 = float;
}  // namespace SimuMori
