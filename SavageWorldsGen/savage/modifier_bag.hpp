#pragma once

#include "dice_e.hpp"

#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>

namespace savage {
// The possible targets of modifiers. Their concrete syntax is in the comments.
enum class modifier_target_e : uint8_t {
  attr_agility = 0,     // Agility
  attr_smarts,          // Smarts
  attr_spirit,          // Spirit
  attr_strength,        // Strength
  attr_vigour,          // Vigour
  attriibute_points,    // AttributePoints
  skill_points,         // SkillPoints
  smarts_skill_points,  // SmartsSkillPoints
  edge_points,          // EdgePoints
  toughness,            // Toughness
  parry,                // Parry
  pace,                 // Pace
  run,                  // Run
  charisma,             // Charisma
  fatigue_checks,       // FatigueChecks
  fear_checks,          // FearChecks
  bennies,              // Bennies
  skill,                // Inferred as the target if no other target is recognized.
  count,
  invalid = count
};

std::ostream& operator<<( std::ostream& o, modifier_target_e t );

enum class modifier_action_e : uint8_t {
  set = 0,
  add,
  subtract,
  count,
  invalid = count
};

inline std::ostream& operator<<( std::ostream& o, modifier_action_e a ) {
  switch( a ) {
  case modifier_action_e::set:
    o << '=';
    break;
  case modifier_action_e::add:
    o << '+';
    break;
  case modifier_action_e::subtract:
    o << '-';
    break;
  default:
    o << "INVALID MODIFIER ACTION";
    break;
  }
  return o;
}

// The unit of the operand of a modifier.
enum class modifier_unit_e : uint8_t {
  number = 0,
  die,
  die_type,
  count,
  invalid = count
};

// Checks if a unit u is a valid unit for the operand of an action a on a target t.
bool modifier_unit_matches_target( modifier_target_e t, modifier_action_e a, modifier_unit_e u );

struct modifier {
  modifier_target_e target;
  std::string subTarget;
  modifier_action_e action;
  modifier_unit_e operandUnit;
  int32_t operand;

  modifier()
    : target( modifier_target_e::invalid )
    , subTarget()
    , action( modifier_action_e::invalid )
    , operandUnit( modifier_unit_e::invalid )
    , operand( 0 ) {}
};

inline std::ostream& operator<<( std::ostream& o, const modifier& m ) {
  o << m.target << m.subTarget << ' ' << m.action << ' ';
  switch( m.operandUnit ) {
  case modifier_unit_e::number:
    o << m.operand;
    break;
  case modifier_unit_e::die:
    o << static_cast<dice_e>( m.operand );
    break;
  case modifier_unit_e::die_type:
    o << m.operand << "dt";
    break;
  default:
    o << "(INVALID OPERAND UNIT) " << m.operand;
  }
  return o;
}

class modifier_bag {
  std::vector<modifier> modifiers;

public:
  modifier_bag()
    : modifiers() {}

  void add_modifier( const modifier& m ) { modifiers.push_back( m ); }

  const modifier* get_modifier( modifier_target_e target, const std::string& subTarget = "" ) const {
    for( const modifier& m : modifiers ) {
      if( m.target == target && m.subTarget == subTarget ) {
        return &m;
      }
    }
    return nullptr;
  }

  const std::vector<modifier>& as_vector() const { return modifiers; }
};

inline std::ostream& operator<<( std::ostream& o, const modifier_bag& b ) {
  const std::vector<modifier>& v = b.as_vector();
  if( !v.empty() ) {
    o << v.front();
    for( std::size_t i = 1, iEnd = v.size(); i < iEnd; ++i ) {
      o << ", " << v[i];
    }
  }
  return o;
}

/**
 * Takes a string of modifiers in their concrete syntax and parses them into something usable.
 * Returns true if successful; false if a syntax error is encountered.
 *
 * Examples of concrete syntax:
 * "Agility + 2dt"
 * "Repair + 1dt"
 * "Repair + 2"
 * "Swimming - 2, Climbing - 2, Run = d4"
 */
bool parse_modifiers( const std::string& modifierText, modifier_bag& outModifierBag );

/**
 * Takes a string of modifiers in their concrete syntax and parses them into something usable.
 * Returns a modifier_bag if successful; throws an exception if it fails.
 *
 * THIS SHOULD ONLY BE USED FOR HARDCODED MODIFIERS, AS EXCEPTIONS SHOULD NOT BE THROWN ON
 * USER-SUPPLIED DATA.
 */
inline modifier_bag parse_modifiers( const std::string& modifierText ) {
  modifier_bag modifiers;
  if( parse_modifiers( modifierText, modifiers ) ) {
    return modifiers;
  }
  throw std::runtime_error( "Failed to parse modifiers from: " + modifierText );
}
} // namespace savage
