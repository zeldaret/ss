#pragma once

#include <common.h>

// MOST LIKELY mVEC
struct Vec3f {
    f32 x, y, z;
};

struct Vec2f {
    f32 x, y;
};

struct Vec3s {
    s16 x, y, z;
};

struct ObjInfo {
    /* 0x00 */ char *name;
    /* 0x04 */ u16 obj_id;
    /* 0x06 */ u16 obj_id2;
    /* 0x08 */ u16 fiTextEntryId;
    /* 0x0A */ u8 unk_0xA;
    /* 0x0B */ u8 subtype;
};

// is now fLiNdBa_c
// struct ActorReference {
//     ActorReference* prev;
//     ActorReference* next;
//     fBase_c* link;
// };

class UnkCollider {};
