#pragma once

#include "rank_e.hpp"
#include "attributes_bag.hpp"
#include "skills_bag.hpp"
#include "edge.hpp"
#include "hindrance.hpp"

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
  std::vector<edge> edges;
  std::vector<std::string> replacedEdges;
  std::vector<hindrance> hindrances;

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
    , edges()
    , replacedEdges()
    , hindrances()
    , raisedAttributeFlags( raised_attribute_flags_e::none ) {}

  uint32_t level() const { return 1 + ( experience / 5 ); }

  rank_e rank() const {
    const uint32_t l = level();
    if( l >= 80 ) {
      return rank_e::legendary;
    } else if( l >= 60 ) {
      return rank_e::heroic;
    } else if( l >= 40 ) {
      return rank_e::veteran;
    } else if( l >= 20 ) {
      return rank_e::seasoned;
    } else {
      return rank_e::novice;
    }
  }

  const edge* find_edge( const std::string& edgeName ) const {
    for( const edge& e : edges ) {
      if( e.name == edgeName ) {
        return &e;
      }
    }
    return nullptr;
  }

  const std::string* find_replaced_edge( const std::string& edgeName ) const {
    for( const std::string& s : replacedEdges ) {
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
};

inline person::raised_attribute_flags_e operator|( person::raised_attribute_flags_e lhs, person::raised_attribute_flags_e rhs ) {
  return static_cast<person::raised_attribute_flags_e>( static_cast<uint8_t>( lhs ) | static_cast<uint8_t>( rhs ) );
}

inline person::raised_attribute_flags_e operator&( person::raised_attribute_flags_e lhs, person::raised_attribute_flags_e rhs ) {
  return static_cast<person::raised_attribute_flags_e>( static_cast<uint8_t>( lhs ) & static_cast<uint8_t>( rhs ) );
}
} // namespace savage
