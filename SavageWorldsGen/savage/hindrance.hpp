#pragma once

#include "modifier_bag.hpp"

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
  modifier_bag modifiers; // The modifiers this hindrance applies.
  std::string additionalEffects; // Additional effects of the hindrance which cannot be expressed as modifiers.

  hindrance()
    : name()
    , severity( hindrance_severity_e::major )
    , modifiers()
    , additionalEffects() {}

  hindrance( const std::string& n, hindrance_severity_e s, const std::string& e )
    : name( n )
    , severity( s )
    , modifiers()
    , additionalEffects( e ) {}

  hindrance( const std::string& n, hindrance_severity_e s, modifier_bag m, const std::string& e = "" )
    : name( n )
    , severity( s )
    , modifiers( m )
    , additionalEffects( e ) {}
};

inline std::ostream& operator<<( std::ostream& o, const hindrance& h ) {
  o << h.name << ": ";
  if( h.modifiers.as_vector().empty() ) {
    return o << h.additionalEffects;
  } else if( h.additionalEffects.empty() ) {
    return o << h.modifiers;
  } else {
    return o << h.modifiers << ", " << h.additionalEffects;
  }
}
} // namespace savage
