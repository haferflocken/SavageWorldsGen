#pragma once

#include "dice_e.hpp"
#include "modifier_bag.hpp"

#include <algorithm>
#include <vector>

namespace savage {
enum class attributes_e : uint8_t {
  agility = 0,
  smarts,
  spirit,
  strength,
  vigour,
  count,
  invalid = count
};

/**
 * The attributes of a character in Savage Worlds.
 */
struct attributes_bag {
  dice_e agilityDie;
  dice_e smartsDie;
  dice_e spiritDie;
  dice_e strengthDie;
  dice_e vigourDie;

  attributes_bag()
    : agilityDie( dice_e::d4 )
    , smartsDie( dice_e::d4 )
    , spiritDie( dice_e::d4 )
    , strengthDie( dice_e::d4 )
    , vigourDie( dice_e::d4 ) {}

  attributes_bag( dice_e ag, dice_e sm, dice_e sp, dice_e st, dice_e vi )
    : agilityDie( ag )
    , smartsDie( sm )
    , spiritDie( sp )
    , strengthDie( st )
    , vigourDie( vi ) {}

  attributes_bag( attributes_e attrib, dice_e d )
    : attributes_bag() {
    as_array()[static_cast<std::size_t>( attrib )] = d;
  }

  dice_e* as_array() { return reinterpret_cast<dice_e*>( this ); }
  const dice_e* as_array() const { return reinterpret_cast<const dice_e*>( this ); }

  dice_e& get( attributes_e a ) {
    return as_array()[static_cast<std::size_t>( a )];
  }

  dice_e get( attributes_e a ) const {
    return as_array()[static_cast<std::size_t>( a )];
  }

  std::vector<attributes_e> get_attributes_below( dice_e threshold ) const {
    std::vector<attributes_e> result;
    for( std::size_t i = 0; i < 5; ++i ) {
      if( as_array()[i] < threshold ) {
        result.push_back( static_cast<attributes_e>( i ) );
      }
    }
    return result;
  }

  attributes_bag operator+( const attributes_bag& rhs ) const {
    return attributes_bag(
      std::max( agilityDie, rhs.agilityDie ),
      std::max( smartsDie, rhs.smartsDie ),
      std::max( spiritDie, rhs.spiritDie ),
      std::max( strengthDie, rhs.strengthDie ),
      std::max( vigourDie, rhs.vigourDie )
      );
  }

  bool operator>=( const attributes_bag& rhs ) const {
    return agilityDie >= rhs.agilityDie && smartsDie >= rhs.smartsDie
      && spiritDie >= rhs.spiritDie && strengthDie >= rhs.strengthDie && vigourDie >= rhs.vigourDie;
  }
};

inline attributes_bag agility_bag( dice_e d ) {
  return attributes_bag( attributes_e::agility, d );
}

inline attributes_bag smarts_bag( dice_e d ) {
  return attributes_bag( attributes_e::smarts, d );
}

inline attributes_bag spirit_bag( dice_e d ) {
  return attributes_bag( attributes_e::spirit, d );
}

inline attributes_bag strength_bag( dice_e d ) {
  return attributes_bag( attributes_e::strength, d );
}

inline attributes_bag vigour_bag( dice_e d ) {
  return attributes_bag( attributes_e::vigour, d );
}

inline std::ostream& operator<<( std::ostream& o, const attributes_bag& t ) {
  o << "Agility: " << t.agilityDie << std::endl;
  o << "Smarts: " << t.smartsDie << std::endl;
  o << "Spirit: " << t.spiritDie << std::endl;
  o << "Strength: " << t.strengthDie << std::endl;
  o << "Vigour: " << t.vigourDie;
  return o;
}

/**
 * A modifier_bag_source for modifiers to attributes applied at character initialization or at level up.
 */
struct attribute_modifier_source : public modifier_bag_source {
  modifier_bag modifiers;

  virtual const modifier_bag& get_modifiers() const override { return modifiers; }
};
} // namespace savage
