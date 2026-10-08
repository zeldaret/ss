#include "d/a/b/d_a_b_girahimu2.h"

#include "c/c_math.h"
#include "s/s_State.hpp"
#include "toBeSorted/minigame_mgr.h"

STATE_DEFINE(dAcGirahimu2_c, CreateSpinKnife);
STATE_DEFINE(dAcGirahimu2_c, WaitKnifeAttack);
STATE_DEFINE(dAcGirahimu2_c, KnifeAttack);
STATE_DEFINE(dAcGirahimu2_c, KnifePreAttack);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, CatchDamage);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, Walk);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, Counter);
STATE_DEFINE(dAcGirahimu2_c, G_SwordDemo);
STATE_DEFINE(dAcGirahimu2_c, G_SwordWait);
STATE_DEFINE(dAcGirahimu2_c, G_SwordPiyori);
STATE_DEFINE(dAcGirahimu2_c, Death);
STATE_DEFINE(dAcGirahimu2_c, DeathMiniGame);
STATE_DEFINE(dAcGirahimu2_c, UpWarp);
STATE_DEFINE(dAcGirahimu2_c, UpAttack);
STATE_DEFINE(dAcGirahimu2_c, CreateKnife);
STATE_DEFINE(dAcGirahimu2_c, ComeOn);
STATE_DEFINE(dAcGirahimu2_c, ComeOnGuard);
STATE_DEFINE(dAcGirahimu2_c, GuardJustCounter);
STATE_DEFINE(dAcGirahimu2_c, G_SwordDamage); // Non Virtual Define??
STATE_DEFINE(dAcGirahimu2_c, TwoSwordAttack);
STATE_DEFINE(dAcGirahimu2_c, HitAction);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, Catch);

SPECIAL_ACTOR_PROFILE(B_GIRAHIMU2, dAcGirahimu2_c, fProfile::B_GIRAHIMU2, 0x10F, 0, 0);

struct dAcGirahimu2_HIO_c {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    s16 field_0x20;
    s16 field_0x22;
    s16 field_0x24;
    s16 field_0x26;

    static const dAcGirahimu2_HIO_c sHIO;
};

const dAcGirahimu2_HIO_c dAcGirahimu2_HIO_c::sHIO = {1.05f, 1.05f, -5.0f, -2.0f, 15.0f, 150.0f,
                                                     3.0f,  40.0f, 1,     0x5DC, 0x14,  0x1000};

void dAcGirahimu2_c::initializeState_G_SwordWait() {
    field_0xD8C = true;
    field_0xD6C = 6;
    mCyl2.OnTgSet();
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
    field_0xD46 = 30;
    if (vt_0x218()) {
        field_0x186C = 60.0f;
        field_0xD46 = 5;
    }
    if (cM::rndInt(100) > 50) {
        field_0xD7E = true;
    } else {
        field_0xD7E = false;
    }
}
void dAcGirahimu2_c::executeState_G_SwordWait() {}
void dAcGirahimu2_c::finalizeState_G_SwordWait() {}

void dAcGirahimu2_c::initializeState_CreateKnife() {}
void dAcGirahimu2_c::executeState_CreateKnife() {}
void dAcGirahimu2_c::finalizeState_CreateKnife() {}

void dAcGirahimu2_c::initializeState_KnifePreAttack() {}
void dAcGirahimu2_c::executeState_KnifePreAttack() {}
void dAcGirahimu2_c::finalizeState_KnifePreAttack() {}

void dAcGirahimu2_c::initializeState_KnifeAttack() {}
void dAcGirahimu2_c::executeState_KnifeAttack() {}
void dAcGirahimu2_c::finalizeState_KnifeAttack() {}

void dAcGirahimu2_c::initializeState_HitAction() {}
void dAcGirahimu2_c::executeState_HitAction() {}
void dAcGirahimu2_c::finalizeState_HitAction() {}

void dAcGirahimu2_c::initializeState_ComeOn() {}
void dAcGirahimu2_c::executeState_ComeOn() {}
void dAcGirahimu2_c::finalizeState_ComeOn() {}

void dAcGirahimu2_c::initializeState_ComeOnGuard() {}
void dAcGirahimu2_c::executeState_ComeOnGuard() {}
void dAcGirahimu2_c::finalizeState_ComeOnGuard() {}

void dAcGirahimu2_c::initializeState_G_SwordDemo() {}
void dAcGirahimu2_c::executeState_G_SwordDemo() {}
void dAcGirahimu2_c::finalizeState_G_SwordDemo() {}

void dAcGirahimu2_c::initializeState_G_SwordDamage() {}
void dAcGirahimu2_c::executeState_G_SwordDamage() {
    mCollider.findTgHit();
}
void dAcGirahimu2_c::finalizeState_G_SwordDamage() {}

void dAcGirahimu2_c::initializeState_UpWarp() {}
void dAcGirahimu2_c::executeState_UpWarp() {}
void dAcGirahimu2_c::finalizeState_UpWarp() {}

void dAcGirahimu2_c::initializeState_UpAttack() {}
void dAcGirahimu2_c::executeState_UpAttack() {}
void dAcGirahimu2_c::finalizeState_UpAttack() {}

void dAcGirahimu2_c::initializeState_Counter() {}
void dAcGirahimu2_c::executeState_Counter() {}
void dAcGirahimu2_c::finalizeState_Counter() {}

void dAcGirahimu2_c::initializeState_GuardJustCounter() {}
void dAcGirahimu2_c::executeState_GuardJustCounter() {}
void dAcGirahimu2_c::finalizeState_GuardJustCounter() {}

void dAcGirahimu2_c::initializeState_TwoSwordAttack() {}
void dAcGirahimu2_c::executeState_TwoSwordAttack() {}
void dAcGirahimu2_c::finalizeState_TwoSwordAttack() {}

void dAcGirahimu2_c::initializeState_G_SwordPiyori() {}
void dAcGirahimu2_c::executeState_G_SwordPiyori() {}
void dAcGirahimu2_c::finalizeState_G_SwordPiyori() {}

void dAcGirahimu2_c::initializeState_Death() {}
void dAcGirahimu2_c::executeState_Death() {}
void dAcGirahimu2_c::finalizeState_Death() {}

void dAcGirahimu2_c::fn_228_4550(int idxStart, int idxEnd) {}

void dAcGirahimu2_c::fn_228_47F0(bool) {}

void dAcGirahimu2_c::initializeState_DeathMiniGame() {}
void dAcGirahimu2_c::executeState_DeathMiniGame() {}
void dAcGirahimu2_c::finalizeState_DeathMiniGame() {}

void dAcGirahimu2_c::fn_228_4A30() {
    mMdlSwordA.mCcList.findAtHit();
}

void dAcGirahimu2_c::fn_228_4C20() {}

bool dAcGirahimu2_c::fn_228_4CD0(s32 cutDir) {}

void dAcGirahimu2_c::vt_0x220() {}

bool dAcGirahimu2_c::fn_228_5110() {}

void dAcGirahimu2_c::fn_228_5400() {}

void dAcGirahimu2_c::fn_228_5740() {}

bool dAcGirahimu2_c::fn_228_5770(s32 cutDir) {}

bool dAcGirahimu2_c::fn_228_5E90() {}

bool dAcGirahimu2_c::fn_228_5EC0() {}

void dAcGirahimu2_c::vt_0x1F4() {}

void dAcGirahimu2_c::vt_0x1F8() {}

void dAcGirahimu2_c::vt_0x20C() {}

bool dAcGirahimu2_c::createHeap() {}

int dAcGirahimu2_c::actorCreate() {
    MinigameManager::GetInstance()->checkInBossRush();
}

int dAcGirahimu2_c::doDelete() {}

void dAcGirahimu2_c::vt_0x1DC() {}

int dAcGirahimu2_c::actorExecute() {
    executeState();
    changeState(StateID_DeathMiniGame);
    getStateID().isEqual(StateID_DeathMiniGame);
}

void dAcGirahimu2_c::initializeState_Walk() {}
void dAcGirahimu2_c::executeState_Walk() {}
void dAcGirahimu2_c::finalizeState_Walk() {}

void dAcGirahimu2_c::initializeState_CreateSpinKnife() {}
void dAcGirahimu2_c::executeState_CreateSpinKnife() {}
void dAcGirahimu2_c::finalizeState_CreateSpinKnife() {}

void dAcGirahimu2_c::initializeState_CatchDamage() {}
void dAcGirahimu2_c::executeState_CatchDamage() {}
void dAcGirahimu2_c::finalizeState_CatchDamage() {}

void dAcGirahimu2_c::initializeState_WaitKnifeAttack() {}
void dAcGirahimu2_c::executeState_WaitKnifeAttack() {}
void dAcGirahimu2_c::finalizeState_WaitKnifeAttack() {}

void dAcGirahimu2_c::initializeState_Catch() {}
void dAcGirahimu2_c::executeState_Catch() {}
void dAcGirahimu2_c::finalizeState_Catch() {}

void dAcGirahimu2_c::fn_228_80A0() {}

void dAcGirahimu2_c::fn_228_80E0() {}

void dAcGirahimu2_c::fn_228_82C0() {}

void dAcGirahimu2_c::vt_0x1FC() {}

void dAcGirahimu2_c::vt_0x200() {}

void dAcGirahimu2_c::vt_0x204() {}

void dAcGirahimu2_c::vt_0x228() {}

void dAcGirahimu2_c::vt_0x22C() {}

bool dAcGirahimu2_c::vt_0x210() {}

void dAcGirahimu2_c::vt_0x230() {}

void dAcGirahimu2_c::vt_0x214() {}

bool dAcGirahimu2_c::vt_0x218() {}

void dAcGirahimu2_c::vt_0x234() {}
