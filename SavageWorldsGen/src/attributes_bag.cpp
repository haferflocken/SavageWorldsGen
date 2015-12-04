#include "../savage/attributes_bag.hpp"

using namespace savage;

attribute_modifier_source* inc_agility = nullptr;
attribute_modifier_source* inc_smarts = nullptr;
attribute_modifier_source* inc_spirit = nullptr;
attribute_modifier_source* inc_strength = nullptr;
attribute_modifier_source* inc_vigour = nullptr;

const attribute_modifier_source* ::savage::get_inc_attribute( attributes_e attribute ) {
  switch( attribute ) {
  case attributes_e::agility: {
    if( inc_agility == nullptr ) {
      inc_agility = new attribute_modifier_source( parse_modifiers( "Agility + 1dt" ) );
    }
    return inc_agility;
  }
  case attributes_e::smarts: {
    if( inc_smarts == nullptr ) {
      inc_smarts = new attribute_modifier_source( parse_modifiers( "Smarts + 1dt" ) );
    }
    return inc_smarts;
  }
  case attributes_e::spirit: {
    if( inc_spirit == nullptr ) {
      inc_spirit = new attribute_modifier_source( parse_modifiers( "Spirit + 1dt" ) );
    }
    return inc_spirit;
  }
  case attributes_e::strength: {
    if( inc_strength == nullptr ) {
      inc_strength = new attribute_modifier_source( parse_modifiers( "Strength + 1dt" ) );
    }
    return inc_strength;
  }
  case attributes_e::vigour: {
    if( inc_vigour == nullptr ) {
      inc_vigour = new attribute_modifier_source( parse_modifiers( "Vigour + 1dt" ) );
    }
    return inc_vigour;
  }
  default:
    throw savage_error( "INVALID ATTRIBUTE IN GET_INC_ATTRIBUTE." );
  }
}
