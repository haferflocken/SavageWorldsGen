#pragma once

#include <ostream>

namespace savage {
/**
 * The rank of a character in Savage Worlds.
 */
enum class rank_e {
  novice = 0,
  seasoned,
  veteran,
  heroic,
  legendary,
  count,
  invalid = count
};

inline std::ostream& operator<<( std::ostream& o, rank_e r ) {
  switch( r ) {
  case savage::rank_e::novice:
    o << "Novice";
    break;
  case savage::rank_e::seasoned:
    o << "Seasoned";
    break;
  case savage::rank_e::veteran:
    o << "Veteran";
    break;
  case savage::rank_e::heroic:
    o << "Heroic";
    break;
  case savage::rank_e::legendary:
    o << "Legendary";
    break;
  default:
    o << "INVALID RANK";
    break;
  }
  return o;
}
} // namespace savage
