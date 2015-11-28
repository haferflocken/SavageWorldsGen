#pragma once

#include "dice_e.hpp"
#include "managers/skills_manager.hpp"

#include <string>
#include <vector>

namespace savage {
/**
 * A skill is a name and a die value.
 */
struct skill {
  skill_definition def;
  dice_e die;

  skill()
    : def()
    , die( dice_e::none ) {}

  explicit skill( const skill_definition& def_ )
    : def( def_ )
    , die( dice_e::none ) {}
};

/**
 * The skills of a person in Savage Worlds.
 */
class skills_bag {
  std::vector<skill> skills;

public:
  skills_bag()
    : skills() {
    const std::vector<skill_definition>& skillDefs = skills_manager::get_skill_definitions();
    for( const skill_definition& s : skillDefs ) {
      skills.push_back( skill( s ) );
    }
  }

  skills_bag( const std::string& s, dice_e d )
    : skills_bag() {
    set_die( s, d );
  }

  std::vector<skill>& as_vector() { return skills; }
  const std::vector<skill>& as_vector() const { return skills; }

  dice_e find_die( const std::string& skillName ) const {
    for( const skill& s : skills ) {
      if( s.def.name == skillName ) {
        return s.die;
      }
    }
    return dice_e::none;
  }

  void set_die( const std::string& skillName, dice_e die ) {
    for( skill& s : skills ) {
      if( s.def.name == skillName ) {
        s.die = die;
        return;
      }
    }
  }

  std::vector<skill*> get_skills_below( dice_e threshold ) {
    std::vector<skill*> result;
    for( skill& s : skills ) {
      if( s.die < threshold ) {
        result.push_back( &s );
      }
    }
    return result;
  }

  skills_bag operator+( const skills_bag& rhs ) const {
    skills_bag result;
    for( std::size_t i = 0, iEnd = skills.size(); i < iEnd; ++i ) {
      result.skills[i].die = std::max( skills[i].die, rhs.skills[i].die );
    }
    return result;
  }

  bool operator>=( const skills_bag& rhs ) const {
    if( skills.size() != rhs.skills.size() ) {
      throw std::runtime_error( "Skill list changed during character generation." );
    }
    for( std::size_t i = 0, iEnd = skills.size(); i < iEnd; ++i ) {
      if( skills[i].def.name != rhs.skills[i].def.name ) {
        throw std::runtime_error( "Skill list changed during character generation." );
      }
      if( skills[i].die < rhs.skills[i].die ) {
        return false;
      }
    }
    return true;
  }
};

inline std::ostream& operator<<( std::ostream& o, const skills_bag& bag ) {
  const std::vector<skill>& skills = bag.as_vector();
  if( skills.empty() ) {
    return o;
  }
  
  o << skills.front().def.name << ": " << skills.front().die;
  for( auto i = skills.begin() + 1, iEnd = skills.end(); i != iEnd; ++i ) {
    o << std::endl << i->def.name << ": " << i->die;
  }
  return o;
}
} // namespace savage
