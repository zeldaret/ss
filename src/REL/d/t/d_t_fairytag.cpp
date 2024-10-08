#include <c/c_math.h>
#include <d/t/d_t_fairytag.h>

SPECIAL_ACTOR_PROFILE(T_FAIRY, dTgFairy_c, fProfile::T_FAIRY, 0x0296, 0, 0);

bool dTgFairy_c::createHeap() {
    mpFairyRefs = new dAcRef_c<dAcObjFairy_c>[mCount]();
    return static_cast<bool>(mpFairyRefs);
}

int dTgFairy_c::create() {
    mCount = params & 0x3F;
    if (mCount == 0 || mCount == 7) {
        mCount = 1;
    }

    CREATE_ALLOCATOR(dTgFairy_c);
    createFairies();
    return SUCCEEDED;
}

int dTgFairy_c::doDelete() {
    if (mpFairyRefs != nullptr) {
        delete[] mpFairyRefs;
        mpFairyRefs = nullptr;
    }
    return SUCCEEDED;
}

int dTgFairy_c::actorExecute() {
    return SUCCEEDED;
}

int dTgFairy_c::draw() {
    return SUCCEEDED;
}

void dTgFairy_c::createFairies() {
    u32 parms = params;
    u32 someNum = (parms >> 6) & 0xFF;
    u32 fairyParams1 = (parms >> 14) & 0x3F;
    if (someNum > 100) {
        someNum = 100;
    }

    f32 f = 0.01f * someNum;

    for (int i = 0; i < mCount; i++) {
        if (mpFairyRefs[i].get() == nullptr) {
            // ?
            mVec3_c v2;
            mVec3_c v = calcLocation(f);
            v2 = v;
            dAcObjFairy_c *ac = static_cast<dAcObjFairy_c *>(
                    dAcObjBase_c::create("Fairy", roomid, fairyParams1, &v2, nullptr, nullptr, -1, -1, viewclip_index));
            if (ac != nullptr) {
                mpFairyRefs[i].link(ac);
            }
        }
    }
}

mVec3_c dTgFairy_c::calcLocation(const f32 &offset) {
    mVec3_c v;
    v.y = scale.y * 0.5f;
    v.x = scale.x * calcRnd(0.5f, offset);
    v.z = scale.z * calcRnd(0.5f, offset);
    v.rotY(rotation.y);
    return position + v;
}

float dTgFairy_c::calcRnd(const f32 &a, const f32 &b) {
    f32 f1 = 1.0f - (1.0f - b) * cM::rnd();
    f32 f2 = cM::rndFX(a);
    f2 *= f1;
    return f2;
}
