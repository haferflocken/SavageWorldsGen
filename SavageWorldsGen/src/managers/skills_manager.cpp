#include "../../savage/managers/skills_manager.hpp"

using namespace savage;

std::vector<skill_definition>* skills_manager::s_skillDefinitions = nullptr;

const std::vector<skill_definition>& skills_manager::get_skill_definitions() {
  if( s_skillDefinitions == nullptr ) {
    s_skillDefinitions = new std::vector<skill_definition> {
      skill_definition( "Athletics", attributes_e::strength ),
      skill_definition( "Boating", attributes_e::agility ),
      skill_definition( "Driving", attributes_e::agility ),
      skill_definition( "Fighting", attributes_e::agility ),
      skill_definition( "Gambling", attributes_e::smarts ),
      skill_definition( "Healing", attributes_e::smarts ),
      skill_definition( "Intimidation", attributes_e::spirit ),
      skill_definition( "Investigation", attributes_e::smarts ),
      skill_definition( "Knowledge: Tactics", attributes_e::smarts ),
      skill_definition( "Lockpicking", attributes_e::agility ),
      skill_definition( "Notice", attributes_e::smarts ),
      skill_definition( "Persuasion", attributes_e::spirit ),
      skill_definition( "Piloting", attributes_e::agility ),
      skill_definition( "Repair", attributes_e::smarts ),
      skill_definition( "Riding", attributes_e::agility ),
      skill_definition( "Shooting", attributes_e::agility ),
      skill_definition( "Stealth", attributes_e::agility ),
      skill_definition( "Streetwise", attributes_e::smarts ),
      skill_definition( "Survival", attributes_e::smarts ),
      skill_definition( "Taunt", attributes_e::smarts ),
      skill_definition( "Tracking", attributes_e::smarts )
    };
  }
  return *s_skillDefinitions;
}
