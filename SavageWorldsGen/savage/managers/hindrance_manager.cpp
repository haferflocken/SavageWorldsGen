#include "../../savage/managers/hindrance_manager.hpp"

#include <cstdlib>

using namespace savage;

std::vector<hindrance>* hindrance_manager::s_hindrances = nullptr;
std::vector<hindrance>* hindrance_manager::s_majorHindrances = nullptr;
std::vector<hindrance>* hindrance_manager::s_minorHindrances = nullptr;

const std::vector<hindrance>& hindrance_manager::get_hindrances() {
  if( s_hindrances == nullptr ) {
    s_hindrances = new std::vector<hindrance> {
      hindrance( "All Thumbs", hindrance_severity_e::minor, parse_modifiers( "Repair - 2" ), "Roll of 1 casues malfunction" ),
      hindrance( "Anemic", hindrance_severity_e::minor, parse_modifiers( "FatigueChecks - 2" ) ),
      hindrance( "Arrogant", hindrance_severity_e::major, "Must humiliate opponent, challenge the 'leader'" ),
      hindrance( "Bad Eyes (Major)", hindrance_severity_e::major, "-2 to attack or notice something more than 5\" distant" ),
      hindrance( "Bad Eyes (Minor)", hindrance_severity_e::minor, "-2 to attack or notice something more than 5\" distant" ),
      hindrance( "Bad Luck", hindrance_severity_e::major, parse_modifiers( "Bennies - 1" ) ),
      hindrance( "Big Mouth", hindrance_severity_e::minor, "Unable to keep a secret, blabs at the worst time" ),
      hindrance( "Blind", hindrance_severity_e::major, "-6 to all actions that rely on vision; -2 on social rolls, gain additional Edge" ),
      hindrance( "Bloodthirsty", hindrance_severity_e::major, "Never takes prisoners" ),
      hindrance( "Cautious", hindrance_severity_e::minor, "Character is overly careful" ),
      hindrance( "Clueless", hindrance_severity_e::major, "-2 to most Common Knowledge rolls" ),
      hindrance( "Code of Honor", hindrance_severity_e::major, "Character keeps their word and acts like a gentleperson" ),
      hindrance( "Curious", hindrance_severity_e::major, "Character wants to know about everything" ),
      hindrance( "Death Wish", hindrance_severity_e::minor, "Hero wants to die after completing some task" ),
      hindrance( "Delusional (Major)", hindrance_severity_e::major, "Character suffers from grave delusions" ),
      hindrance( "Delusional (Minor)", hindrance_severity_e::minor, "Character suffers from grave delusions" ),
      hindrance( "Doubting Thomas", hindrance_severity_e::minor, "Character doesn't believe in the supernatural" ),
      hindrance( "Elderly", hindrance_severity_e::major, parse_modifiers( "Pace - 1, Strength - 1dt, Vigour - 1dt, SmartsSkillPoints + 5" ) ),
      hindrance( "Enemy (Major)", hindrance_severity_e::major, "Character has a recurring nemesis of some sort" ),
      hindrance( "Enemy (Minor)", hindrance_severity_e::minor, "Character has a recurring nemesis of some sort" ),
      hindrance( "Greedy (Major)", hindrance_severity_e::major, "Character is obsessed with wealth" ),
      hindrance( "Greedy (Minor)", hindrance_severity_e::minor, "Character is obsessed with wealth" ),
      hindrance( "Habit (Major)", hindrance_severity_e::major, parse_modifiers( "Charisma - 1" ), "Fatigue rolls when deprived" ),
      hindrance( "Habit (Minor)", hindrance_severity_e::minor, parse_modifiers( "Charisma - 1" ) ),
      hindrance( "Hard of Hearing (Major)", hindrance_severity_e::major, "Automatic failure to notice sounds" ),
      hindrance( "Hard of Hearing (Minor)", hindrance_severity_e::minor, "-2 to Notice sounds" ),
      hindrance( "Heroic", hindrance_severity_e::major, "Character always helps those in need" ),
      hindrance( "Illiterate", hindrance_severity_e::minor, "Unable to read or write" ),
      hindrance( "Lame", hindrance_severity_e::major, parse_modifiers( "Pace - 2, Run = d4" ) ),
      hindrance( "Loyal", hindrance_severity_e::minor, "The hero tries to never betray or disappoint their friends" ),
      hindrance( "Mean", hindrance_severity_e::minor, parse_modifiers( "Charisma - 2" ), "Ill-tempered and surly" ),
      hindrance( "Obese", hindrance_severity_e::minor, parse_modifiers( "Toughness + 1, Pace - 1, Run = d4" ) ),
      hindrance( "One Arm", hindrance_severity_e::major, "-2 to tasks requiring two arms" ),
      hindrance( "One Eye", hindrance_severity_e::major, parse_modifiers( "Charisma - 1" ), "-2 to rolls requiring depth perception" ),
      hindrance( "One Leg", hindrance_severity_e::major, parse_modifiers( "Pace - 2, Run = d4" ), "-2 to rolls requiring mobility, -2 to swimming rolls" ),
      hindrance( "Outsider", hindrance_severity_e::minor, parse_modifiers( "Charisma - 2" ), "Treated badly by those of dominant society" ),
      hindrance( "Overconfident", hindrance_severity_e::major, "The hero believes he can do anything" ),
      hindrance( "Pacifist (Major)", hindrance_severity_e::major, "Character won't harm living creatures" ),
      hindrance( "Pacifist (Minor)", hindrance_severity_e::minor, "Character fights only in self defense" ),
      hindrance( "Phobia (Major)", hindrance_severity_e::major, "-4 to Trait tests when near the phobia" ),
      hindrance( "Phobia (Minor)", hindrance_severity_e::minor, "-2 to Trait tests when near the phobia" ),
      hindrance( "Poverty", hindrance_severity_e::minor, "Half starting funds, inability to hang onto future income" ),
      hindrance( "Quirk", hindrance_severity_e::minor, "Character has some minor but persistent foible" ),
      hindrance( "Small", hindrance_severity_e::major, parse_modifiers( "Toughness - 1" ) ),
      hindrance( "Stubborn", hindrance_severity_e::minor, "Hero always wants his way" ),
      hindrance( "Ugly", hindrance_severity_e::minor, parse_modifiers( "Charisma - 2" ) ),
      hindrance( "Vengeful (Major)", hindrance_severity_e::major, "Character holds a grudge and will kill due to it" ),
      hindrance( "Vengeful (Minor)", hindrance_severity_e::minor, "Character holds a grudge" ),
      hindrance( "Vow (Major)", hindrance_severity_e::major, "A pledge to a group, deity, or religion" ),
      hindrance( "Vow (Minor)", hindrance_severity_e::minor, "A pledge to a group, deity, or religion" ),
      hindrance( "Wanted (Major)", hindrance_severity_e::major, "The character is a criminal of some sort" ),
      hindrance( "Wanted (Minor)", hindrance_severity_e::minor, "The character is a criminal of some sort" ),
      hindrance( "Yellow", hindrance_severity_e::major, parse_modifiers( "FearChecks - 2" ), "The character is cowardly" ),
      hindrance( "Young", hindrance_severity_e::major, parse_modifiers( "AttributePoints = 3, SkillPoints = 10, Bennies + 1" ) )
    };
  }
  return *s_hindrances;
}

void filter_by_severity( const std::vector<hindrance>& hindrances, hindrance_severity_e severity, std::vector<hindrance>& outResult ) {
  for( const hindrance& h : hindrances ) {
    if( h.severity == severity ) {
      outResult.push_back( h );
    }
  }
}

const std::vector<hindrance>& hindrance_manager::get_major_hindrances() {
  if( s_majorHindrances == nullptr ) {
    s_majorHindrances = new std::vector<hindrance>();
    filter_by_severity( get_hindrances(), hindrance_severity_e::major, *s_majorHindrances );
  }
  return *s_majorHindrances;
}

const std::vector<hindrance>& hindrance_manager::get_minor_hindrances() {
  if( s_minorHindrances == nullptr ) {
    s_minorHindrances = new std::vector<hindrance>();
    filter_by_severity( get_hindrances(), hindrance_severity_e::minor, *s_minorHindrances );
  }
  return *s_minorHindrances;
}

const hindrance& hindrance_manager::random_major_hindrance() {
  std::size_t i = rand() % get_major_hindrances().size();
  return get_major_hindrances()[i];
}

const hindrance& hindrance_manager::random_minor_hindrance() {
  std::size_t i = rand() % get_minor_hindrances().size();
  return get_minor_hindrances()[i];
}

void hindrance_manager::read_hindrances( const std::string& filePath ) {
  if( s_hindrances != nullptr ) {
    delete s_hindrances;
  }
  if( s_majorHindrances != nullptr ) {
    delete s_majorHindrances;
  }
  if( s_minorHindrances != nullptr ) {
    delete s_minorHindrances;
  }

  // TODO(jwerner) Read the hindrances from the file.
  s_hindrances = new std::vector<hindrance>();
}
