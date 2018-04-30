
#include "Bloodrage.h"
#include "Warrior.h"
#include "ResourceGain.h"

Bloodrage::Bloodrage(Engine* engine, Character* pchar, CombatRoll* roll) :
    Spell("Bloodrage", engine, pchar, roll, 60, 0)
{
    this->pchar = dynamic_cast<Warrior*>(pchar);
    this->talent_ranks = {10, 12, 15};
    this->periodic_rage_base = 10;
    this->periodic_rage_current = periodic_rage_base;
}

int Bloodrage::spell_effect(const int) {
    add_spell_cd_event();

    ResourceGain* new_event = new ResourceGain(this, engine->get_current_priority() + 1.0);
    this->engine->add_event(new_event);
    periodic_rage_current = periodic_rage_base;

    return talent_ranks[rank_talent];
}

void Bloodrage::periodic_effect() {
    // TODO: Add statistics for periodic rage gain.
    pchar->gain_rage(1);
    --periodic_rage_current;

    if (periodic_rage_current == 0)
        return;

    ResourceGain* new_event = new ResourceGain(this, engine->get_current_priority() + 1.0);
    this->engine->add_event(new_event);
}