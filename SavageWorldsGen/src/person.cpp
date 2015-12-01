#include "../savage/person.hpp"

using namespace savage;

namespace {
void apply_action_die( dice_e& lhs, modifier_action_e action, modifier_unit_e unit, int32_t operand ) {
  switch( unit ) {
  case modifier_unit_e::die:
  case modifier_unit_e::die_type: {
    // The type-checker in the modifier parser has already checked that the die and die_type operands use the right action.
    switch( action ) {
    case modifier_action_e::set:
      lhs = static_cast<dice_e>( operand );
      break;
    case modifier_action_e::add:
      if( lhs < dice_e::d12 ) {
        lhs = lhs + operand;
      }
      break;
    case modifier_action_e::subtract:
      if( lhs > dice_e::d4 ) {
        lhs = lhs - operand;
      }
      break;
    default:
      throw savage_error( "INVALID MODIFIER ACTION IN PERSON::APPLY_MODIFIER." );
    }

    break;
  }
  case modifier_unit_e::number:
    // TODO(jwerner) numeric modifiers to dice
    break;

  default:
    throw savage_error( "INVALID MODIFIER UNIT IN APPLY_ACTION_DIE." );
  }
}

void unapply_action_die( dice_e& lhs, modifier_action_e action, modifier_unit_e unit, int32_t operand ) {
  switch( unit ) {
  case modifier_unit_e::die:
  case modifier_unit_e::die_type: {
    switch( action ) {
    case modifier_action_e::set:
      // We can't undo a set, but it isn't an error to try to.
      break;
    case modifier_action_e::add:
      if( lhs == dice_e::none ) {
        throw savage_error( "CANNOT UNAPPLY ADD WHEN DIE IS ALREADY AT MIN IN UNAPPLY_ACTION_DIE." );
      }
      lhs = lhs - operand;
      break;
    case modifier_action_e::subtract:
      if( lhs >= dice_e::d12 ) {
        throw savage_error( "CANNOT UNAPPLY SUBTRACT WHEN DIE IS ALREADY AT MAX IN UNAPPLY_ACTION_DIE." );
      }
      lhs = lhs + operand;
      break;
    default:
      throw savage_error( "INVALID MODIFIER ACTION IN UNAPPLY_ACTION_DIE." );
    }

    break;
  }
  case modifier_unit_e::number:
    // TODO(jwerner) numeric modifiers to dice
    break;

  default:
    throw savage_error( "INVALID MODIFIER UNIT IN UNAPPLY_ACTIOND_DIE." );
  }
}

template<typename lhs_t>
void apply_action_numeric( lhs_t& lhs, modifier_action_e action, int32_t operand ) {
  switch( action ) {
  case modifier_action_e::set:
    lhs = operand;
    break;
  case modifier_action_e::add:
    lhs += operand;
    break;
  case modifier_action_e::subtract:
    lhs -= operand;
    break;
  default:
    throw savage_error( "INVALID MODIFIER ACTION IN APPLY_ACTION_NUMERIC." );
  }
}

template<typename lhs_t>
void unapply_action_numeric( lhs_t& lhs, modifier_action_e action, int32_t operand ) {
  switch( action ) {
  case modifier_action_e::set:
    // We can't unapply a set, but it isn't an error to try.
    break;
  case modifier_action_e::add:
    lhs -= operand;
    break;
  case modifier_action_e::subtract:
    lhs += operand;
    break;
  default:
    throw savage_error( "INVALID MODIFIER ACTION IN UNAPPLY_ACTION_NUMERIC." );
  }
}
} // anonymous namespace

void person::apply_modifier( const modifier& m ) {
  switch( m.target ) {
  case modifier_target_e::attr_agility:
  case modifier_target_e::attr_smarts:
  case modifier_target_e::attr_spirit:
  case modifier_target_e::attr_strength:
  case modifier_target_e::attr_vigour: {
    const std::size_t attributeIndex = static_cast<std::size_t>( m.target ) - static_cast<std::size_t>( modifier_target_e::attr_agility );
    dice_e& attributeDie = attributes.as_array()[attributeIndex];
    apply_action_die( attributeDie, m.action, m.operandUnit, m.operand );
    break;
  }
  case modifier_target_e::attriibute_points:
  case modifier_target_e::skill_points:
  case modifier_target_e::smarts_skill_points:
  case modifier_target_e::edge_points:
    // These are special-cased within character initialization and do not apply outside of that.
    break;

  case modifier_target_e::toughness:
    apply_action_numeric( m_toughness, m.action, m.operand );
    break;
  case modifier_target_e::parry:
    apply_action_numeric( m_parryModifier, m.action, m.operand );
    break;
  case modifier_target_e::pace:
    apply_action_numeric( m_pace, m.action, m.operand );
    break;
  case modifier_target_e::run:
    apply_action_die( m_runDie, m.action, m.operandUnit, m.operand );
    break;
  case modifier_target_e::charisma:
    apply_action_numeric( m_charisma, m.action, m.operand );
    break;
  case modifier_target_e::fatigue_checks:
    apply_action_numeric( m_fatigueChecks, m.action, m.operand );
    break;
  case modifier_target_e::fear_checks:
    apply_action_numeric( m_fearChecks, m.action, m.operand );
    break;
  case modifier_target_e::bennies:
    apply_action_numeric( m_bennies, m.action, m.operand );
    break;
  case modifier_target_e::skill:
    // TODO(jwerner)
    break;
  default:
    throw savage_error( "INVALID MODIFIER TARGET IN PERSON::APPLY_MODIFIER." );
  }
}

void person::unapply_modifier( const modifier& m ) {
  switch( m.target ) {
  case modifier_target_e::attr_agility:
  case modifier_target_e::attr_smarts:
  case modifier_target_e::attr_spirit:
  case modifier_target_e::attr_strength:
  case modifier_target_e::attr_vigour: {
    const std::size_t attributeIndex = static_cast<std::size_t>( m.target ) - static_cast<std::size_t>( modifier_target_e::attr_agility );
    dice_e& attributeDie = attributes.as_array()[attributeIndex];
    unapply_action_die( attributeDie, m.action, m.operandUnit, m.operand );
    break;
  }
  case modifier_target_e::attriibute_points:
  case modifier_target_e::skill_points:
  case modifier_target_e::smarts_skill_points:
  case modifier_target_e::edge_points:
    // These are special-cased within character initialization and do not apply outside of that.
    break;

  case modifier_target_e::toughness:
    unapply_action_numeric( m_toughness, m.action, m.operand );
    break;
  case modifier_target_e::parry:
    unapply_action_numeric( m_parryModifier, m.action, m.operand );
    break;
  case modifier_target_e::pace:
    unapply_action_numeric( m_pace, m.action, m.operand );
    break;
  case modifier_target_e::run:
    unapply_action_die( m_runDie, m.action, m.operandUnit, m.operand );
    break;
  case modifier_target_e::charisma:
    unapply_action_numeric( m_charisma, m.action, m.operand );
    break;
  case modifier_target_e::fatigue_checks:
    unapply_action_numeric( m_fatigueChecks, m.action, m.operand );
    break;
  case modifier_target_e::fear_checks:
    unapply_action_numeric( m_fearChecks, m.action, m.operand );
    break;
  case modifier_target_e::bennies:
    unapply_action_numeric( m_bennies, m.action, m.operand );
    break;
  case modifier_target_e::skill:
    // TODO(jwerner)
    break;
  default:
    throw savage_error( "INVALID MODIFIER TARGET IN PERSON::APPLY_MODIFIER." );
  }
}


void person::add_edge( const edge& e ) {
  // If this edge replaces another edge, move that edge to the replaced edges list and revert its modifiers.
  if( !e.replacesEdge.empty() ) {
    std::vector<edge>::iterator replaced, edgesEnd;
    for( replaced = m_edges.begin(), edgesEnd = m_edges.end(); replaced != edgesEnd; ++replaced ) {
      if( replaced->name == e.replacesEdge ) {
        break;
      }
    }
    if( replaced == edgesEnd ) {
      throw savage_error( "FORGOT TO CHECK MEETS_REQUIREMENTS BEFORE CALLING ADD_EDGE." );
    }

    for( const modifier& m : replaced->modifiers.as_vector() ) {
      unapply_modifier( m );
    }

    m_replacedEdges.push_back( replaced->name );
    m_edges.erase( replaced );
  }

  // Add the new edge and apply its modifiers.
  m_edges.push_back( e );
  for( const modifier& m : e.modifiers.as_vector() ) {
    apply_modifier( m );
  }
}

void person::add_hindrance( const hindrance& h ) {
  // Add the hindrance to the hindrance list and apply its modifiers.
  m_hindrances.push_back( h );
  for( const modifier& m : h.modifiers.as_vector() ) {
    apply_modifier( m );
  }
}
