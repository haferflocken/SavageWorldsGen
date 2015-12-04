#pragma once

#include "rank_e.hpp"
#include "attributes_bag.hpp"
#include "skills_bag.hpp"
#include "edge.hpp"
#include "hindrance.hpp"
#include "savage_exception.hpp"

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

namespace savage {
// Forward declarations.
class level_one_generator;
class level_up_generator;

/**
 * A person in Savage Worlds.
 */
class person {
  friend class level_one_generator;
  friend class level_up_generator;

public:
  enum class raised_attribute_flags_e : uint8_t {
    none = 0,
    raised_as_novice = 1,
    raised_as_seasoned = 2,
    raised_as_veteran = 4,
    raised_as_heroic = 8,
    raised_as_legendary = 16
  };
  
private:
  uint32_t m_level;
  attributes_bag m_attributes;
  skills_bag m_skills;

  std::vector<edge> m_edges;
  std::vector<std::string> m_replacedEdges;
  std::vector<hindrance> m_hindrances;

  // The modifiers that have been applied to this person, in the order they were applied.
  std::vector<const modifier_bag_source*> m_modifierStack;

  // Derived statistics.
  int8_t m_toughness;
  int8_t m_parryModifier;
  int8_t m_dodge;
  int8_t m_pace;
  dice_e m_runDie;
  int8_t m_charisma;
  int8_t m_fatigueChecks;
  int8_t m_fearChecks;
  int8_t m_bennies;

  raised_attribute_flags_e m_raisedAttributeFlags;

public:
  /**
   * Default constructor, which makes a level 0 character.
   */
  person( uint32_t level = 0 )
    : m_level( level )
    , m_attributes()
    , m_skills()
    , m_edges()
    , m_replacedEdges()
    , m_hindrances()
    , m_modifierStack()
    , m_pace( 6 )
    , m_runDie( dice_e::d6 )
    , m_parryModifier( 0 )
    , m_dodge( 0 )
    , m_charisma( 0 )
    , m_bennies( 3 )
    , m_raisedAttributeFlags( raised_attribute_flags_e::none ) {}

  /**
   * Initialize a character of the given level with a modifier stack. This constructor will
   * read the given stack and appropriately determine the edges and hindrances in it.
   */
  person( uint32_t level, const std::vector<const modifier_bag_source*>& modifierStack );

  uint32_t get_level() const { return m_level; }
  const attributes_bag& get_attributes() const { return m_attributes; }
  const skills_bag& get_skills() const { return m_skills; }
  const std::vector<edge>& get_edges() const { return m_edges; }
  const std::vector<hindrance>& get_hindrances() const { return m_hindrances; }

  rank_e rank() const {
    if( m_level >= 16 ) {
      return rank_e::legendary;
    } else if( m_level >= 12 ) {
      return rank_e::heroic;
    } else if( m_level >= 8 ) {
      return rank_e::veteran;
    } else if( m_level >= 4 ) {
      return rank_e::seasoned;
    } else {
      return rank_e::novice;
    }
  }

  uint8_t parry() const {
    const dice_e fightingSkill = m_skills.find_die( "Fighting" );
    return ( as_numeric( fightingSkill ) / 2 ) + m_parryModifier;
  }

  void set_level( uint32_t level ) { m_level = level; }

  const edge* find_edge( const std::string& edgeName ) const {
    for( const edge& e : m_edges ) {
      if( e.name == edgeName ) {
        return &e;
      }
    }
    return nullptr;
  }

  const std::string* find_replaced_edge( const std::string& edgeName ) const {
    for( const std::string& s : m_replacedEdges ) {
      if( s == edgeName ) {
        return &s;
      }
    }
    return nullptr;
  }

  /**
   * Checks if a person meets the requirements for an edge.
   */
  bool meets_requirements( const edge& e ) const;

  // Add an edge to the edge list and apply its modifiers.
  void add_edge( const edge& e );

  // Add a hindrance to the hindrance list and apply its modifiers.
  void add_hindrance( const hindrance& h );

  // Apply a general modifier bag.
  void add_general_modifier( const modifier_bag_source* source );

private:
  // Apply a modifier to this person.
  void apply_modifier( const modifier& m );

  // Remove the effects of a modifier from this person.
  void unapply_modifier( const modifier& m );
};

inline person::raised_attribute_flags_e operator|( person::raised_attribute_flags_e lhs, person::raised_attribute_flags_e rhs ) {
  return static_cast<person::raised_attribute_flags_e>( static_cast<uint8_t>( lhs ) | static_cast<uint8_t>( rhs ) );
}

inline person::raised_attribute_flags_e operator&( person::raised_attribute_flags_e lhs, person::raised_attribute_flags_e rhs ) {
  return static_cast<person::raised_attribute_flags_e>( static_cast<uint8_t>( lhs ) & static_cast<uint8_t>( rhs ) );
}
} // namespace savage
