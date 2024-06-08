#include <d/a/obj/d_a_obj_base.h>
#include <d/d_player.h>
#include <d/tg/d_t_sound_area.h>
#include <rvl/MTX.h>

SPECIAL_ACTOR_PROFILE(TAG_SOUND_AREA, dTgSndAr_c, fProfile::TAG_SOUND_AREA, 0x0146, 0, 0);

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
    f32 x;
    f32 y;
    f32 z;
};

extern Unk *lbl_80575D58;

int dTgSndAr_c::actorExecute() {
    dAcBase_c *link = dPlayer::LINK;
    if (link != nullptr && checkPosInArea(link->position)) {
        link->setBit_field_0xE8(params & 0xFF);
    }
    if (lbl_80575D58 != nullptr) {
        // Halp
        mVec3_c pos(lbl_80575D58->x, lbl_80575D58->y, lbl_80575D58->z);
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

// Box
bool dTgSndAr_c::checkAlg0(mVec3_c &pos) {
    mVec3_c c2 = pos;
    PSMTXMultVec(mtx.m, c2, c2);
    f32 scaleX = scale.x;
    f32 scaleZ = scale.z;
    if (c2.x >= scaleX * -50.0f && c2.x <= scaleX * 50.0f && c2.y >= 0.0f && c2.y <= scale.y * 100.0f &&
            c2.z >= scaleZ * -50.0f && c2.z <= scaleZ * 50.0f) {
        return true;
    }

    return false;
}

// Sphere
bool dTgSndAr_c::checkAlg1(mVec3_c &pos) {
    f32 tgtDist = scale.x * 100.0f;
    f32 tgtDist2 = tgtDist * tgtDist;
    return PSVECSquareDistance(position, pos) < tgtDist2;
}

// Cylinder
bool dTgSndAr_c::checkAlg2(mVec3_c &pos) {
    f32 f3 = position.x;
    if (pos.y < f3 || (f3 + scale.y * 100.0f < pos.y)) {
        return false;
    }

    f32 f1 = position.z;
    f32 f2 = position.x;
    f3 = scale.x * 100.0f;
    return f2 * f2 + f1 * f1 <= f3 * f3;
}

// ???
bool dTgSndAr_c::checkAlg3(mVec3_c &pos) {}
