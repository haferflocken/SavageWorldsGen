#include "../savage/level_one_generator.hpp"

#include "../savage/managers/edge_manager.hpp"
#include "../savage/managers/hindrance_manager.hpp"
#include "../savage/managers/skills_manager.hpp"

#include "../savage/savage_exception.hpp"

#include <vector>

using namespace savage;

void level_one_generator::gen_state::generate_next_states() {
  if( nextStatesGenerated ) {
    return;
  }
  nextStatesGenerated = true; // Immediately set the nextStatesGenerated bool so we don't have to set it when returning later.

  // Our options are to:
  // - decrement majorHindranceToTake, get a major hindrance, and increase hindrance points by 2
  // - decrement minorHindrancesToTake, get a minor hindrance, and increase hindrance points by 1
  // - spend 2 hindrance points to get an attribute point
  // - spend 2 hindrance points to get an edge point
  // - spend 1 hindrance point to get a skill point
  // - spend 1 attribute point to buy an attribute
  // - spend 2 skill points to increase a skill by one die type, provided the skill is at or over its attribute die
  // - spend 1 skill point to increase a skill by one die type, provided the skill is beneath its attribute die
  // - spend 2 smarts skill points to increase a smarts skill by one die type, provided the skill is at or over the smarts die
  // - spend 1 smarts skill point to increase a smarts skill by one die type, provided the skill is beneath the smarts die
  // - spend 1 edge point to get an edge
  // We generate a next state for every option that applies.

  if( majorHindranceToTake >= 1 ) {
    // Generate a state for every major hindrance we can take.
    for( const hindrance& majorHindrance : hindrance_manager::get_major_hindrances() ) {
      gen_state* next = add_empty_next_state();
      --next->majorHindranceToTake;
      next->hindrancePointsRemaining += 2;
      next->appliedModifier = &majorHindrance;
      next->generatedPerson.add_hindrance( majorHindrance );
    }
  }

  if( minorHindrancesToTake >= 1 ) {
    // Generate a state for every minor hindrance we can take.
    for( const hindrance& minorHindrance : hindrance_manager::get_minor_hindrances() ) {
      gen_state* next = add_empty_next_state();
      --next->minorHindrancesToTake;
      ++next->hindrancePointsRemaining;
      next->appliedModifier = &minorHindrance;
      next->generatedPerson.add_hindrance( minorHindrance );
    }
  }
  if( hindrancePointsRemaining >= 2 ) {
    // There isn't a check that verifies that spent hindrance points will actually be able to buy attributes
    // or edges later. That is checked when trying to spend the attribute or edge point.
    gen_state* nextAttribute = add_empty_next_state();
    nextAttribute->hindrancePointsRemaining -= 2;
    ++nextAttribute->attributePointsRemaining;

    gen_state* nextEdge = add_empty_next_state();
    nextEdge->hindrancePointsRemaining -= 2;
    ++nextEdge->edgePointsRemaining;
  }
  if( hindrancePointsRemaining >= 1 ) {
    // There isn't a check that verifies that spent hindrance points will actually be able to buy a skill later.
    // That is checked when trying to spend the skill point.
    gen_state* nextSkill = add_empty_next_state();
    --nextSkill->hindrancePointsRemaining;
    ++nextSkill->skillPointsRemaining;
  }

  if( attributePointsRemaining >= 1 ) {
    // Generate a state for increasing each attribute that can be increased.
    std::vector<attributes_e> attributesBelowD12 = generatedPerson.get_attributes().get_attributes_below( dice_e::d12 );
    for( attributes_e attribute : attributesBelowD12 ) {
      gen_state* next = add_empty_next_state();
      --next->attributePointsRemaining;
      next->appliedModifier = get_inc_attribute( attribute );
      next->generatedPerson.add_general_modifier( get_inc_attribute( attribute ) );
    }
  }

  if( skillPointsRemaining >= 1 || smartsSkillPointsRemaining >= 1 ) {
    std::vector<const skill*> skillsBelowAttributes;
    std::vector<const skill*> skillsAtOrAboveAttributes;
    split_skills_by_attribute_dice( generatedPerson.get_skills(), generatedPerson.get_attributes(), skillsBelowAttributes, skillsAtOrAboveAttributes );
    skillsAtOrAboveAttributes = only_skills_below( skillsAtOrAboveAttributes, dice_e::d12 );

    if( !skillsBelowAttributes.empty() ) {
      if( skillPointsRemaining >= 1 ) {
        // Generate a state for each skill we can increase for 1 point.
        for( const skill* skillFor1Point : skillsBelowAttributes ) {
          gen_state* next = add_empty_next_state();
          --next->skillPointsRemaining;
          // TODO(jwerner) skill increase modifiers
        }
      }
      if( smartsSkillPointsRemaining >= 1 ) {
        // Generate a state for each smarts skill we can increase for 1 point.
        const std::vector<const skill*> smartsSkillsBelowAttributes = only_skills_of_attribute( skillsBelowAttributes, attributes_e::smarts );
        for( const skill* smartsSkillFor1Point : smartsSkillsBelowAttributes ) {
          gen_state* next = add_empty_next_state();
          --next->smartsSkillPointsRemaining;
          // TODO(jwerner) skill increase modifiers
        }
      }
    }
    if( !skillsAtOrAboveAttributes.empty() ) {
      if( skillPointsRemaining >= 2 ) {
        // Generate a state for each skill we can increase for 2 points.
        for( const skill* skillFor2Points : skillsAtOrAboveAttributes ) {
          gen_state* next = add_empty_next_state();
          next->skillPointsRemaining -= 2;
          // TODO(jwerner) skill increase modifiers
        }
      }
      if( smartsSkillPointsRemaining >= 2 ) {
        // Generate a state for each smarts skill we can increase for 2 points.
        const std::vector<const skill*> smartsSkillsAtOrAboveSmarts = only_skills_of_attribute( skillsAtOrAboveAttributes, attributes_e::smarts );
        for( const skill* smartsSkillFor2Points : smartsSkillsAtOrAboveSmarts ) {
          gen_state* next = add_empty_next_state();
          next->smartsSkillPointsRemaining -= 2;
          // TODO(jwerner) skill increase modifiers
        }
      }
    }
  }

  if( edgePointsRemaining >= 1 ) {
    // Generate a state for each edge we can take.
    for( const edge* e : edge_manager::allowed_edges( generatedPerson ) ) {
      gen_state* next = add_empty_next_state();
      --next->edgePointsRemaining;
      next->appliedModifier = e;
      next->generatedPerson.add_edge( *e );
    }
  }
}

level_one_generator::gen_state* level_one_generator::gen_state::add_empty_next_state() {
  nextStates.push_back( gen_state() );
  gen_state* next = &nextStates.back();

  next->majorHindranceToTake = majorHindranceToTake;
  next->minorHindrancesToTake = minorHindrancesToTake;
  next->hindrancePointsRemaining = hindrancePointsRemaining;
  next->attributePointsRemaining = attributePointsRemaining;
  next->skillPointsRemaining = skillPointsRemaining;
  next->smartsSkillPointsRemaining = smartsSkillPointsRemaining;
  next->edgePointsRemaining = edgePointsRemaining;

  next->generatedPerson = generatedPerson;

  next->prevState = this;

  return next;
}

level_one_generator::level_one_generator()
  : m_current()
  , m_rootNodes()
  , m_frontier() {
  // Initialize the root nodes with all the possible numbers of hindrances to take,
  // along with the right number of attribute points, skill points, and edge points.
  gen_state noHindrances;
  noHindrances.attributePointsRemaining = 5;
  noHindrances.skillPointsRemaining = 15;
  noHindrances.edgePointsRemaining = 1;

  gen_state maxHindrances = noHindrances;
  maxHindrances.majorHindranceToTake = 1;
  maxHindrances.minorHindrancesToTake = 2;

  gen_state oneMajorOneMinorHindrance = noHindrances;
  oneMajorOneMinorHindrance.majorHindranceToTake = 1;
  oneMajorOneMinorHindrance.minorHindrancesToTake = 1;

  gen_state majorHindrance = noHindrances;
  majorHindrance.majorHindranceToTake = 1;

  gen_state twoMinorHindrances = noHindrances;
  twoMinorHindrances.minorHindrancesToTake = 2;

  gen_state oneMinorHindrance = noHindrances;
  oneMinorHindrance.minorHindrancesToTake = 1;

  m_rootNodes.push_back( noHindrances );
  m_rootNodes.push_back( maxHindrances );
  m_rootNodes.push_back( oneMajorOneMinorHindrance );
  m_rootNodes.push_back( majorHindrance );
  m_rootNodes.push_back( twoMinorHindrances );
  m_rootNodes.push_back( oneMinorHindrance );

  // Add the root nodes to the frontier.
  for( gen_state& s : m_rootNodes ) {
    m_frontier.push_back( &s );
  }
}

bool level_one_generator::has_next() const {
  return !m_frontier.empty();
}

void level_one_generator::generate_next() {
  if( m_frontier.empty() ) {
    throw savage_error( "LEVEL_ONE_GENERATOR::GENERATE_NEXT WAS CALLED WHEN THERE WAS NO NEXT PERSON." );
  }

  // Begin with a random node on the frontier.
  std::size_t nextFrontierIndex = rand() % m_frontier.size();

  while( !m_frontier.empty() ) {
    // Remove the chosen node from the frontier.
    std::vector<gen_state*>::iterator nextFrontierItr = m_frontier.begin() + nextFrontierIndex;
    gen_state* node = *nextFrontierItr;
    m_frontier.erase( nextFrontierItr );

    // Try and generate the next states for the node.
    node->generate_next_states();

    // If the node is a terminal node, set m_current to it and return. Otherwise, add its next states to the frontier and continue.
    if( node->nextStates.empty() ) {
      // Generate a person using this path in the generation tree.
      // First, build the modifier stack in reverse order by traversing up the tree.
      /*std::vector<const modifier_bag_source*> modifiers;
      for( gen_state* i = node; i != nullptr; i = i->prevState ) {
        if( i->appliedModifier != nullptr ) {
          modifiers.push_back( i->appliedModifier );
        }
      }

      // Reverse the stack so it is in the correct order.
      std::reverse( modifiers.begin(), modifiers.end() );*/
      
      // Set m_current to a level 1 person with the generated modifier stack.
      m_current = node->generatedPerson;

      // After generating the person, remove this node from its parent's list of children, and prune as far
      // upwards as possible.
      if( node->prevState != nullptr ) {
        remove_node( node );
      }
      return;
    } else {
      // Add the next states to the frontier, and randomly choose one of the new states to progress with.
      // This means we have a randomized traversal of the generation tree which is similar to depth-first.
      for( gen_state& s : node->nextStates ) {
        m_frontier.push_back( &s );
      }
      nextFrontierIndex = m_frontier.size() - ( rand() % node->nextStates.size() ) - 1;
    }
  }
}

void level_one_generator::remove_node( gen_state* node ) {
  while( node->prevState != nullptr ) {
    gen_state* parent = node->prevState;
    for( auto i = parent->nextStates.begin(), iEnd = parent->nextStates.end(); i != iEnd; ++i ) {
      if( &( *i ) == node ) {
        parent->nextStates.erase( i );
        break;
      }
    }
    if( parent->nextStates.empty() ) {
      node = parent;
    } else {
      break;
    }
  }
}
