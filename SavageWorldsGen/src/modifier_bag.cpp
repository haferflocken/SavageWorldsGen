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
  "Toughness",
  "Parry",
  "Pace",
  "Run",
  "Charisma",
  "FatigueChecks",
  "FearChecks",
  "Bennies"
};

const std::size_t num_concrete_modifiers = 15;

std::ostream& ::savage::operator<<( std::ostream& o, modifier_target t ) {
  if( t < modifier_target::skill ) {
    o << concrete_modifier_syntax[static_cast<std::size_t>( t )];
  } else if ( t != modifier_target::skill ) {
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

  modifier_target target = modifier_target::invalid;
  for( std::size_t i = 0, iEnd = num_concrete_modifiers; i < iEnd; ++i ) {
    if( rawTarget == concrete_modifier_syntax[i] ) {
      target = static_cast<modifier_target>( i );
      break;
    }
  }

  // If none of the built-in targets apply, see if it is a skill.
  std::string subTarget = "";
  if( target == modifier_target::invalid ) {
    if( skills_manager::is_skill( rawTarget ) ) {
      target = modifier_target::skill;
      subTarget = rawTarget;
    } else {
      // If the target is not a skill or a built-in target, we cannot parse the modifier.
      return false;
    }
  }

  // Parse the action of the modifier. This is either +, -, or =.
  modifier_action action;
  switch( modifierText[actionChar] ) {
  case '+':
    action = modifier_action::add;
    break;
  case '-':
    action = modifier_action::subtract;
    break;
  case '=':
    action = modifier_action::set;
    break;
  default:
    return false;
  }

  // Parse the operand of the modifier. This is a decimal integer.
  const std::size_t firstCharOfOperand = next_non_whitespace_index( modifierText, actionChar + 1 );
  if( firstCharOfOperand == std::string::npos ) {
    return false;
  }
  const std::size_t lastCharOfOperand = previous_non_whitespace_index( modifierText, modifierText.length() - 1 );
  const bool negativeOperand = modifierText[firstCharOfOperand] == '-';
  if( negativeOperand && action != modifier_action::set ) {
    // Negative operands are only allowed for set actions.
    return false;
  }

  int32_t operand = 0;
  for( std::size_t i = negativeOperand ? firstCharOfOperand + 1 : firstCharOfOperand;
    i <= lastCharOfOperand; ++i ) {
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

  // Place the parsed values in the output parameter.
  outModifier.target = target;
  outModifier.subTarget = subTarget;
  outModifier.action = action;
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
