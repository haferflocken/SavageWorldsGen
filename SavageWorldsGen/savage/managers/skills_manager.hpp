#pragma once

#include "../attributes_bag.hpp"

#include <string>
#include <vector>

namespace savage {
struct skill_definition {
  std::string name;
  attributes_e attribute;

  skill_definition()
    : name()
    , attribute( attributes_e::agility ) {}

  skill_definition( const std::string& n, attributes_e a )
    : name( n )
    , attribute( a ) {}
};

class skills_manager {
  static std::vector<skill_definition>* s_skillDefinitions;

  skills_manager() {}
  ~skills_manager() {}

public:
  static const std::vector<skill_definition>& get_skill_definitions();
  
  static void set_skill_definitions( const std::vector<skill_definition>& skillDefinitions ) {
    *s_skillDefinitions = skillDefinitions;
  }
};
} // namespace savage
