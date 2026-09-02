#ifndef UTILS_H_
#define UTILS_H_

#include <cassert>
#include <concepts>
#include <bit>
#include <cstdint>
#include <cstring>

namespace utils {

[[maybe_unused]] static void SecZeroMemory(void* ptr, std::size_t n) {
#ifdef _WIN32
   ::SecureZeroMemory(ptr, n);
#else
   assert(ptr);
   std::memset(ptr, 0, n);
   asm volatile("" : : "r"(ptr) : "memory");
#endif
}

#if __cplusplus >= 202302L

//
// LE
//
template<std::integral Type>
constexpr Type ValueToLE(Type value) noexcept {
   if constexpr (std::endian::native == std::endian::little)
        return value;
   else return std::byteswap(value);
}

template<std::integral Type, typename PtrType = const uint8_t*>
constexpr Type ReadLE(PtrType ptr) noexcept {
   Type a;
   std::memcpy(&a, ptr, sizeof(Type));
   return ValueToLE(a);
}

template<std::integral Type, typename PtrType = uint8_t*>
constexpr void WriteLE(PtrType ptr, Type value) noexcept {
   value = ValueToLE(value);
   std::memcpy(ptr, &value, sizeof(Type));
}

//
// BE
//
template<std::integral Type>
constexpr Type ValueToBE(Type value) noexcept {
   if constexpr (std::endian::native == std::endian::big)
        return value;
   else return std::byteswap(value);
}

template<std::integral Type, typename PtrType = uint8_t*>
constexpr Type ReadBE(PtrType ptr) noexcept {
   Type a;
   std::memcpy(&a, ptr, sizeof(Type));
   return ValueToBE(a);
}

template<std::integral Type, typename PtrType = uint8_t*>
constexpr void WriteBE(PtrType ptr, Type value) noexcept {
   value = ValueToBE(value);
   std::memcpy(ptr, &value, sizeof(Type));
}

#endif

} // namespace utils

#endif // UTILS_H_
