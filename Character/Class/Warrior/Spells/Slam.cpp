
#include "Slam.h"
#include "Warrior.h"
#include "WarriorSpells.h"
#include "MainhandAttack.h"
#include "OffhandAttack.h"
#include "CharacterStats.h"
#include "CooldownReady.h"
#include "Flurry.h"
#include "DeepWounds.h"
#include "OverpowerBuff.h"
#include "CastComplete.h"

Slam::Slam(Character* pchar) :
    Spell("Slam", pchar, true, 0.0, 15),
    warr(dynamic_cast<Warrior*>(pchar)),
    cast_time(1500)
{
    talent_ranks = {1500, 1400, 1300, 1200, 1100, 1000};
}

void Slam::increase_effect_via_talent() {
    ++rank_talent;
    assert(rank_talent < talent_ranks.size());
    cast_time = talent_ranks[rank_talent];
}

void Slam::decrease_effect_via_talent() {
    --rank_talent;
    assert(rank_talent >= 0);
    cast_time = talent_ranks[rank_talent];
}

double Slam::get_cast_time() const {
    return double(cast_time) / 1000;
}

void Slam::spell_effect() {
    add_gcd_event();

    auto* new_event = new CastComplete(this, engine->get_current_priority() + get_cast_time());
    this->engine->add_event(new_event);

    pchar->stop_attack();
}

void Slam::perform_periodic() {
    const int result = roll->get_melee_ability_result(warr->get_mh_wpn_skill(), pchar->get_stats()->get_mh_crit_chance());

    if (result == AttackResult::MISS) {
        increment_miss();
        warr->lose_rage(static_cast<unsigned>(resource_cost));
        return;
    }
    if (result == AttackResult::DODGE) {
        increment_dodge();
        warr->get_overpower_buff()->apply_buff();
        warr->lose_rage(static_cast<unsigned>(round(resource_cost * 0.25)));
        return;
    }
    if (result == AttackResult::PARRY) {
        increment_parry();
        warr->lose_rage(static_cast<unsigned>(round(resource_cost * 0.25)));
        return;
    }

    double damage_dealt = damage_after_modifiers(warr->get_random_normalized_mh_dmg() + 87);

    if (result == AttackResult::CRITICAL) {
        damage_dealt *= warr->get_ability_crit_dmg_mod();
        warr->melee_mh_yellow_critical_effect();
        add_crit_dmg(static_cast<int>(round(damage_dealt)));
    }
    else if (result == AttackResult::HIT) {
        warr->melee_mh_yellow_hit_effect();
        add_hit_dmg(static_cast<int>(round(damage_dealt)));
    }

    warr->lose_rage(static_cast<unsigned>(resource_cost));

    pchar->get_spells()->get_mh_attack()->reset_swingtimer();
    pchar->get_spells()->get_oh_attack()->reset_swingtimer();
    pchar->start_attack();
    pchar->add_player_reaction_event();
}