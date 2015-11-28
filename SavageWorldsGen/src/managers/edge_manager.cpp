#include "../../savage/managers/edge_manager.hpp"

#include <cstdlib>

using namespace savage;

std::vector<edge>* edge_manager::s_edges = nullptr;

const std::vector<edge>& edge_manager::get_edges() {
  if( s_edges == nullptr ) {
    s_edges = new std::vector<edge> {
      edge( "Ace", rank_e::novice, agility_bag( dice_e::d8 ), "+2 to Boating, Driving, Piloting; may take Soak rolls for vehicle at -2" ),
      edge( "Acrobat", rank_e::novice, agility_bag( dice_e::d8 ) + strength_bag( dice_e::d6 ), "+2 to nimbleness-based Agility rolls; +1 Parry if unencumbered" ),
      // TODO(jwerner) Adept
      edge( "Alertness", rank_e::novice, "+2 Notice" ),
      edge( "Ambidextrous", rank_e::novice, agility_bag( dice_e::d8 ), "Ignore -2 penalty for using off-hand" ),
      // TODO(jwerner) Arcane Backgrounds
      edge( "Arcane Resistance", rank_e::novice, spirit_bag( dice_e::d8 ), "Armor 2 vs. magic, +2 to resist powers" ),
      edge( "Improved Arcane Resistance", rank_e::novice, "Arcane Resistance", "Armor 4 vs. magic, +4 to resist magic effects" ),
      edge( "Assassin", rank_e::novice, edge_requirements( agility_bag( dice_e::d8 ), skills_bag( "Athletics", dice_e::d6 ) + skills_bag( "Fighting", dice_e::d6 ) + skills_bag( "Stealth", dice_e::d8 ) ), "+2 to damage when striking a foe unawares" ),
      edge( "Attractive", rank_e::novice, vigour_bag( dice_e::d6 ), "Charisma + 2" ),
      edge( "Very Attractive", rank_e::novice, edge_requirements( "Attractive" ), "Charisma + 4" ),
      edge( "Beast Bond", rank_e::novice, "Character may spend Bennies for their animals" ),
      edge( "Beast Master", rank_e::novice, spirit_bag( dice_e::d8 ), "The hero gains an animal companion" ),
      edge( "Berserk", rank_e::novice, "See book" ),
      edge( "Block", rank_e::seasoned, skills_bag( "Fighting", dice_e::d8 ), "Parry + 1" ),
      edge( "Improved Block", rank_e::veteran, "Block", "Parry + 2" ),
      edge( "Brave", rank_e::novice, spirit_bag( dice_e::d6 ), "+2 to Fear tests" ),
      edge( "Brawler", rank_e::novice, strength_bag( dice_e::d8 ), "+2 to unarmed damage rolls" ),
      edge( "Bruiser", rank_e::seasoned, "Brawler", "Bonus die to unarmed damage is d8 instead of d6" ),
      edge( "Brawny", rank_e::novice, strength_bag( dice_e::d6 ) + vigour_bag( dice_e::d6 ), "Toughness + 1; load limit is 8 x Str" ),
      // TODO(jwerner) Champion
      edge( "Charismatic", rank_e::novice, spirit_bag( dice_e::d8 ), "Charisma + 2" ),
      edge( "Combat Reflexes", rank_e::seasoned, "+2 to recover from being Shaken" ),
      edge( "Command", rank_e::novice, smarts_bag( dice_e::d6 ), "+1 to troops recovering from being Shaken" ),
      edge( "Command Presence", rank_e::novice, edge_requirements( "Command" ), "Increase command radius to 10\"" ),
      // TODO(jwerner) Common Bond
      edge( "Connections", rank_e::novice, "Call upon powerful friends" ),
      edge( "Counterattack", rank_e::seasoned, skills_bag( "Fighting", dice_e::d8 ), "Receive free Fighting attack at -2 once per round when a foe fails a Fighting attack" ),
      edge( "Improved Counterattack", rank_e::veteran, "Counterattack", "Receive free Fighting attack once per round when a foe fails a Fighting attack" ),
      edge( "Danger Sense", rank_e::novice, "Notice at -2 to detect surprise attacks/danger" ),
      // TODO(jwerner) Dead Shot
      edge( "Dodge", rank_e::seasoned, agility_bag( dice_e::d8 ), "-1 to be hit with ranged attacks" ),
      edge( "Improved Dodge", rank_e::veteran, "Dodge", "-2 to be hit with ranged attacks" ),
      edge( "Elan", rank_e::novice, spirit_bag( dice_e::d8 ), "+2 when spending a Benny on a Trait roll (including Soak rolls)" ),
      edge( "Extraction", rank_e::novice, agility_bag( dice_e::d8 ), "Ignore one foe's free attack when withdrawing from melee with an Agility roll" ),
      edge( "Improved Extraction", rank_e::novice, "Extraction", "Ignore one foe's free attack when withdrawing from melee with an Agility roll. With a raise, no foes get their free melee attack" ),
      edge( "Fast Healer", rank_e::novice, vigour_bag( dice_e::d8 ), "+2 to natural healing rolls" ),
      edge( "Fervor", rank_e::veteran, edge_requirements( spirit_bag( dice_e::d8 ), "Command" ), "+1 melee damage to troops in command" ),
      edge( "First Strike", rank_e::novice, agility_bag( dice_e::d8 ), "May attack one foe who moves adjacent" ),
      edge( "Improved First Strike", rank_e::heroic, "First Strike", "May attack every foe who moves adjacent" ),
      edge( "Fleet-Footed", rank_e::novice, agility_bag( dice_e::d6 ), "+2 Pace, d10 running die instead of d6" )
    };
  }
  return *s_edges;
}

const edge* edge_manager::random_allowed_edge( const person& p ) {
  std::vector<const edge*> allowedEdges;
  for( const edge& e : get_edges() ) {
    if( p.meets_requirements( e ) && p.find_edge( e.name ) == nullptr && p.find_replaced_edge( e.name ) == nullptr ) {
      // Only accept edges that we meet the requirements for, we don't already have, and we haven't
      // replaced with a different edge.
      allowedEdges.push_back( &e );
    }
  }
  if( allowedEdges.empty() ) {
    return nullptr;
  }
  int i = rand() % allowedEdges.size();
  return allowedEdges[i];
}

void edge_manager::read_edges( const std::string& filePath ) {
  if( s_edges != nullptr ) {
    delete s_edges;
  }
  
  // TODO(jwerner) Read the edges from the file.
  s_edges = new std::vector<edge>();
}
