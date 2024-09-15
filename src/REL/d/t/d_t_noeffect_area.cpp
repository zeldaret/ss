#include <d/tg/d_t_noeffect_area.h>

SPECIAL_ACTOR_PROFILE(TAG_NOEFFECT_AREA, dTgNoEffectArea_c, fProfile::TAG_NOEFFECT_AREA, 0x0158, 0, 0);

fLiMgBa_c dTgNoEffectArea_c::sList;

const f32 dTgNoEffectArea_c::sFloat1 = 100.0f;
const f32 dTgNoEffectArea_c::sFloat2 = 100.0f;

int dTgNoEffectArea_c::create() {
    scale *= 0.01f;
    PSMTXTrans(mMtx, position.x, position.y, position.z);
    mMtx.YrotM(rotation.y);
    PSMTXInverse(mMtx, mMtx);
    addToList(sList);
    return SUCCEEDED;
}

int dTgNoEffectArea_c::doDelete() {
    sList.remove(&this->mEntry);
    return 1;
}

int dTgNoEffectArea_c::actorExecute() {
    return 1;
}

int dTgNoEffectArea_c::draw() {
    return 1;
}

void dTgNoEffectArea_c::addToList(fLiMgBa_c &list) {
    list.append(&this->mEntry);
}

fLiNdBa_c *dTgNoEffectArea_c::getList() {
    return sList.getFirst();
}
