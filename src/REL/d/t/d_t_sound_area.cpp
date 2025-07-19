#include "d/t/d_t_sound_area.h"

#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_m3d_g_cps.h"
#include "d/snd/d_snd_3d_manager.h"
#include "d/t/d_t_sound_area_mgr.h"
#include "rvl/MTX.h" // IWYU pragma: export

SPECIAL_ACTOR_PROFILE(TAG_SOUND_AREA, dTgSndAr_c, fProfile::TAG_SOUND_AREA, 0x0146, 0, 0);

void float_ordering() {
    f32 arr[6] = {0, 100, 100, 100, 100, 100};
    0.01f;
}

int dTgSndAr_c::create() {
    mScale *= 0.01f;
    if (dTgSndMg_c::GetInstance() == nullptr) {
        dAcObjBase_c::createActorUnkGroup3(fProfile::SOUND_AREA_MGR, roomid, 0, nullptr, nullptr, nullptr, -1);
    }

    switch (getTypeFromParams()) {
        case 0:
            PSMTXTrans(mtx.m, position.x, position.y, position.z);
            mtx.YrotM(rotation.y);
            PSMTXInverse(mtx.m, mtx.m);
            break;
        case 3: mRail.initWithPathIndex(params >> 8 & 0xFF, roomid, 0); break;
    }

    fBase_c *base = nullptr;
    while (true) {
        if ((base = fManager_c::searchBaseByGroupType(ACTOR, base)) == nullptr) {
            break;
        }
        dAcBase_c *ac = static_cast<dAcBase_c *>(base);
        if (!ac->isActorPlayer() && checkPosInArea(ac->position)) {
            ac->setBit_field_0xE8(params & 0xFF);
        }
    }
    return SUCCEEDED;
}

int dTgSndAr_c::doDelete() {
    return SUCCEEDED;
}

int dTgSndAr_c::actorExecute() {
    dAcBase_c *link = dAcPy_c::LINK;
    if (link != nullptr && checkPosInArea(link->position)) {
        link->setBit_field_0xE8(params & 0xFF);
    }
    if (dSnd3DManager_c::GetInstance() != nullptr) {
        mVec3_c pos(dSnd3DManager_c::GetInstance()->getCameraTargetPos());
        if (checkPosInArea(pos) && dTgSndMg_c::GetInstance() != nullptr) {
            dTgSndMg_c::GetInstance()->setBgmFlag(params & 0xFF);
        }
    }
    return SUCCEEDED;
}

int dTgSndAr_c::draw() {
    return SUCCEEDED;
}

bool dTgSndAr_c::checkPosInArea(mVec3_c &pos) {
    switch (getTypeFromParams()) {
        case 0: return checkAlg0(pos);
        case 1: return checkAlg1(pos);
        case 2: return checkAlg2(pos);
        case 3: return checkAlg3(pos);
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
    f32 sxLower = -50.0f * mScale.x;
    f32 sxUpper = 50.0f * mScale.x;
    f32 syLower = 0.0f;
    f32 syUpper = 100.0f * mScale.y;
    f32 szLower = -50.0f * mScale.z;
    f32 szUpper = 50.0f * mScale.z;

    if (sxLower <= c2.x && c2.x <= sxUpper && syLower <= c2.y && c2.y <= syUpper && szLower <= c2.z &&
        c2.z <= szUpper) {
        return true;
    }
    return false;
}

// Sphere
bool dTgSndAr_c::checkAlg1(const mVec3_c &pos) {
    f32 tgtDist = mScale.x * 100.0f;
    f32 tgtDist2 = tgtDist * tgtDist;
    return PSVECSquareDistance(position, pos) < tgtDist2;
}

// Cylinder
bool dTgSndAr_c::checkAlg2(const mVec3_c &pos) {
    if (pos.y < position.y || pos.y > (position.y + 100.0f * mScale.y)) {
        return false;
    }

    f32 radius = mScale.x * 100.0f;
    mVec3_c diff = pos - position;

    f32 dist = diff.x * diff.x + diff.z * diff.z;
    f32 r2 = radius * radius;

    return !(dist > r2);
}

struct UnkStruct {
    /* 0x00 */ mVec3_c vec;
    /* 0x0C */ mVec3_c vec2;
    /* 0x18 */ u32 unk;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ u8 field_0x20;
    /* 0x24 */ u32 field_0x24;
};

// Capsule
bool dTgSndAr_c::checkAlg3(const mVec3_c &pos) {
    mVec3_c q;

    f32 radius = mScale.x * 100.0f;
    radius = radius * radius;
    nw4r::math::VEC3 a = pos;
    cM3dGCps unk;

    // Line between b and c
    mVec3_c b;
    b.copyFrom(mRail.getPoint(0));
    mVec3_c c;
    c.copyFrom(mRail.getPoint(1));

    unk.Set(b, c, mScale.x * 100.0f);
    f32 d;
    if (cM3d_Len3dSqPntAndSegLine(unk, a, q, &d, nullptr)) {
        // At the cylindrical part of the capsule, just check the distance to
        // the line
        return d < radius;
    } else {
        // Otherwise check if we are within the spheres around the endpoints
        f32 distSq = PSVECSquareDistance(unk.GetStart(), pos);
        if (distSq < radius) {
            return true;
        } else {
            distSq = PSVECSquareDistance(unk.GetEnd(), pos);
            return distSq < radius;
        }
    }
}
