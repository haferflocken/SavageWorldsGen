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
/**
 * A person in Savage Worlds.
 */
struct person {
  std::string name;
  uint32_t experience;
  attributes_bag attributes;
  skills_bag skills;

private:
  std::vector<edge> m_edges;
  std::vector<std::string> m_replacedEdges;
  std::vector<hindrance> m_hindrances;

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

public:
  enum class raised_attribute_flags_e : uint8_t {
    none = 0,
    raised_as_novice = 1,
    raised_as_seasoned = 2,
    raised_as_veteran = 4,
    raised_as_heroic = 8,
    raised_as_legendary = 16
  };
  raised_attribute_flags_e raisedAttributeFlags;

  person()
    : name()
    , experience( 0 )
    , attributes()
    , skills()
    , m_edges()
    , m_replacedEdges()
    , m_hindrances()
    , m_pace( 6 )
    , m_runDie( dice_e::d6 )
    , m_parryModifier( 0 )
    , m_dodge( 0 )
    , m_charisma( 0 )
    , m_bennies( 3 )
    , raisedAttributeFlags( raised_attribute_flags_e::none ) {}

  uint32_t level() const { return 1 + ( experience / 5 ); }

  rank_e rank() const {
    if( experience >= 80 ) {
      return rank_e::legendary;
    } else if( experience >= 60 ) {
      return rank_e::heroic;
    } else if( experience >= 40 ) {
      return rank_e::veteran;
    } else if( experience >= 20 ) {
      return rank_e::seasoned;
    } else {
      return rank_e::novice;
    }
  }

  uint8_t parry() const {
    const dice_e fightingSkill = skills.find_die( "Fighting" );
    return ( as_numeric( fightingSkill ) / 2 ) + m_parryModifier;
  }

  const std::vector<edge>& get_edges() const { return m_edges; }
  const std::vector<hindrance>& get_hindrances() const { return m_hindrances; }

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

  bool meets_requirements( const edge& e ) const {
    // Check that our rank is greater than or equal to the edge's required rank.
    if( rank() < e.requiredRank ) {
      return false;
    }

    // Check that we have the edge that this edge replaces, if any.
    if( !e.replacesEdge.empty() && find_edge( e.replacesEdge ) == nullptr ) {
      return false;
    }

    // We meet the requirements if we meet any of the requirement options, or if there are no requirements.
    if( e.requirementOptions.empty() ) {
      return true;
    }
    for( const edge_requirements& r : e.requirementOptions ) {
      // Check if we have the required attributes.
      if( !( attributes >= r.requiredAttributes ) ) {
        continue;
      }

      // Check if we have the required skills.
      if( !( skills >= r.requiredSkills ) ) {
        continue;
      }
      
      // Check if we have the required edges.
      if( !r.requiredEdge.empty() && find_edge( r.requiredEdge ) == nullptr ) {
        continue;
      }

      // Return true if we meet the requirements.
      return true;
    }
    // Return false when we meet no requirements.
    return false;
  }

  /**
   * Add an edge to the edge list and apply its modifiers.
   */
  void add_edge( const edge& e );

  /**
   * Add a hindrance to the hindrance list and apply its modifiers.
   */
  void add_hindrance( const hindrance& h );

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
