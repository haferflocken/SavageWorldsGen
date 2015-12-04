#pragma once

#include "person.hpp"

namespace savage {
/**
 * Generates all possible level one characters for the current world.
 */
class level_one_generator {
  // A node in the tree of generation.
  struct gen_state {
    uint8_t majorHindranceToTake;
    uint8_t minorHindrancesToTake;
    uint8_t hindrancePointsRemaining;
    uint8_t attributePointsRemaining;
    uint8_t skillPointsRemaining;
    uint8_t smartsSkillPointsRemaining;
    uint8_t edgePointsRemaining;

    const modifier_bag_source* appliedModifier;
    person generatedPerson;

    gen_state* prevState;
    std::vector<gen_state> nextStates;
    bool nextStatesGenerated;

    gen_state()
      : majorHindranceToTake( 0 )
      , minorHindrancesToTake( 0 )
      , hindrancePointsRemaining( 0 )
      , attributePointsRemaining( 0 )
      , skillPointsRemaining( 0 )
      , smartsSkillPointsRemaining( 0 )
      , edgePointsRemaining( 0 )
      , appliedModifier( nullptr )
      , generatedPerson( 1 )
      , prevState( nullptr )
      , nextStates()
      , nextStatesGenerated( false ) {}

    // Generates the next states, sets this as their parent, and sets nextStatesGenerated to true.
    void generate_next_states();

  private:
    // Add a next state to nextStates and return a pointer to it.
    gen_state* add_empty_next_state();
  };

  person m_current; // The most recently generated level one character.
  std::vector<gen_state> m_rootNodes; // The root nodes of the generation trees.
  std::vector<gen_state*> m_frontier; // The current leaves of the generation trees.

public:
  level_one_generator();

  /**
   * Get the most recent person the generator has generated. Only valid until the next call to generate_next().
   */
  const person& get_current() const { return m_current; }

  /**
   * Returns true if there are more people to generate; false otherwise.
   */
  bool has_next() const;

  /**
   * Generates the next person. Throws an error if there is no next person.
   */
  void generate_next();

private:
  // Removes a node from the generation tree. Removes nodes further up the tree if this means they have no children.
  void remove_node( gen_state* node );
};
} // namespace savage
