#include "d/a/npc/d_a_npc_knight_leader.h"

#include "d/d_sc_game.h"
#include "toBeSorted/event_manager.h"

static int lbl_5_data_0 = 0x00000000;
static int lbl_5_data_4 = 0xFFFFF05C;
static int lbl_5_data_8 = 0x0000038E;
static int lbl_5_data_C = 0xFFFFFC72;
static int lbl_5_data_10 = 0x0000071C;
static int lbl_5_data_14 = 0xFFFFF6C2;
static int lbl_5_data_18 = 0x00000E38;
static int lbl_5_data_1C = 0xFFFFF1C8;
static int lbl_5_data_20 = 0x00000666;
static int lbl_5_data_24 = 0xFFFFF6C2;
static int lbl_5_data_28 = 0x00000000;
static int lbl_5_data_2C = 0x00000000;

extern "C" const f32 lbl_5_rodata_0 = 1500.0f;
struct RoData4 {
    f32 f;
    s16 s1;
    s16 s2;
};
extern "C" const RoData4 lbl_5_rodata_4 = {1200.0f, 90, 12};
extern "C" const s16 lbl_5_rodata_C = 0x00FC;
extern "C" const s16 lbl_5_rodata_E = 0x0001;
extern "C" const s16 lbl_5_rodata_10 = 0x0258;
extern "C" const u8 lbl_5_rodata_12[0x3A] = {
    0x00, 0xCD, 0x00, 0x00,
    0xFF, 0xEA, 0x00, 0x05,
    0xFF, 0xFB, 0x00, 0x0A,
    0xFF, 0xF3, 0x00, 0x14,
    0xFF, 0xEC, 0x00, 0x09,
    0xFF, 0xF3, 0x00, 0x00,
    0x00, 0x00, 0x3D, 0x8F,
    0x5C, 0x29, 0x3D, 0xCC,
    0xCC, 0xCD, 0x3D, 0xF5,
    0xC2, 0x8F, 0x3D, 0xF5,
    0xC2, 0x8F, 0x3F, 0x80,
    0x00, 0x00, 0x00, 0x0C,
    0x01, 0x68, 0x00, 0x78,
    0x00, 0x3C, 0x00, 0x78,
    0x00, 0x00,
};
extern "C" const f32 lbl_5_rodata_4C = 1.0f;
extern "C" const u8 lbl_5_rodata_50[0x50] = {
    0x40, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00,
    0x43, 0x41, 0x00, 0x00,
    0x43, 0x7A, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x42, 0xDC, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x43, 0x87, 0x00, 0x00,
    0xC1, 0x8C, 0x00, 0x00,
    0xC0, 0xE0, 0x00, 0x00,
    0xBF, 0xE6, 0x66, 0x66,
    0x42, 0xA0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x3F, 0xA6, 0x66, 0x66,
    0x00, 0xB4, 0x00, 0xB4,
    0x44, 0x89, 0x80, 0x00,
    0x43, 0x5C, 0x00, 0x00,
    0x43, 0x5C, 0x00, 0x00,
    0xC3, 0x48, 0x00, 0x00,
};
extern "C" const f32 lbl_5_rodata_A0 = 0.07f;
extern "C" const f32 lbl_5_rodata_A4 = 0.1f;
extern "C" const f32 lbl_5_rodata_A8 = 0.12f;
extern "C" const f32 lbl_5_rodata_AC = 1.0f;
extern "C" const f32 lbl_5_rodata_B0 = 193.0f;
extern "C" const f32 lbl_5_rodata_B4 = 250.0f;
extern "C" const f32 lbl_5_rodata_B8 = 0.0f;

extern "C" void fn_5_F0(void *self, s16 *out) {
    out[0] = lbl_5_data_0;
    out[1] = lbl_5_data_4;
}

extern "C" void fn_5_110(void *self, s16 *out) {
    out[0] = lbl_5_data_8;
    out[1] = lbl_5_data_C;
}

extern "C" void fn_5_130(void *self, s16 *out) {
    out[0] = lbl_5_data_10;
    out[1] = lbl_5_data_14;
}

extern "C" void fn_5_150(void *self, s16 *out) {
    out[0] = lbl_5_data_18;
    out[1] = lbl_5_data_1C;
}

extern "C" void fn_5_170(void *self, s16 *out) {
    out[0] = lbl_5_data_20;
    out[1] = lbl_5_data_24;
}

extern "C" void fn_5_190(void *self, s16 *out) {
    out[0] = lbl_5_data_28;
    out[1] = lbl_5_data_2C;
}

extern "C" const s16 *fn_5_1B0(void *self) {
    return &lbl_5_rodata_C;
}

extern "C" const s16 *fn_5_1C0(void *self) {
    return &lbl_5_rodata_E;
}

extern "C" const s16 *fn_5_1D0(void *self) {
    return &lbl_5_rodata_10;
}

extern "C" const void *fn_5_1E0(void *self) {
    return lbl_5_rodata_12;
}

extern "C" void fn_5_1F0(void *self, f32 *p1, f32 *p2) {
    *p1 = lbl_5_rodata_A0;
    *p2 = lbl_5_rodata_A4;
}

extern "C" void fn_5_210(void *self, f32 *p1, f32 *p2) {
    *p1 = lbl_5_rodata_A8;
    *p2 = lbl_5_rodata_A8;
}

extern "C" f32 fn_5_230(void *self) {
    return lbl_5_rodata_AC;
}

extern "C" void fn_5_240(s16 *out) {
    *out = 0x888;
}

extern "C" f32 fn_5_250(void *self) {
    return lbl_5_rodata_AC;
}

extern "C" f32 fn_5_260(void *self) {
    return lbl_5_rodata_B0;
}

extern "C" f32 fn_5_270(void *self) {
    return lbl_5_rodata_B4;
}

struct TargetObj {
    s16 angle0;
    s16 angle1;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
};

struct SubBase {
    s16 mField0;
};

struct SubClass : public SubBase {
    virtual TargetObj *getTarget();
    TargetObj *mpTarget;
};

static inline TargetObj *getSubTarget(void *self) {
    return ((SubClass *)((char *)self + 0x1054))->getTarget();
}

extern "C" f32 fn_5_280(void *self) {
    return getSubTarget(self)->f4;
}

extern "C" void *fn_5_2C0(void *self) {
    return *(void **)((char *)self + 8);
}

extern "C" void fn_5_2D0(s16 *out, void *self) {
    *out = getSubTarget(self)->angle0 * 0xB6;
}

extern "C" f32 fn_5_320(void *self) {
    return getSubTarget(self)->f8;
}

extern "C" f32 fn_5_360(void *self) {
    return getSubTarget(self)->fC;
}

extern "C" f32 fn_5_3A0(void *self) {
    return getSubTarget(self)->f10;
}

extern "C" void fn_5_3E0(s16 *out, void *self) {
    *out = getSubTarget(self)->angle1 * 0xB6;
}

SPECIAL_ACTOR_PROFILE(NPC_KNIGHT_LEADER, dAcNpcKnightLeader_c, fProfile::NPC_KNIGHT_LEADER, 0x8F, 0, 2);

static const char *sEvents[] = {"LordsLogComment", nullptr};

void dAcNpcKnightLeader_c::triggerLordsLogCommentEvent(dAcNpcKnightLeader_c *actor) {
    {
        Event event = Event(sEvents[0], 0x190, 0x100001, nullptr, nullptr);
        EventManager::alsoSetAsCurrentEvent(actor, &event, nullptr);
    }
    actor->field_0x104C = dScGame_c::getUpdateFrameCount();
}
