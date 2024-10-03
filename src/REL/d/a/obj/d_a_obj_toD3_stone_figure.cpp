#include <d/a/obj/d_a_obj_toD3_stone_figure.h>
#include <d/cc/d_cc_mgr.h>
#include <toBeSorted/attention.h>
#include <toBeSorted/event.h>
#include <toBeSorted/event_manager.h>
#include <toBeSorted/item_story_flag_manager.h>
#include <toBeSorted/scgame.h>


SPECIAL_ACTOR_PROFILE(OBJ_TOD3_STONE, dAcOtoD3StoneFigure_c, fProfile::OBJ_TOD3_STONE, 0x1B3, 0, 0);

STATE_DEFINE(dAcOtoD3StoneFigure_c, OneEye);
STATE_DEFINE(dAcOtoD3StoneFigure_c, Eyes);

f32 dAcOtoD3StoneFigure_c::sRadius = 85.0f;
f32 dAcOtoD3StoneFigure_c::sHeight = 290.0f;

// clang-format off
dCcD_SrcCyl dAcOtoD3StoneFigure_c::sCcSrc = {
    {{{0}, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {{0xFEB77DFF}, 0x1000111, 0x60407, 0, 0}, 
    {0xE9}},
    {dAcOtoD3StoneFigure_c::sRadius, dAcOtoD3StoneFigure_c::sHeight}
};
// clang-format on

bool dAcOtoD3StoneFigure_c::createHeap() {
    const char *modelName = getModelName();
    mResFile = getOarcResFile("BirdObjD3");
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(modelName);
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120));
    return true;
}

int dAcOtoD3StoneFigure_c::create() {
    mIsSkyKeepAlreadyOpen = isStoneOfTrialsPlaced();
    mExitId = params & 0xFF;
    updateMatrix();
    if (!initAllocatorWork1Heap(-1, "dAcOtoD3StoneFigure_c::m_allocator", 0x20)) {
        return FAILED;
    }

    mMdl.setLocalMtx(worldMatrix);
    mCCdStruct.setField0x38ToMagicValue();
    mCollision.init(sCcSrc);
    mCollision.initUnk(mCCdStruct);
    int zero = 0;
    mCollision.setC(position);
    mCollision.setR(dAcOtoD3StoneFigure_c::sRadius + zero);
    mCollision.setH(dAcOtoD3StoneFigure_c::sHeight + zero);
    ColliderManager::getColliderManager()->addCollider(&mCollision);
    // mCollision.clearCoFlag();
    mCollision.setTgCoFlag(1);

    // ???
    f32 a, b, c;
    c = position.z;
    b = getYPos();
    b += zero;
    a = position.x;
    poscopy2.x = a;
    poscopy2.y = b;
    poscopy2.z = c;
    poscopy3.x = a;
    poscopy3.y = b;
    poscopy3.z = c;
    // poscopy3 = poscopy2;
    mCollision.setSomeDefendValue(0x2000);

    if (!mIsSkyKeepAlreadyOpen) {
        mStateMgr.changeState(StateID_OneEye);
    } else {
        mStateMgr.changeState(StateID_Eyes);
    }

    return SUCCEEDED;
}

int dAcOtoD3StoneFigure_c::doDelete() {
    return SUCCEEDED;
}

int dAcOtoD3StoneFigure_c::actorExecute() {
    mStateMgr.executeState();
    ColliderManager::getColliderManager()->addCollider(&mCollision);
    return SUCCEEDED;
}

int dAcOtoD3StoneFigure_c::draw() {
    if (ScGame::currentSpawnInfo.layer != 0x12) {
        drawModelType1(&mMdl);
    }
    return SUCCEEDED;
}

void dAcOtoD3StoneFigure_c::initializeState_OneEye() {
    if (ScGame::currentSpawnInfo.night == 1) {
        rotation.y.mVal += -0x8000;
        updateMatrix();
        mMdl.setLocalMtx(worldMatrix);
    }
}

void dAcOtoD3StoneFigure_c::executeState_OneEye() {
    if (EventManager::isInEvent(this, "D3OpenStart")) {
        ScGame::sInstance->triggerExit(roomid, mExitId);
    } else {
        if (hasStoneOfTrials() && ScGame::currentSpawnInfo.night != 1) {
            // These are interaction related
            -0.0f;
            AttentionManager::sInstance->addExamineTalkTarget(*this, 1, 300.0f, 50.0f, -100.0f, 100.0f);
            AttentionManager::sInstance->addExamineTalkTarget(*this, 1, 200.0f, -100.0f, 100.0f);
        }
    }
}
void dAcOtoD3StoneFigure_c::finalizeState_OneEye() {}

void dAcOtoD3StoneFigure_c::initializeState_Eyes() {}
void dAcOtoD3StoneFigure_c::executeState_Eyes() {}
void dAcOtoD3StoneFigure_c::finalizeState_Eyes() {}

const char *dAcOtoD3StoneFigure_c::getModelName() const {
    return mIsSkyKeepAlreadyOpen ? "BirdObjD3B" : "BirdObjD3A";
}

bool dAcOtoD3StoneFigure_c::isStoneOfTrialsPlaced() const {
    return StoryFlagManager::sInstance->getCounterOrFlag(22);
}

void dAcOtoD3StoneFigure_c::doInteraction(s32 arg) {
    if (arg == 5) {
        Event event = Event("D3OpenStart", 100, 0x100001, nullptr, nullptr);
        EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
    }
}

extern "C" s32 AcItem__checkFlag(s32);
bool dAcOtoD3StoneFigure_c::hasStoneOfTrials() const {
    return AcItem__checkFlag(0xB4);
}
