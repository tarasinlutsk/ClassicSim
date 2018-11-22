#ifndef TESTEVISCERATE_H
#define TESTEVISCERATE_H

#include "TestSpellRogue.h"

class TestEviscerate: public TestSpellRogue {
public:
    TestEviscerate(EquipmentDb *equipment_db);

    void test_all();

    Eviscerate* eviscerate();

    void test_name_correct() override;
    void test_spell_cooldown() override;
    void test_incurs_global_cooldown() override;
    void test_obeys_global_cooldown() override;
    void test_resource_cost() override;
    void test_is_ready_conditions() override;
    void test_combo_points() override;
    void test_stealth() override;

    void test_hit_dmg_1_combo_points();
    void test_hit_dmg_2_combo_points();
    void test_hit_dmg_3_combo_points();
    void test_hit_dmg_4_combo_points();
    void test_hit_dmg_5_combo_points();

    void test_crit_dmg_1_combo_points();
    void test_crit_dmg_2_combo_points();
    void test_crit_dmg_3_combo_points();
    void test_crit_dmg_4_combo_points();
    void test_crit_dmg_5_combo_points();

    void when_eviscerate_is_performed();

private:
};

#endif // TESTEVISCERATE_H