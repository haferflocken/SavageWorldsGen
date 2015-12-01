#include "../savage/person.hpp"

#include "../savage/managers/edge_manager.hpp"
#include "../savage/managers/hindrance_manager.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

std::vector<std::vector<std::string>> parse_main_arguments( int argCount, char** args ) {
  std::vector<std::vector<std::string>> completeGroups;
  for( int i = 1; i < argCount; ) {
    if( args[i][0] == '-' ) {
      // If we encounter a dash at the start of a raw argument, capture that argument and the arguments
      // following it until the next argument beginning with a dash.
      std::vector<std::string> currentGroup;
      currentGroup.push_back( args[i] );
      for( ++i; i < argCount; ++i ) {
        if( args[i][0] == '-' ) {
          break;
        } else {
          currentGroup.push_back( args[i] );
        }
      }
      completeGroups.push_back( currentGroup );
    } else {
      // Arguments that do not start with a dash in the outer loop are unrecognized.
      std::cout << "Invalid argument " << args[i] << std::endl;
      ++i;
    }
  }
  return completeGroups;
}

void handle_override_world_arg( const std::vector<std::string>& argGroup ) {
  // -ow indicates a custom world file. It should be followed by a file name.
  if( argGroup.size() == 2 ) {
    // TODO(jwerner) implement -ow
    std::cout << "-ow not yet supported, so " << argGroup[1] << " was not used." << std::endl;
  } else {
    std::cout << "Invalid -ow syntax. -ow should be followed by a single parameter, a file path to a JSON file with the skills, edges, and hindrances of the world." << std::endl;
  }
}

void handle_mount_world_arg( const std::vector<std::string>& argGroup ) {
  // -mw indicates a custom world file. It should be followed by a file name.
  if( argGroup.size() == 2 ) {
    // TODO(jwerner) implement -mw
    std::cout << "-mw not yet supported, so " << argGroup[1] << " was not used." << std::endl;
  } else {
    std::cout << "Invalid -mw syntax. -mw should be followed by a single parameter, a file path to a JSON file with skills, edges, and hindrances to supplement the default world." << std::endl;
  }
}

void random_initialize( savage::person& p );
void random_level_up( savage::person& p, std::size_t targetLevel );

int main( int argCount, char** args ) {
  // Gather the arguments.
  const std::vector<std::vector<std::string>> parsedArgGroups = parse_main_arguments( argCount, args );

  std::string nameArg = "Unnamed Character";
  std::size_t levelArg = 1;

  // Gather the information from the arguments.
  for( const std::vector<std::string>& argGroup : parsedArgGroups ) {
    const std::string& groupType = argGroup.front();
    if( groupType == "-ow" ) {
      // The -ow parameter ("override world") overrides the default world entirely, replacing it with a custom one.
      handle_override_world_arg( argGroup );
    } else if( groupType == "-mw" ) {
      // The -mw parameter ("mount world") mounts a world on top of the default world.
      handle_mount_world_arg( argGroup );
    } else if( groupType == "-name" ) {
      // The -name parameter sets the name of the generated character.
      if( argGroup.size() >= 2 ) {
        nameArg = argGroup[1];
        for( std::size_t i = 2, iEnd = argGroup.size(); i < iEnd; ++i ) {
          nameArg += " ";
          nameArg += argGroup[i];
        }
      } else {
        std::cout << "Invalid -name syntax. -name should be followed by the name of the character." << std::endl;
      }
    } else if( groupType == "-level" ) {
      // If -level parameter sets the level of the generated character.
      if( argGroup.size() == 2 ) {
        long levelLong = std::strtol( argGroup[1].data(), nullptr, 10 );
        if( levelLong > 0 ) {
          levelArg = levelLong;
        } else {
          std::cout << "The parameter to -level must be greater than zero." << std::endl;
        }
      } else {
        std::cout << "Invalid -level syntax. -level should be followed by a single parameter, the level of the character as a positive decimal integer." << std::endl;
      }
    } else {
      // No match indicates an invalid argument.
      std::cout << "Invalid argument " << groupType << std::endl;
    }
  }

  // Generate the character.
  srand( static_cast<unsigned int>( time( nullptr ) ) );
  savage::person generatedPerson;
  generatedPerson.name = nameArg;
  random_initialize( generatedPerson );
  random_level_up( generatedPerson, levelArg );

  // Output the generated character.
  std::cout << std::endl;
  std::cout << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV" << std::endl;
  std::cout << std::endl;

  std::cout << generatedPerson.name << " [Level " << generatedPerson.level() << "] ";
  std::cout << "[" << generatedPerson.rank() << "]" << std::endl;
  std::cout << "--------------------------------------------------------------------------------" << std::endl;
  std::cout << generatedPerson.attributes << std::endl;
  std::cout << "--------------------------------------------------------------------------------" << std::endl;
  std::cout << generatedPerson.skills << std::endl;
  std::cout << "--------------------------------------------------------------------------------" << std::endl;
  for( const savage::hindrance& h : generatedPerson.get_hindrances() ) {
    std::cout << h << std::endl;
  }
  std::cout << "--------------------------------------------------------------------------------" << std::endl;
  for( const savage::edge& e : generatedPerson.get_edges() ) {
    std::cout << e << std::endl;
  }

  std::cout << std::endl;
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
  std::cout << std::endl;

  return 0;
}

namespace {
// Assumes target is a number.
void apply_hindrances_modifier( const std::vector<savage::hindrance>& hindrances, savage::modifier_target_e target, std::size_t& val ) {
  using namespace savage;

  for( const hindrance& h : hindrances ) {
    const modifier* m = h.modifiers.get_modifier( target );
    if( m != nullptr ) {
      switch( m->action ) {
      case modifier_action_e::set:
        val = m->operand;
        break;
      case modifier_action_e::add:
        val += m->operand;
        break;
      case modifier_action_e::subtract:
        val -= m->operand;
        break;
      }
    }
  }
}
} // anonymous namespace

void random_initialize( savage::person& p ) {
  using namespace savage;
  
  // Randomize hindrances. A person can have two minor and a major at most.
  // A major hindrance grants 2 hindrance points, and a minor hindrance grants 1.
  bool hasMajorHindrance = ( rand() % 2 ) == 1;
  int numMinorHindrances = rand() % 3;
  std::size_t hindrancePoints = 0;

  if( hasMajorHindrance ) {
    hindrancePoints += 2;
    p.add_hindrance( hindrance_manager::random_major_hindrance() );
  }
  if( numMinorHindrances > 0 ) {
    ++hindrancePoints;
    const hindrance* minorHindranceA = &hindrance_manager::random_minor_hindrance();
    p.add_hindrance( *minorHindranceA );
    if( numMinorHindrances > 1 ) {
      ++hindrancePoints;
      const hindrance* minorHindranceB;
      do {
        minorHindranceB = &hindrance_manager::random_minor_hindrance();
      } while( minorHindranceA == minorHindranceB );
      p.add_hindrance( *minorHindranceB );
    }
  }

  // Determine the number of points we have to spend on attributes.
  std::size_t attributePoints = 5;
  // Attribute points can be affected by hindrances.
  apply_hindrances_modifier( p.get_hindrances(), modifier_target_e::attriibute_points, attributePoints );
  // Randomly spend hindrance points on attributes.
  while( hindrancePoints >= 2 ) {
    if( ( rand() % 2 ) == 1 ) {
      hindrancePoints -= 2;
      ++attributePoints;
    } else {
      break;
    }
  }
  // Spend attribute points randomly.
  while( attributePoints > 0 ) {
    // Treat the attributes bag as an array of dice to make it easy to work with.
    dice_e* attributeArray = p.attributes.as_array();

    // Find an attribute that can be increased.
    std::vector<attributes_e> increasableAttributes = p.attributes.get_attributes_below( dice_e::d12 );
    std::size_t attributeIndex = static_cast<std::size_t>( increasableAttributes[rand() % increasableAttributes.size()] );

    // Increase the attribute by one die type.
    attributeArray[attributeIndex] = attributeArray[attributeIndex] + 1;
    --attributePoints;
  }
  // Hindrances can modify the die types of attributes.
  for( const hindrance& h : p.get_hindrances() ) {
    for( std::size_t i = static_cast<std::size_t>( modifier_target_e::attr_agility ),
         iLast = static_cast<std::size_t>( modifier_target_e::attr_vigour ); i <= iLast; ++i ) {
      const modifier* m = h.modifiers.get_modifier( static_cast<modifier_target_e>( i ) );
      if( m != nullptr ) {
        if( m->operandUnit == modifier_unit_e::die_type ) {
          dice_e& die = p.attributes.as_array()[i - static_cast<std::size_t>( modifier_target_e::attr_agility )];
          // Add or subtract the operand, because the type-checker in the modifier parser has verified that is the action.
          switch( m->action ) {
          case modifier_action_e::add:
            if( die < dice_e::d12 ) {
              die = die + m->operand;
            }
            break;
          case modifier_action_e::subtract:
            if( die > dice_e::d4 ) {
              die = die - m->operand;
            }
            break;
          }
        }
      }
    }
}

  // Determine the number of points we have to spend on skills.
  std::size_t skillPoints = 15;
  std::size_t smartsSkillPoints = 0;
  // Hindrances can affect skill points.
  apply_hindrances_modifier( p.get_hindrances(), modifier_target_e::skill_points, skillPoints );
  apply_hindrances_modifier( p.get_hindrances(), modifier_target_e::smarts_skill_points, smartsSkillPoints );
  // Randomly spend hindrance points on skill points.
  while( hindrancePoints >= 1 ) {
    if( ( rand() % 2 ) == 1 ) {
      --hindrancePoints;
      ++skillPoints;
    } else {
      break;
    }
  }
  if( ( hindrancePoints % 2 ) == 1 ) {
    // Make sure the hindrance points are even before reaching the edges, as edges consume two.
    --hindrancePoints;
    ++skillPoints;
  }
  // Spend skill points specific to smarts randomly on smarts skills.
  while( smartsSkillPoints > 0 ) {
    // Find a smarts skill that can be increased.
    std::vector<skill*> smartsSkillsBelowD12 = only_skills_below(
      only_skills_of_attribute( p.skills.as_vector(), attributes_e::smarts ), dice_e::d12 );

    skill* s;
    if( smartsSkillPoints >= 2 ) {
      s = smartsSkillsBelowD12[rand() % smartsSkillsBelowD12.size()];
    } else {
      std::vector<skill*> skillsBelowSmarts = only_skills_below( smartsSkillsBelowD12, p.attributes.get( attributes_e::smarts ) );
      s = skillsBelowSmarts[rand() % skillsBelowSmarts.size()];
    }
    const std::size_t costToIncrease = ( s->die >= p.attributes.get( attributes_e::smarts ) ) ? 2 : 1;

    // Increase the skill by one die type.
    s->die = s->die + 1;
    smartsSkillPoints -= costToIncrease;
  }
  // Spend skill points randomly.
  while( skillPoints > 0 ) {
    // Find a skill that can be increased.
    std::vector<skill*> skillsBelowD12 = only_skills_below( p.skills.as_vector(), dice_e::d12 );
    
    skill* s;
    if( skillPoints >= 2 ) {
      s = skillsBelowD12[rand() % skillsBelowD12.size()];
    } else {
      std::vector<skill*> skillsBelowAttribute;
      for( skill* sp : skillsBelowD12 ) {
        if( sp->die < p.attributes.get( sp->def.attribute ) ) {
          skillsBelowAttribute.push_back( sp );
        }
      }
      s = skillsBelowAttribute[rand() % skillsBelowAttribute.size()];
    }
    const std::size_t costToIncrease = ( s->die >= p.attributes.get( s->def.attribute ) ) ? 2 : 1;
    
    // Increase the skill by one die type.
    s->die = s->die + 1;
    skillPoints -= costToIncrease;
  }

  // Randomize edges and spend remaining hindrance points.
  std::size_t edgePoints = 1 + hindrancePoints / 2; // Includes the free edge for being a human.
  while( edgePoints > 0 ) {
    // Take a random edge whose requirements we meet.
    const edge* e = edge_manager::random_allowed_edge( p );
    if( e == nullptr ) {
      // If no edges are available, just stop looking for edges.
      break;
    }
    p.add_edge( *e );
    --edgePoints;
  }
}

bool can_raise_attribute( savage::person& p ) {
  using namespace savage;

  switch( p.rank() ) {
  case rank_e::novice:
    if( ( p.raisedAttributeFlags & person::raised_attribute_flags_e::raised_as_novice ) == person::raised_attribute_flags_e::none ) {
      return true;
    }
    break;
  case rank_e::seasoned:
    if( ( p.raisedAttributeFlags & person::raised_attribute_flags_e::raised_as_seasoned ) == person::raised_attribute_flags_e::none ) {
      return true;
    }
    break;
  case rank_e::veteran:
    if( ( p.raisedAttributeFlags & person::raised_attribute_flags_e::raised_as_veteran ) == person::raised_attribute_flags_e::none ) {
      return true;
    }
    break;
  case rank_e::heroic:
    if( ( p.raisedAttributeFlags & person::raised_attribute_flags_e::raised_as_heroic ) == person::raised_attribute_flags_e::none ) {
      return true;
    }
    break;
  case rank_e::legendary:
    if( ( p.raisedAttributeFlags & person::raised_attribute_flags_e::raised_as_legendary ) == person::raised_attribute_flags_e::none ) {
      return true;
    }
    break;
  default:
    throw std::runtime_error( "Invalid rank." );
  }
  return false;
}

void random_level_up( savage::person& p, std::size_t targetLevel ) {
  using namespace savage;

  if( p.level() > targetLevel ) {
    return;
  }

  // Randomly level up to the target level, either raising an attribute die, choosing an edge,
  // or increasing skill dice depending on what is available.
  std::size_t numLevelUps = targetLevel - p.level();
  while( numLevelUps > 0 ) {
    p.experience += 5;

    // Determine what can be done at this level up.
    std::vector<int> availableTasks;
    const edge* e = edge_manager::random_allowed_edge( p );
    if( e != nullptr ) {
      availableTasks.push_back( 0 );
    }
    std::vector<skill*> skillsBelowD12 = only_skills_below( p.skills.as_vector(), dice_e::d12 );
    if( !skillsBelowD12.empty() ) {
      availableTasks.push_back( 1 );
    }
    if( can_raise_attribute( p ) ) {
      availableTasks.push_back( 2 );
    }

    if( availableTasks.empty() ) {
      // If we can't do anything at this level up, just stop leveling up.
      std::cout << "Failed to level up all the way to " << targetLevel << std::endl;
      break;
    }

    // Randomly select a task from the available ones and perform it.
    int task = availableTasks[rand() % availableTasks.size()];
    switch( task ) {
    case 0: {
      // Task 0: Choose an edge.
      p.add_edge( *e );
      break;
    }
    case 1: {
      // Task 1: Increase skills. Either raise one skill over its attribute die or raise two skills one die type,
      // possibly up to their attribute dice.
      skill* firstSkill = skillsBelowD12[rand() % skillsBelowD12.size()];
      firstSkill->die = firstSkill->die + 1;
      if( firstSkill->die <= p.attributes.get( firstSkill->def.attribute ) ) {
        // The second skill can be taken if the first skill did not get raised over its attribute die.
        // The second skill must be one which will not be raised over its attribute die.
        std::vector<skill*> secondSkillPool;
        for( skill* sp : skillsBelowD12 ) {
          if( sp != firstSkill && sp->die < p.attributes.get( sp->def.attribute ) ) {
            secondSkillPool.push_back( sp );
          }
        }
        if( !secondSkillPool.empty() ) {
          skill* secondSkill = secondSkillPool[rand() % secondSkillPool.size()];
          secondSkill->die = secondSkill->die + 1;
        }
      }
      break;
    }
    case 2: {
      // Task 2: Raise an attribute die.
      std::vector<attributes_e> increasableAttributes = p.attributes.get_attributes_below( dice_e::d12 );
      std::size_t attributeIndex = static_cast<std::size_t>( increasableAttributes[rand() % increasableAttributes.size()] );

      dice_e* attributeArray = p.attributes.as_array();
      attributeArray[attributeIndex] = attributeArray[attributeIndex] + 1;

      switch( p.rank() ) {
      case rank_e::novice:
        p.raisedAttributeFlags = p.raisedAttributeFlags | person::raised_attribute_flags_e::raised_as_novice;
        break;
      case rank_e::seasoned:
        p.raisedAttributeFlags = p.raisedAttributeFlags | person::raised_attribute_flags_e::raised_as_seasoned;
        break;
      case rank_e::veteran:
        p.raisedAttributeFlags = p.raisedAttributeFlags | person::raised_attribute_flags_e::raised_as_veteran;
        break;
      case rank_e::heroic:
        p.raisedAttributeFlags = p.raisedAttributeFlags | person::raised_attribute_flags_e::raised_as_heroic;
        break;
      case rank_e::legendary:
        p.raisedAttributeFlags = p.raisedAttributeFlags | person::raised_attribute_flags_e::raised_as_legendary;
        break;
      default:
        throw std::runtime_error( "Invalid rank." );
      }
      break;
    }
    default:
      throw std::runtime_error( "Invalid level up task." );
    }

    --numLevelUps;
  }
}
