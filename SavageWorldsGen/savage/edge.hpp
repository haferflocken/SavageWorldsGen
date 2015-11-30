#pragma once

#include "rank_e.hpp"
#include "attributes_bag.hpp"
#include "skills_bag.hpp"
#include "modifier_bag.hpp"

#include <string>
#include <vector>

namespace savage {
/**
 * A set of requirements for an edge.
 */
struct edge_requirements {
  attributes_bag requiredAttributes;
  skills_bag requiredSkills;
  std::string requiredEdge;

  edge_requirements( const attributes_bag& a = attributes_bag(), const skills_bag& s = skills_bag(), const std::string& e = "" )
    : requiredAttributes( a )
    , requiredSkills( s )
    , requiredEdge( e ) {}

  edge_requirements( const attributes_bag& a, const std::string& e )
    : requiredAttributes( a )
    , requiredSkills()
    , requiredEdge( e ) {}

  edge_requirements( const skills_bag& s, const std::string& e = "" )
    : requiredAttributes()
    , requiredSkills( s )
    , requiredEdge( e ) {}

  edge_requirements( const std::string& e )
    : requiredAttributes()
    , requiredSkills()
    , requiredEdge( e ) {}
};

/**
 * An edge a Savage Worlds character can have.
 */
struct edge {
  std::string name; // The name of this edge.
  rank_e requiredRank; // The rank a person must have to have this edge.
  std::string replacesEdge; // If not empty, this edge replaces replacesEdge when taken, which makes it a mandatory requirement.
  std::vector<edge_requirements> requirementOptions; // If any of these requirements are fully met by a person or this is empty, they can have this edge.
  modifier_bag modifiers; // The modifiers this edge applies.
  std::string additionalEffects; // The effects of this edge which can't be expressed as modifiers.

  edge()
    : name()
    , requiredRank( rank_e::novice )
    , requirementOptions()
    , additionalEffects() {}

  edge( const std::string& n, rank_e r, const std::string& e, const modifier_bag& m = modifier_bag() )
    : name( n )
    , requiredRank( r )
    , requirementOptions()
    , modifiers( m )
    , additionalEffects( e ) {}

  edge( const std::string& n, rank_e r, const edge_requirements& req, const std::string& e, const modifier_bag& m = modifier_bag() )
    : name( n )
    , requiredRank( r )
    , requirementOptions( { req } )
    , modifiers( m )
    , additionalEffects( e ) {}

  edge( const std::string& n, rank_e r, const std::vector<edge_requirements>& reqs, const std::string& e, const modifier_bag& m = modifier_bag() )
    : name( n )
    , requiredRank( r )
    , requirementOptions( reqs )
    , modifiers( m )
    , additionalEffects( e ) {}

  edge( const std::string& n, rank_e r, const std::string& rpl, const std::string& e, const modifier_bag& m = modifier_bag() )
    : name( n )
    , requiredRank( r )
    , replacesEdge( rpl )
    , requirementOptions()
    , modifiers( m )
    , additionalEffects( e ) {}

  edge( const std::string& n, rank_e r, const std::string& rpl, const edge_requirements& req, const std::string& e, const modifier_bag& m = modifier_bag() )
    : name( n )
    , requiredRank( r )
    , replacesEdge( rpl )
    , requirementOptions( { req } )
    , modifiers( m )
    , additionalEffects( e ) {}
};

inline std::ostream& operator<<( std::ostream& o, const edge& e ) {
  o << e.name << ": ";
  if( e.modifiers.as_vector().empty() ) {
    return o << e.additionalEffects;
  } else if( e.additionalEffects.empty() ) {
    return o << e.modifiers;
  } else {
    return o << e.modifiers << ", " << e.additionalEffects;
  }
}
} // namespace savage
