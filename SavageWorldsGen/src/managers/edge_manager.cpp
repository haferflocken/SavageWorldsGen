#include "../../savage/managers/edge_manager.hpp"

#include <cstdlib>

using namespace savage;

std::vector<edge>* edge_manager::s_edges = nullptr;

const std::vector<edge>& edge_manager::get_edges() {
  if( s_edges == nullptr ) {
    s_edges = new std::vector<edge> {
      edge( "Ace", rank_e::novice, agility_bag( dice_e::d8 ), "May take Soak rolls for vehicle at -2", parse_modifiers( "Boating + 2, Driving + 2, Piloting + 2" ) ),
      edge( "Acrobat", rank_e::novice, agility_bag( dice_e::d8 ) + strength_bag( dice_e::d6 ), "+2 to nimbleness-based Agility rolls; +1 Parry if unencumbered" ),
      // TODO(jwerner) Adept
      edge( "Alertness", rank_e::novice, "", parse_modifiers( "Notice + 2" ) ),
      edge( "Ambidextrous", rank_e::novice, agility_bag( dice_e::d8 ), "Ignore -2 penalty for using off-hand" ),
      // TODO(jwerner) Arcane Backgrounds
      edge( "Arcane Resistance", rank_e::novice, spirit_bag( dice_e::d8 ), "Armor 2 vs. magic, +2 to resist powers" ),
      edge( "Improved Arcane Resistance", rank_e::novice, "Arcane Resistance", "Armor 4 vs. magic, +4 to resist magic effects" ),
      edge( "Assassin", rank_e::novice, edge_requirements( agility_bag( dice_e::d8 ), skills_bag( "Athletics", dice_e::d6 ) + skills_bag( "Fighting", dice_e::d6 ) + skills_bag( "Stealth", dice_e::d8 ) ), "+2 to damage when striking a foe unawares" ),
      edge( "Attractive", rank_e::novice, vigour_bag( dice_e::d6 ), "", parse_modifiers( "Charisma + 2" ) ),
      edge( "Very Attractive", rank_e::novice, edge_requirements( "Attractive" ), "", parse_modifiers( "Charisma + 4" ) ),
      edge( "Beast Bond", rank_e::novice, "Character may spend Bennies for their animals" ),
      edge( "Beast Master", rank_e::novice, spirit_bag( dice_e::d8 ), "The hero gains an animal companion" ),
      edge( "Berserk", rank_e::novice, "See book" ),
      edge( "Block", rank_e::seasoned, skills_bag( "Fighting", dice_e::d8 ), "", parse_modifiers( "Parry + 1" ) ),
      edge( "Improved Block", rank_e::veteran, "Block", "", parse_modifiers( "Parry + 2" ) ),
      edge( "Brave", rank_e::novice, spirit_bag( dice_e::d6 ), "", parse_modifiers( "FearChecks + 2" ) ),
      edge( "Brawler", rank_e::novice, strength_bag( dice_e::d8 ), "+2 to unarmed damage rolls" ),
      edge( "Bruiser", rank_e::seasoned, "Brawler", "Bonus die to unarmed damage is d8 instead of d6" ),
      edge( "Brawny", rank_e::novice, strength_bag( dice_e::d6 ) + vigour_bag( dice_e::d6 ), "Load limit is 8 x Str", parse_modifiers( "Toughness + 1" ) ),
      // TODO(jwerner) Champion
      edge( "Charismatic", rank_e::novice, spirit_bag( dice_e::d8 ), "", parse_modifiers( "Charisma + 2" ) ),
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
      edge( "Fleet-Footed", rank_e::novice, agility_bag( dice_e::d6 ), "", parse_modifiers( "Pace + 2, Run = d10" ) ),
      edge( "Florentine", rank_e::novice, edge_requirements( agility_bag( dice_e::d8 ), skills_bag( "Fighting", dice_e::d8 ) ), "+1 vs. foes with single weapon and no shield, ignore 1 point of gang up bonus" ),
      edge( "Followers", rank_e::legendary, "Attract 5 henchmen" ),
      edge( "Frenzy", rank_e::seasoned, skills_bag( "Fighting", dice_e::d10 ), "1 extra Fighting attack at -2" ),
      edge( "Improved Frenzy", rank_e::veteran, "Frenzy", "1 extra Fighting attack" ),
      // TODO(jwerner) Gadgeteer
      edge( "Giant Killer", rank_e::veteran, "+1d6 damage when attacking large creatures" ),
      edge( "Hard to Kill", rank_e::novice, spirit_bag( dice_e::d8 ), "Ignore wound penalties for Vigour rolls made on the injury table" ),
      edge( "Harder to Kill", rank_e::veteran, "Hard to Kill", "50% chance of surviving 'death'" ),
      edge( "Healer", rank_e::novice, spirit_bag( dice_e::d8 ), "", parse_modifiers( "Healing + 2" ) ),
      edge( "Hold the Line!", rank_e::seasoned, edge_requirements( smarts_bag( dice_e::d8 ), "Command" ), "Troops have +1 Toughness" ),
      // TODO(jwerner) Holy/Unholy Warrior
      edge( "Improvisational Fighter", rank_e::seasoned, smarts_bag( dice_e::d6 ), "Ignores the usual -1 penalty to attack and Parry for improvised weapons" ),
      edge( "Inspire", rank_e::seasoned, edge_requirements( "Command" ), "+1 to Spirit rolls of all troops in command" ),
      edge( "Investigator", rank_e::novice, edge_requirements( smarts_bag( dice_e::d8 ), skills_bag( "Investigation", dice_e::d8 ) + skills_bag( "Streetwise", dice_e::d8 ) ), "", parse_modifiers( "Investigation + 2, Streetwise + 2" ) ),
      edge( "Jack-of-All-Trades", rank_e::novice, smarts_bag( dice_e::d10 ), "No -2 for unskilled Smarts-based tests" ),
      edge( "Killer Instinct", rank_e::heroic, "Wins tied opposed rolls, may reroll opposed skill die if it comes up a '1'" ),
      edge( "Leader of Men", rank_e::veteran, edge_requirements( "Command" ), "Roll a d10 as the Wild Die for subordinates' group rolls" ),
      edge( "Level Headed", rank_e::seasoned, smarts_bag( dice_e::d8 ), "Act on best of two cards in combat" ),
      edge( "Improved Level Headed", rank_e::seasoned, "Level Headed", "Act on best of three cards in combat" ),
      edge( "Linguist", rank_e::novice, smarts_bag( dice_e::d6 ), "Begin play with a number of languages equal to Smarts, Smarts - 2 to be understood in any language heard for a week" ),
      edge( "Liquid Courage", rank_e::novice, vigour_bag( dice_e::d8 ), "Gain Vigour die type after imbibing at least 8 oz of alcohol" ),
      edge( "Luck", rank_e::novice, "", parse_modifiers( "Bennies + 1" ) ),
      edge( "Great Luck", rank_e::novice, "Luck", "", parse_modifiers( "Bennies + 2" ) ),
      edge( "Marksman", rank_e::seasoned, "Aim maneuver (+2 Shooting) if hero does not move" ),
      edge( "Martial Artist", rank_e::novice, skills_bag( "Fighting", dice_e::d6 ), "Never considered unarmed, +d4 to unarmed damage rolls" ),
      edge( "Improved Martial Artist", rank_e::veteran, "Martial Artist", skills_bag( "Fighting", dice_e::d10 ), "Never considered unarmed, +d6 to unarmed damage rolls" ),
      edge( "Martial Arts Master 1", rank_e::legendary, edge_requirements( skills_bag( "Fighting", dice_e::d12 ), "Improved Martial Artist" ), "+2 to unarmed damage rolls" ),
      edge( "Martial Arts Master 2", rank_e::legendary, "Martial Arts Master 1", "+4 to unarmed damage rolls" ),
      edge( "Martial Arts Master 3", rank_e::legendary, "Martial Arts Master 2", "+6 to unarmed damage rolls" ),
      edge( "Martial Arts Master 4", rank_e::legendary, "Martial Arts Master 3", "+8 to unarmed damage rolls" ),
      edge( "Martial Arts Master 5", rank_e::legendary, "Martial Arts Master 4", "+10 to unarmed damage rolls" ),
      edge( "McGyver", rank_e::novice, edge_requirements( smarts_bag( dice_e::d6 ), skills_bag( "Repair", dice_e::d6 ) + skills_bag( "Notice", dice_e::d8 ) ), "May improvise temporary gadgets" ),
      // TODO(jwerner) Mentalist
      edge( "Mighty Blow", rank_e::seasoned, skills_bag( "Fighting", dice_e::d10 ), "Double melee damage when dealt Joker" ),
      // TODO(jwerner) Mr. Fix It
      edge( "Natural Leader", rank_e::novice, edge_requirements( spirit_bag( dice_e::d8 ), "Command" ), "Leader may give Bennies to troops in command" ),
      edge( "Nerves of Steel", rank_e::novice, vigour_bag( dice_e::d8 ), "Ignore 1 point of wound penalties" ),
      edge( "Improved Nerves of Steel", rank_e::novice, "Nerves of Steel", "Ignore 2 points of wound penalties" ),
      // TODO(jwerner) New Power
      // TODO(jwerner) edge("Noble", rank_e::novice, "Rich, Status and Wealth", parse_modifiers("Charisma + 2")),
      edge( "No Mercy", rank_e::seasoned, "May spend Bennies on damage rolls" ),
      // TODO(jwerner) Power Points
      // TODO(jwerner) Power Surge
      // TODO(jwerner) Professional
      // TODO(jwerner) Expert
      // TODO(jwerner) Master
      edge( "Quick", rank_e::novice, agility_bag( dice_e::d8 ), "Discard draw of 5 or less for new card" ),
      edge( "Quick Draw", rank_e::novice, agility_bag( dice_e::d8 ), "May draw weapon as a free action" ),
      // TODO(jwerner) Rapid Recharge
      // TODO(jwerner) Improved Rapid Recharge
      // TODO(jwerner) Rich
      // TODO(jwerner) Filthy Rich
      edge( "Rock and Roll", rank_e::seasoned, skills_bag( "Shooting", dice_e::d8 ), "Ignore full-auto penalty if shooter doesn't move" ),
      edge( "Scavenger", rank_e::novice, edge_requirements( "Luck" ), "Find an essential piece of equipment once per session" ),
      // TODO(jwerner) Scholar
      edge( "Sidekick", rank_e::legendary, "Character gains a Novice WC sidekick" ),
      // TODO(jwerner) Soul Drain
      edge( "Steady Hands", rank_e::novice, agility_bag( dice_e::d8 ), "Ignore unstable platform penalty, Running penalty reduced to -1" ),
      edge( "Sweep", rank_e::novice, edge_requirements( strength_bag( dice_e::d8 ), skills_bag( "Fighting", dice_e::d8 ) ), "Attack all adjacent foes at -2" ),
      edge( "Improved Sweep", rank_e::veteran, "Sweep", "Attack all adjacent foes at no penalty" ),
      edge( "Strong Willed", rank_e::novice, skills_bag( "Intimidation", dice_e::d6 ) + skills_bag( "Taunt", dice_e::d6 ), "+2 to resist Tests of Will", parse_modifiers( "Intimidation + 2, Taunt + 2" ) ),
      // TODO(jwerner) Tactician
      edge( "Thief", rank_e::novice, edge_requirements( agility_bag( dice_e::d8 ), skills_bag( "Athletics", dice_e::d6 ) + skills_bag( "Lockpicking", dice_e::d6 ) + skills_bag( "Stealth", dice_e::d8 ) ), "+2 to disarm traps", parse_modifiers( "Athletics + 2, Lockpicking + 2, Stealth + 2" ) ),
      edge( "Tough as Nails", rank_e::legendary, "", parse_modifiers( "Toughness + 2" ) ),
      edge( "Improved Tough as Nails", rank_e::legendary, "Tough as Nails", "", parse_modifiers( "Toughness + 2" ) ),
      edge( "Trademark Weapon (Melee)", rank_e::novice, skills_bag( "Fighting", dice_e::d10 ), "+1 Fighting with a particular weapon" ),
      edge( "Trademark Weapon (Ranged)", rank_e::novice, skills_bag( "Shooting", dice_e::d10 ), "+1 Shooting with a particular weapon" ),
      edge( "Improved Trademark Weapon (Melee)", rank_e::veteran, "Trademark Weapon (Melee)", "+2 Fighting with a particular weapon" ),
      edge( "Improved Trademark Weapon (Ranged)", rank_e::veteran, "Trademark Weapon (Ranged)", "+2 Shooting with a particular weapon" ),
      edge( "Two-Fisted", rank_e::novice, agility_bag( dice_e::d8 ), "May attack with a weapon in each hand without a multi-action penalty" ),
      edge( "Weapon Master", rank_e::legendary, skills_bag( "Fighting", dice_e::d12 ), "", parse_modifiers( "Parry + 1" ) ),
      edge( "Master of Arms", rank_e::legendary, "Weapon Master", "", parse_modifiers( "Parry + 2" ) ),
      // TODO(jwerner) Wizard
      edge( "Woodsman", rank_e::novice, edge_requirements( spirit_bag( dice_e::d6 ), skills_bag( "Survival", dice_e::d8 ) + skills_bag( "Tracking", dice_e::d8 ) ), "", parse_modifiers( "Survival + 2, Tracking + 2, Stealth + 2" ) ),
    };
  }
  return *s_edges;
}

std::vector<const edge*> edge_manager::allowed_edges( const person& p ) {
  std::vector<const edge*> allowedEdges;
  for( const edge& e : get_edges() ) {
    if( p.meets_requirements( e ) && p.find_edge( e.name ) == nullptr && p.find_replaced_edge( e.name ) == nullptr ) {
      // Only accept edges that we meet the requirements for, we don't already have, and we haven't
      // replaced with a different edge.
      allowedEdges.push_back( &e );
    }
  }
  return allowedEdges;
}

const edge* edge_manager::random_allowed_edge( const person& p ) {
  std::vector<const edge*> allowedEdges = allowed_edges( p );
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
