#pragma once

#include <string>

namespace savage {
/**
 * The severity of a hindrance.
 */
enum class hindrance_severity_e {
  major,
  minor
};

/**
 * A hindrance of a person in Savage Worlds.
 */
struct hindrance {
  std::string name; // The name of this hindrance.
  hindrance_severity_e severity; // The severity of the hindrance.
  std::string effects; // The effect of the hindrance.

  hindrance()
    : name()
    , severity( hindrance_severity_e::major )
    , effects() {}

  hindrance( const std::string& n, hindrance_severity_e s, const std::string& e )
    : name( n )
    , severity( s )
    , effects( e ) {}
};
} // namespace savage
