#pragma once

#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>

namespace savage {
// The possible targets of modifiers. Their concrete syntax is in the comments.
enum class modifier_target : uint8_t {
  attr_agility = 0,     // Agility
  attr_smarts,          // Smarts
  attr_spirit,          // Spirit
  attr_strength,        // Strength
  attr_vigour,          // Vigour
  attriibute_points,    // AttributePoints
  skill_points,         // SkillPoints
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

std::ostream& operator<<( std::ostream& o, modifier_target t );

enum class modifier_action {
  set = 0,
  add,
  subtract,
  count,
  invalid = count
};

inline std::ostream& operator<<( std::ostream& o, modifier_action a ) {
  switch( a ) {
  case modifier_action::set:
    o << '=';
    break;
  case modifier_action::add:
    o << '+';
    break;
  case modifier_action::subtract:
    o << '-';
    break;
  default:
    o << "INVALID MODIFIER ACTION";
    break;
  }
  return o;
}

struct modifier {
  modifier_target target;
  std::string subTarget;
  modifier_action action;
  int32_t operand;

  modifier()
    : target( modifier_target::invalid )
    , subTarget()
    , action( modifier_action::invalid )
    , operand( 0 ) {}
};

class modifier_bag {
  std::vector<modifier> modifiers;

public:
  modifier_bag()
    : modifiers() {}

  void add_modifier( const modifier& m ) { modifiers.push_back( m ); }

  const modifier* get_modifier( modifier_target target, const std::string& subTarget = "" ) const {
    for( const modifier& m : modifiers ) {
      if( m.target == target && m.subTarget == subTarget ) {
        return &m;
      }
    }
    return nullptr;
  }

  const std::vector<modifier>& as_vector() const { return modifiers; }
};

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
} // namespace savage
