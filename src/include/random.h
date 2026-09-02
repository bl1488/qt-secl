#ifndef XOSHIRO256_H_
#define XOSHIRO256_H_

#include <random>

namespace utils {

//
// Xoshiro256Generator
//
class Xoshiro256Generator {
public:
   using result_type = uint64_t;

   static constexpr result_type min() noexcept { return 0; }
   static constexpr result_type max() noexcept { 
      return std::numeric_limits<result_type>::max(); 
   }

public:
   Xoshiro256Generator() noexcept {
      std::random_device rd;
      for (auto& i : array_)
         i = static_cast<uint64_t>(rd()) << 32 | rd();
   }

   constexpr uint64_t operator()() noexcept {
      const uint64_t result = Rotl(array_[1] * 5, 7) * 9;
      const uint64_t tmp    = array_[1] << 17;
      
      array_[2] ^= array_[0]; array_[3] ^= array_[1];
      array_[1] ^= array_[2]; array_[0] ^= array_[3];
      array_[2] ^= tmp;       array_[3] = Rotl(array_[3], 45);

      return result;
   }

private:
   constexpr uint64_t Rotl(const uint64_t x, int k)
      const noexcept { return (x << k) | (x >> (64 - k)); }

private:
   uint64_t array_[4];
};

template<std::integral Type = std::size_t>
Type Random() noexcept {
   static thread_local Xoshiro256Generator gen;
   return static_cast<Type>(gen());
}

template<std::integral Type = std::size_t>
Type Random(Type from, Type to) noexcept {
   assert(to >= from);
   static thread_local Xoshiro256Generator gen;
   return std::uniform_int_distribution<Type>(from, to)(gen);
}

} // namespace utils

#endif // XOSHIRO256_RANDOM_H_