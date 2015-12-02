#pragma once

#include "person.hpp"

namespace savage {
/**
 * Generates all the possible characters that could result from leveling up a given character.
 */
class level_up_generator {
  const person m_base; // The person to level up.
  person m_current; // The most recently generated level-up result.

public:
  level_up_generator( const person& base )
    : m_base( base )
    , m_current() {}

  /**
   * Returns the most recently generated person. Only valid until the next call to generate_next().
   */
  const person& get_current() const { return m_current; }

  /**
   * Returns true if there are more people to generate; false otherwise.
   */
  bool has_next() const { return false; }

  /**
   * Generates the next person. Throws an error if there is no next person.
   */
  void generate_next() {
    // TODO(jwerner)
  }
};
} // namespace savage
