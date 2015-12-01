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
  std::vector<skill> m_skills;

public:
  skills_bag()
    : m_skills() {
    const std::vector<skill_definition>& skillDefs = skills_manager::get_skill_definitions();
    for( const skill_definition& s : skillDefs ) {
      m_skills.push_back( skill( s ) );
    }
  }

  skills_bag( const std::string& s, dice_e d )
    : skills_bag() {
    set_die( s, d );
  }

  std::vector<skill>& as_vector() { return m_skills; }
  const std::vector<skill>& as_vector() const { return m_skills; }

  dice_e find_die( const std::string& skillName ) const {
    for( const skill& s : m_skills ) {
      if( s.def.name == skillName ) {
        return s.die;
      }
    }
    return dice_e::none;
  }

  void set_die( const std::string& skillName, dice_e die ) {
    for( skill& s : m_skills ) {
      if( s.def.name == skillName ) {
        s.die = die;
        return;
      }
    }
  }

  skills_bag operator+( const skills_bag& rhs ) const {
    skills_bag result;
    for( std::size_t i = 0, iEnd = m_skills.size(); i < iEnd; ++i ) {
      result.m_skills[i].die = std::max( m_skills[i].die, rhs.m_skills[i].die );
    }
    return result;
  }

  bool operator>=( const skills_bag& rhs ) const {
    if( m_skills.size() != rhs.m_skills.size() ) {
      throw savage_error( "Skill list changed during character generation." );
    }
    for( std::size_t i = 0, iEnd = m_skills.size(); i < iEnd; ++i ) {
      if( m_skills[i].def.name != rhs.m_skills[i].def.name ) {
        throw savage_error( "Skill list changed during character generation." );
      }
      if( m_skills[i].die < rhs.m_skills[i].die ) {
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

inline std::vector<skill*> only_skills_below( const std::vector<skill*>& skills, dice_e threshold ) {
  std::vector<skill*> result;
  for( skill* s : skills ) {
    if( s->die < threshold ) {
      result.push_back( s );
    }
  }
  return result;
}

inline std::vector<skill*> only_skills_below( std::vector<skill>& skills, dice_e threshold ) {
  std::vector<skill*> result;
  for( skill& s : skills ) {
    if( s.die < threshold ) {
      result.push_back( &s );
    }
  }
  return result;
}

inline std::vector<skill*> only_skills_of_attribute( const std::vector<skill*>& skills, attributes_e attribute ) {
  std::vector<skill*> result;
  for( skill* s : skills ) {
    if( s->def.attribute == attribute ) {
      result.push_back( s );
    }
  }
  return result;
}

inline std::vector<skill*> only_skills_of_attribute( std::vector<skill>& skills, attributes_e attribute ) {
  std::vector<skill*> result;
  for( skill& s : skills ) {
    if( s.def.attribute == attribute ) {
      result.push_back( &s );
    }
  }
  return result;
}
} // namespace savage
