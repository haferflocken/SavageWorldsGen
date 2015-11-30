#include "../savage/modifier_bag.hpp"

#include "../savage/managers/skills_manager.hpp"

using namespace savage;

const std::string concrete_modifier_syntax[] = {
  "Agility",
  "Smarts",
  "Spirit",
  "Strength",
  "Vigour",
  "AttributePoints",
  "SkillPoints",
  "SmartsSkillPoints",
  "EdgePoints", 
  "Toughness",
  "Parry",
  "Pace",
  "Run",
  "Charisma",
  "FatigueChecks",
  "FearChecks",
  "Bennies"
};

const std::size_t num_concrete_modifiers = 17;

std::ostream& ::savage::operator<<( std::ostream& o, modifier_target_e t ) {
  if( t < modifier_target_e::skill ) {
    o << concrete_modifier_syntax[static_cast<std::size_t>( t )];
  } else if ( t != modifier_target_e::skill ) {
    o << "INVALID MODIFIER TARGET";
  }
  return o;
}

namespace {
std::size_t next_non_whitespace_index( const std::string& text, std::size_t offset ) {
  for( std::size_t i = offset, iEnd = text.length(); i < iEnd; ++i ) {
    if( text[i] != ' ' ) {
      return i;
    }
  }
  return std::string::npos;
}

std::size_t previous_non_whitespace_index( const std::string& text, std::size_t offset ) {
  for( int32_t i = static_cast<int32_t>( offset ); i >= 0; --i ) {
    if( text[i] != ' ' ) {
      return static_cast<std::size_t>( i );
    }
  }
  return std::string::npos;
}

std::size_t next_action_index( const std::string& text, std::size_t offset ) {
  std::size_t i, iEnd;
  for( i = offset, iEnd = text.length(); i < iEnd; ++i ) {
    if( text[i] == '+' || text[i] == '-' || text[i] == '=' ) {
      return i;
    }
  }
  return std::string::npos;
}

bool parse_modifier( const std::string& modifierText, modifier& outModifier ) {
  // Parse the target of the modifier.
  const std::size_t firstCharOfTarget = next_non_whitespace_index( modifierText, 0 );
  if( firstCharOfTarget == std::string::npos ) {
    return false;
  }
  const std::size_t actionChar = next_action_index( modifierText, firstCharOfTarget );
  if( actionChar == std::string::npos || actionChar == firstCharOfTarget ) {
    return false;
  }
  const std::size_t lastCharOfTarget = previous_non_whitespace_index( modifierText, actionChar - 1 );
  if( lastCharOfTarget == std::string::npos ) {
    return false;
  }
  const std::string rawTarget = modifierText.substr( firstCharOfTarget, lastCharOfTarget - firstCharOfTarget + 1 );

  modifier_target_e target = modifier_target_e::invalid;
  for( std::size_t i = 0, iEnd = num_concrete_modifiers; i < iEnd; ++i ) {
    if( rawTarget == concrete_modifier_syntax[i] ) {
      target = static_cast<modifier_target_e>( i );
      break;
    }
  }

  // If none of the built-in targets apply, see if it is a skill.
  std::string subTarget = "";
  if( target == modifier_target_e::invalid ) {
    if( skills_manager::is_skill( rawTarget ) ) {
      target = modifier_target_e::skill;
      subTarget = rawTarget;
    } else {
      // If the target is not a skill or a built-in target, we cannot parse the modifier.
      return false;
    }
  }

  // Parse the action of the modifier. This is either +, -, or =.
  modifier_action_e action;
  switch( modifierText[actionChar] ) {
  case '+':
    action = modifier_action_e::add;
    break;
  case '-':
    action = modifier_action_e::subtract;
    break;
  case '=':
    action = modifier_action_e::set;
    break;
  default:
    return false;
  }

  // Parse the operand of the modifier. This is a decimal integer, a die, or a decimal integer followed by "dt".
  std::size_t firstCharOfOperand = next_non_whitespace_index( modifierText, actionChar + 1 );
  if( firstCharOfOperand == std::string::npos ) {
    return false;
  }
  std::size_t lastCharOfOperand = previous_non_whitespace_index( modifierText, modifierText.length() - 1 );
  const bool negativeOperand = modifierText[firstCharOfOperand] == '-';
  if( negativeOperand && action != modifier_action_e::set ) {
    // Negative operands are only allowed for set actions.
    return false;
  }
  if( negativeOperand ) {
    firstCharOfOperand += 1;
  }

  // Extract the unit of the operand.
  modifier_unit_e operandUnit;
  if( modifierText[firstCharOfOperand] == 'd' ) {
    operandUnit = modifier_unit_e::die;
    ++firstCharOfOperand;
  } else if( modifierText[lastCharOfOperand - 1] == 'd' && modifierText[lastCharOfOperand] == 't' ) {
    operandUnit = modifier_unit_e::die_type;
    lastCharOfOperand -= 2;
  } else {
    operandUnit = modifier_unit_e::number;
  }

  // Parse the numeric part of the operand.
  int32_t operand = 0;
  switch( operandUnit ) {
  case modifier_unit_e::die: {
    if( negativeOperand ) {
      return false;
    }

    const std::string dieOperandText = modifierText.substr( firstCharOfOperand, lastCharOfOperand - firstCharOfOperand + 1 );
    if( dieOperandText == "4" ) {
      operand = static_cast<int32_t>( dice_e::d4 );
    } else if( dieOperandText == "6" ) {
      operand = static_cast<int32_t>( dice_e::d6 );
    } else if( dieOperandText == "8" ) {
      operand = static_cast<int32_t>( dice_e::d8 );
    } else if( dieOperandText == "10" ) {
      operand = static_cast<int32_t>( dice_e::d10 );
    } else if( dieOperandText == "12" ) {
      operand = static_cast<int32_t>( dice_e::d12 );
    } else if( dieOperandText == "12+1" ) {
      operand = static_cast<int32_t>( dice_e::d12_plus_1 );
    } else if( dieOperandText == "12+2" ) {
      operand = static_cast<int32_t>( dice_e::d12_plus_2 );
    } else {
      return false;
    }
    break;
  }
  case modifier_unit_e::die_type:
  case modifier_unit_e::number: {
    for( std::size_t i = firstCharOfOperand; i <= lastCharOfOperand; ++i ) {
      if( modifierText[i] < '0' || modifierText[i] > '9' ) {
        return false;
      }
      int32_t val = modifierText[i] - '0';
      operand *= 10;
      operand += val;
    }
    if( negativeOperand ) {
      operand *= -1;
    }
    break;
  }
  default:
    throw std::runtime_error( "INVALID CODE BRANCH REACHED." );
  }

  // Place the parsed values in the output parameter.
  outModifier.target = target;
  outModifier.subTarget = subTarget;
  outModifier.action = action;
  outModifier.operandUnit = operandUnit;
  outModifier.operand = operand;
  return true;
}
} // anonymous namespace

bool ::savage::parse_modifiers( const std::string& modifierText, modifier_bag& outModifierBag ) {
  // Each modifier is separated by a comma.
  for( std::size_t firstChar = 0, length = modifierText.length(); firstChar < length; ) {
    std::size_t commaIndex = modifierText.find_first_of( ',', firstChar );
    if( commaIndex == std::string::npos ) {
      commaIndex = length;
    }

    std::string rawModifier = modifierText.substr( firstChar, commaIndex - firstChar );
    modifier parsedModifier;
    if( parse_modifier( rawModifier, parsedModifier ) ) {
      outModifierBag.add_modifier( parsedModifier );
    } else {
      return false;
    }

    firstChar = commaIndex + 1;
  }
  return true;
}
