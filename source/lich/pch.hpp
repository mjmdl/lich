#ifndef LICH_PCH_HPP
#define LICH_PCH_HPP

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace lich {

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
using Usize = std::size_t;
using F32 = float;
using F64 = double;
using B8 = I8;
using B16 = I16;
using B32 = I32;
using Cstr = const char *;

}

#endif
