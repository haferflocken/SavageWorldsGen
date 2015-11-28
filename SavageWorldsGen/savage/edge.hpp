#pragma once

#include "rank_e.hpp"
#include "attributes_bag.hpp"
#include "skills_bag.hpp"

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
  std::string effects; // The effects of this edge.

  edge()
    : name()
    , requiredRank( rank_e::novice )
    , requirementOptions()
    , effects() {}

  edge( const std::string& n, rank_e r, const std::string& e )
    : name( n )
    , requiredRank( r )
    , requirementOptions()
    , effects( e ) {}

  edge( const std::string& n, rank_e r, const edge_requirements& req, const std::string& e )
    : name( n )
    , requiredRank( r )
    , requirementOptions( { req } )
    , effects( e ) {}

  edge( const std::string& n, rank_e r, const std::vector<edge_requirements>& reqs, const std::string& e )
    : name( n )
    , requiredRank( r )
    , requirementOptions( reqs )
    , effects( e ) {}

  edge( const std::string& n, rank_e r, const std::string& rpl, const std::string& e )
    : name( n )
    , requiredRank( r )
    , replacesEdge( rpl )
    , requirementOptions()
    , effects( e ) {}

  edge( const std::string& n, rank_e r, const std::string& rpl, const edge_requirements& req, const std::string& e )
    : name( n )
    , requiredRank( r )
    , replacesEdge( rpl )
    , requirementOptions( { req } )
    , effects( e ) {}
};
} // namespace savage
