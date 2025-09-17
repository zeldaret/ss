#include "d/a/obj/d_a_obj_stream_lava.h"

#include "common.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_room.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m3d/m_fanm.h"
#include "nw4r/g3d/res/g3d_resfile.h"

STATE_DEFINE(dAcOstreamLava_c, Wait);
STATE_DEFINE(dAcOstreamLava_c, Stream);

const f32 dAcOstreamLava_c::unkFloat0 = 0.0f;

static const char *const AcOstreamLava__OarcNames[] = {
    "FlowLavaD2", "FlowLavaD2", "FlowLavaLast_2", "FlowLavaLast_2", "FlowLavaLast_2",
};
static const char *const AcOstreamLava__ModelNames[] = {
    "FlowLavaD2_A", "FlowLavaD2_B", "FlowLavaLast_2A", "FlowLavaLast_2B", "FlowLavaLast_2C",
};
static const char *const AcOstreamLava__DbzNames[] = {
    "dzb/FlowLavaD2_A.dzb",    "dzb/FlowLavaD2_B.dzb",    "dzb/FlowLavaLast_2A.dzb",
    "dzb/FlowLavaLast_2B.dzb", "dzb/FlowLavaLast_2C.dzb",
};
static const char *const AcOstreamLava__PlcNames[] = {
    "dat/FlowLavaD2_A.plc",    "dat/FlowLavaD2_B.plc",    "dat/FlowLavaLast_2A.plc",
    "dat/FlowLavaLast_2B.plc", "dat/FlowLavaLast_2C.plc",
};
static const char *const AcOstreamLava__AnmClrNames[] = {
    "FlowLavaD2", "FlowLavaD2", "FlowLavaLast_2", "FlowLavaLast_2", "FlowLavaLast_2",
};

SPECIAL_ACTOR_PROFILE(OBJ_STREAM_LAVA, dAcOstreamLava_c, fProfile::OBJ_STREAM_LAVA, 0x1FA, 0, 6);

bool dAcOstreamLava_c::createHeap() {
    mSubtype = mParams & 0xF;
    mResFile = nw4r::g3d::ResFile(getOarcResFile(AcOstreamLava__OarcNames[mSubtype]));
    dStage_c::bindStageResToFile(&mResFile);

    nw4r::g3d::ResMdl mdl;
    nw4r::g3d::ResAnmTexSrt anmSrtWait;
    nw4r::g3d::ResAnmClr anmClr;

    mdl = mResFile.GetResMdl(AcOstreamLava__ModelNames[mSubtype]);
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x32C));

    anmSrtWait = mResFile.GetResAnmTexSrt("Wait");
    TRY_CREATE(mAnmTexSrtWait.create(mdl, anmSrtWait, &heap_allocator, nullptr, 1));
    mModel.setAnm(mAnmTexSrtWait);

    anmClr = mResFile.GetResAnmClr(AcOstreamLava__AnmClrNames[mSubtype]);
    TRY_CREATE(mAnmMatClr.create(mdl, anmClr, &heap_allocator, nullptr, 1));

    void *dzb = getOarcFile(AcOstreamLava__OarcNames[mSubtype], AcOstreamLava__DbzNames[mSubtype]);
    void *plc = getOarcFile(AcOstreamLava__OarcNames[mSubtype], AcOstreamLava__PlcNames[mSubtype]);
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);

    TRY_CREATE(!mCollision.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();

    TRY_CREATE(mCollision.InitMapStuff(&heap_allocator));
    return true;
}

int dAcOstreamLava_c::create() {
    CREATE_ALLOCATOR(dAcOstreamLava_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    dBgS::GetInstance()->RegistBg(&mCollision, this);

    mShouldStreamSceneflag = getFromParams(4, 0xFF); // (params >> 4) & 0xFF;
    mEventId = getFromParams(0xC, 0xFF);             // params >> 0xC;
    mHideActor = !getFromParams(0x14, 1);            // (params >> 0x14 & 1) == 0;
    mModel.setAnm(mAnmMatClr);

    bool shouldStream = SceneflagManager::sInstance->checkFlag(roomid, mShouldStreamSceneflag);
    if (shouldStream) {
        mStateMgr.changeState(StateID_Stream);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    mModel.setPriorityDraw(0x1C, 0x9);
    mVec3_c min, max;
    mModel.getBounds(&min, &max);
    boundingBox.Set(min, max);
    mCullingDistance = 50000.0f;

    int roomId_tmp = roomid;
    if (addActorToRoom(-1)) {
        roomid = roomId_tmp;
        changeLoadedEntitiesWithSet();
    }

    return SUCCEEDED;
}

int dAcOstreamLava_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mCollision);
    changeLoadedEntitiesNoSet();
    return SUCCEEDED;
}

int dAcOstreamLava_c::actorExecute() {
    mStateMgr.executeState();
    mAnmMatClr.play();

    dRoom_c *currentRoom = dStage_c::GetInstance()->getRoom(roomid);

    if (currentRoom->checkFlag(2)) {
        setObjectProperty(0x200);
        return SUCCEEDED;
    } else {
        clearObjectProperty(0x200);
        return SUCCEEDED;
    }
}

int dAcOstreamLava_c::draw() {
    if (!mHideActor) {
        drawModelType1(&mModel);
    }
    return SUCCEEDED;
}

void dAcOstreamLava_c::initializeState_Wait() {
    mAnmTexSrtWait.setAnm(mModel, mResFile.GetResAnmTexSrt("Wait"), 0, m3d::PLAY_MODE_4);
}

void dAcOstreamLava_c::executeState_Wait() {
    mAnmTexSrtWait.play();

    bool shouldStream = SceneflagManager::sInstance->checkFlag(roomid, mShouldStreamSceneflag);
    if (shouldStream) {
        mStateMgr.changeState(StateID_Stream);
    }
}

// BADLY MATCHING
// Using the potential improvement causes issues with the andc instruction
// Should be: andc r6, r0, r9
// Currently: andc r6, r9, r0
// https://decomp.me/scratch/3PKdt
void dAcOstreamLava_c::finalizeState_Wait() {
    if (mEventId != 0xFF) {
        // Hacky solution:
        static volatile u32 FLAGS_1 = 0x00000001;
        static u32 FLAGS_2 = 0x00100001;
        u32 f1 = FLAGS_1;
        u32 f2 = FLAGS_2;
        u32 eventFlag = f2 & ~f1;

        // Potential improvement:
        // Swaps the numbers in .data and swaps the operands in andc
        // u32 eventFlag = Event::makeEventFlag(0x100001, 0x1);

        Event ev(mEventId, roomid, eventFlag, nullptr, nullptr);
        mEvent.scheduleEvent(ev, 0);
    }
}

void dAcOstreamLava_c::initializeState_Stream() {
    mAnmTexSrtWait.setAnm(mModel, mResFile.GetResAnmTexSrt("Flow"), 0, m3d::PLAY_MODE_4);
}

void dAcOstreamLava_c::executeState_Stream() {
    mAnmTexSrtWait.play();

    bool shouldStream = SceneflagManager::sInstance->checkFlag(roomid, mShouldStreamSceneflag);
    if (!shouldStream) {
        mStateMgr.changeState(StateID_Wait);
    }
}

void dAcOstreamLava_c::finalizeState_Stream() {}
