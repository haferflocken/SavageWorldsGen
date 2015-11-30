#pragma once

#include <stdint.h>
#include <ostream>

namespace savage {
/**
 * The different dice in Savage Worlds.
 */
enum class dice_e : uint8_t {
  none = 0,
  d4,
  d6,
  d8,
  d10,
  d12,
  d12_plus_1,
  d12_plus_2,
  count,
  invalid = count
};

template<typename T>
inline dice_e operator+( dice_e lhs, T rhs ) {
  return static_cast<dice_e>( static_cast<T>( lhs ) + rhs );
}

template<typename T>
inline dice_e operator-( dice_e lhs, T rhs ) {
  return static_cast<dice_e>( static_cast<T>( lhs ) - rhs );
}

inline std::ostream& operator<<( std::ostream& o, dice_e d ) {
  switch( d ) {
  case dice_e::none:
    o << "none";
    break;
  case dice_e::d4:
    o << "d4";
    break;
  case dice_e::d6:
    o << "d6";
    break;
  case dice_e::d8:
    o << "d8";
    break;
  case dice_e::d10:
    o << "d10";
    break;
  case dice_e::d12:
    o << "d12";
    break;
  case dice_e::d12_plus_1:
    o << "d12 + 1";
    break;
  case dice_e::d12_plus_2:
    o << "d12 + 2";
    break;
  default:
    o << "INVALID DIE";
    break;
  }
  return o;
}
} // namespace savage
