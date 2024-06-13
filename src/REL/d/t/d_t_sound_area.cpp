#include <d/a/obj/d_a_obj_base.h>
#include <d/d_player.h>
#include <d/tg/d_t_sound_area.h>
#include <rvl/MTX.h>

SPECIAL_ACTOR_PROFILE(TAG_SOUND_AREA, dTgSndAr_c, fProfile::TAG_SOUND_AREA, 0x0146, 0, 0);

void float_ordering() {
    f32 arr[6] = {0, 100, 100, 100, 100, 100};
    0.01f;
}

struct SoundAreaManager {
    u8 unk[0xFC];
    u32 bgmFlags;
};

extern SoundAreaManager *lbl_805756EC;

int dTgSndAr_c::create() {
    scale *= 0.01f;
    if (lbl_805756EC == nullptr) {
        dAcObjBase_c::createActorUnkGroup3(fProfile::SOUND_AREA_MGR, roomid, 0, nullptr, nullptr, nullptr, -1);
    }

    switch (getTypeFromParams()) {
    case 0:
        PSMTXTrans(mtx.m, position.x, position.y, position.z);
        mtx.YrotM(rotation.y);
        PSMTXInverse(mtx.m, mtx.m);
        break;
    case 3:
        mRail.init(params >> 8 & 0xFF, roomid, 0);
        break;
    }

    fBase_c *base = nullptr;
    while (true) {
        if ((base = fManager_c::searchBaseByGroupType(ACTOR, base)) == nullptr) {
            break;
        }
        // This has got to be just a member function
        dAcBase_c *ac = static_cast<dAcBase_c *>(base);
        if (!dBase_c::isActorPlayer(*ac) && checkPosInArea(ac->position)) {
            ac->setBit_field_0xE8(params & 0xFF);
        }
    }
    return 1;
}

int dTgSndAr_c::doDelete() {
    return 1;
}

struct Unk {
    u8 unk[0x144];
    mVec3_c v;
};

extern Unk *lbl_80575D58;

int dTgSndAr_c::actorExecute() {
    dAcBase_c *link = dPlayer::LINK;
    if (link != nullptr && checkPosInArea(link->position)) {
        link->setBit_field_0xE8(params & 0xFF);
    }
    if (lbl_80575D58 != nullptr) {
        mVec3_c pos = lbl_80575D58->v;
        if (checkPosInArea(pos) && lbl_805756EC != nullptr) {
            lbl_805756EC->bgmFlags |= 1 << (params & 0xFF);
        }
    }
    return 1;
}

int dTgSndAr_c::draw() {
    return 1;
}

bool dTgSndAr_c::checkPosInArea(mVec3_c &pos) {
    switch (getTypeFromParams()) {
    case 0:
        return checkAlg0(pos);
    case 1:
        return checkAlg1(pos);
    case 2:
        return checkAlg2(pos);
    case 3:
        return checkAlg3(pos);
    }

    return false;
}

inline bool inRange(f32 val, f32 tolerance) {
    return (-tolerance <= val && val <= tolerance);
}

// Box
bool dTgSndAr_c::checkAlg0(const mVec3_c &pos) {
    mVec3_c c2 = pos;
    PSMTXMultVec(mtx.m, c2, c2);
    f32 sxLower = -50.0f * scale.x;
    f32 sxUpper = 50.0f * scale.x;
    f32 syLower = 0.0f;
    f32 syUpper = 100.0f * scale.y;
    f32 szLower = -50.0f * scale.z;
    f32 szUpper = 50.0f * scale.z;

    if (sxLower <= c2.x && c2.x <= sxUpper && syLower <= c2.y && c2.y <= syUpper && szLower <= c2.z &&
            c2.z <= szUpper) {
        return true;
    }
    return false;
}

// Sphere
bool dTgSndAr_c::checkAlg1(const mVec3_c &pos) {
    f32 tgtDist = scale.x * 100.0f;
    f32 tgtDist2 = tgtDist * tgtDist;
    return PSVECSquareDistance(position, pos) < tgtDist2;
}

// Cylinder
bool dTgSndAr_c::checkAlg2(const mVec3_c &pos) {
    if (pos.y < position.y || pos.y > (position.y + 100.0f * scale.y)) {
        return false;
    }

    mVec3_c diff(pos.x - position.x, pos.z - position.z, scale.x * 100.0f);

    return diff.x * diff.x + diff.y * diff.y <= diff.z * diff.z;
}

struct UnkStruct {
    /* 0x00 */ mVec3_c vec;
    /* 0x0C */ mVec3_c vec2;
    /* 0x18 */ u32 unk;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ u8 field_0x20;
    /* 0x24 */ u32 field_0x24;
};

extern "C" void fn_80337EA0(UnkStruct *);
extern "C" void fn_80337EF0(UnkStruct *, mVec3_c&, mVec3_c&, f32);
extern "C" int fn_8032BFB0(UnkStruct *, mVec3_c&, f32*, f32&, int);

// ???
bool dTgSndAr_c::checkAlg3(const mVec3_c &pos) {
    f32 q[4];
    UnkStruct unk;

    f32 radius = scale.x * 100.0f;
    radius = radius * radius;
    mVec3_c a = pos;
    fn_80337EA0(&unk);

    mVec3_c b = *mRail.getPntPosForIndex(0);
    mVec3_c c = *mRail.getPntPosForIndex(1);

    fn_80337EF0(&unk, b, c, scale.x * 100.0f);
    f32 d;
    if (fn_8032BFB0(&unk, a, q, d, 0)) {
        return d < radius;
    } else {
        f32 distSq = PSVECSquareDistance(unk.vec, pos);
        if (distSq < radius) {
            return true;
        } else {
            distSq = PSVECSquareDistance(unk.vec2, pos);
            return distSq < radius;
        }
    }
}
