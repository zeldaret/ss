#include "d/d_stage_parse.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_base.h"
#include "d/d_bzs_types.h"
#include "d/d_room.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/flag_managers.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "sized_string.h"
#include "toBeSorted/file_manager.h"

struct BzsHeader {
    /* 0x00 */ u32 mName;
    /* 0x04 */ u16 mSectionCount;
    /* 0x08 */ u32 mOffset;
};

struct BzsSectionHead {
    /* 0x00 */ u32 mTag;
    /* 0x04 */ u16 mCount;
    /* 0x08 */ u32 mOffset;
};

typedef void (*BzsParseFunc)(int roomid, const BzsSectionHead *section);

struct BzsTagToParseFunc {
    /* 0x00 */ u32 mTag;
    /* 0x04 */ BzsParseFunc mParseFunc;
};

#define OFS_TO_PTR(ty, section)                                                                                        \
    (reinterpret_cast<const ty *>(reinterpret_cast<const char *>(section) + section->mOffset))

static void findAndRunStageTagParseFunc(
    int roomid, const BzsSectionHead *head, const BzsTagToParseFunc *handlerTable, s32 tableSize
);

static void handleBzsPcam(int roomid, const BzsSectionHead *section) {
    dStageMgr_c::GetInstance()->setPcam(OFS_TO_PTR(PCAM, section), section->mCount);
}

static void handleBzsLyse(int roomid, const BzsSectionHead *section) {
    dStageMgr_c::GetInstance()->setLyse(OFS_TO_PTR(LYSE, section), section->mCount);
    dStageMgr_c::GetInstance()->overrideLayer0WithLyse();
}

static void handleBzsStif(int roomid, const BzsSectionHead *section) {
    // section->mCount == 1
    dStageMgr_c::GetInstance()->setStif(OFS_TO_PTR(STIF, section));
    s16 flagIndex = dStageMgr_c::GetInstance()->getFlagIndex();
    FileManager::sInstance->setSceneFlagIndex(flagIndex);
    updateFlagForFlagIndex(flagIndex);
}

static void handleBzsRmpl(int roomid, const BzsSectionHead *section) {
    dStageMgr_c::GetInstance()->setRmpl(OFS_TO_PTR(RMPL, section), section->mCount);
}

static void handleBzsFile(int roomid, const BzsSectionHead *section) {
    const bzsFILE *file = OFS_TO_PTR(bzsFILE, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setFile(file, section->mCount);
}

static void handleBzsLylt(int roomid, const BzsSectionHead *section) {
    const LYLT *lylt = OFS_TO_PTR(LYLT, section);
    for (s32 i = 0; i < section->mCount; i++) {
        if (lylt->layer == dScGame_c::currentSpawnInfo.layer && lylt->demoHigh >= 0) {
            SizedString<16> demoName;
            demoName.sprintf("Demo%02d_%02d", lylt->demoHigh, lylt->demoLow);
            dStageMgr_c::GetInstance()->setDemoName(demoName);
            dStageMgr_c::GetInstance()->addLayerArcn(dStageMgr_c::GetInstance()->getDemoName());
        }
        lylt++;
    }
}

static void handleBzsScen(int roomid, const BzsSectionHead *section) {
    const SCEN *scen = OFS_TO_PTR(SCEN, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setScen(scen, section->mCount);
}

static void handleBzsCam(int roomid, const BzsSectionHead *section) {
    const CAM *cam = OFS_TO_PTR(CAM, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setCam(cam, section->mCount);
}

static void handleBzsEvnt(int roomid, const BzsSectionHead *section) {
    const EVNT *evnt = OFS_TO_PTR(EVNT, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setEvnt(evnt, section->mCount);
}

static void handleBzsPath(int roomid, const BzsSectionHead *section) {
    const PATH *path = OFS_TO_PTR(PATH, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setPath(path, section->mCount);
}

static void handleBzsPnt(int roomid, const BzsSectionHead *section) {
    const PNT *pnt = OFS_TO_PTR(PNT, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setPnt(pnt, section->mCount);
}

static void handleBzsBpnt(int roomid, const BzsSectionHead *section) {
    const BPNT *bpnt = OFS_TO_PTR(BPNT, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setBpnt(bpnt, section->mCount);
}

static void handleBzsSpth(int roomid, const BzsSectionHead *section) {
    const SPTH *spth = OFS_TO_PTR(SPTH, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setSpth(spth, section->mCount);
}

static void handleBzsSpnt(int roomid, const BzsSectionHead *section) {
    const SPNT *spnt = OFS_TO_PTR(SPNT, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setSpnt(spnt, section->mCount);
}

static void handleBzsSbpt(int roomid, const BzsSectionHead *section) {
    const SBPT *sbpt = OFS_TO_PTR(SBPT, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setSbpt(sbpt, section->mCount);
}

static void handleBzsArea(int roomid, const BzsSectionHead *section) {
    const AREA *area = OFS_TO_PTR(AREA, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setArea(area, section->mCount);
}

extern "C" bool fn_8005C5D0(u32);
extern "C" void spawnLightObject();
extern "C" void spawnWeatherTag();

static void handleBzsPly(int roomid, const BzsSectionHead *section) {
    const PLY *ply = OFS_TO_PTR(PLY, section);
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomid);
    room->setPly(ply, section->mCount);

    if (roomid == dScGame_c::currentSpawnInfo.room) {
        dScGame_c *sc = dScGame_c::GetInstance();
        const mVec3_c *linkPos;
        const mAng3_c *linkAngle;
        mAng3_c ang(0, 0, 0);
        mVec3_c pos;
        u32 linkActorParams = 0x7FFFFFF;
        bool param = 0;
        bool preventSetRespawnInfo = false;
        u8 reloaderType = dScGame_c::getReloaderType();
        if (reloaderType != 0) {
            FileManager *fileMgr = FileManager::sInstance;
            if (reloaderType == 2) {
                pos = *FileManager::sInstance->getPosT2();
                ang.y = fileMgr->getAngleT2();
                linkPos = &pos;
                linkAngle = &ang;
                linkActorParams = 0x7FFFF1B;
            } else if (reloaderType == 3) {
                pos = *FileManager::sInstance->getPosT3();
                ang.y = fileMgr->getAngleT3();
                linkPos = &pos;
                linkAngle = &ang;
                if (!StoryflagManager::sInstance->getCounterOrFlag(752) &&
                    !StoryflagManager::sInstance->getCounterOrFlag(119)) {
                    const PLY *entrance = room->getEntranceById(dScGame_c::currentSpawnInfo.entrance);
                    if (fn_8005C5D0(entrance->linkParams) == true) {
                        linkActorParams = entrance->linkParams;
                    }
                }
            } else {
                if (FileManager::sInstance->getEntranceLoadFlagT1() == 0 && dScGame_c::getReloaderType() != 4) {
                    pos = *fileMgr->getPosT1();
                    ang.y = fileMgr->getAngleT1();
                    if (fileMgr->fn_8000CC00() == 2) {
                        const PLY *entrance = room->getEntranceById(dScGame_c::currentSpawnInfo.entrance);
                        linkActorParams = entrance->linkParams;
                    }
                    linkPos = &pos;
                    linkAngle = &ang;
                } else {
                    const PLY *entrance = room->getEntranceById(dScGame_c::currentSpawnInfo.entrance);
                    linkActorParams = entrance->linkParams;
                    linkPos = &entrance->position;
                    linkAngle = &entrance->angle;
                }
            }
            dScGame_c::setReloaderType(0);
            if (sc->getType0CsFlag() != 0) {
                sc->setType0CsFlag(0);
            }
        } else {
            if (dScGame_c::GetInstance()->getType0CsFlag() != 0) {
                preventSetRespawnInfo = true;
                const LinkReloadInfo &info = dScGame_c::GetInstance()->getLinkReloadInfo();
                ang.y = info.mRotY;
                linkPos = &info.mPosition;
                linkAngle = &ang;
                linkActorParams = info.mLinkParams;
                sc->setType0CsFlag(0);
            } else if (dScGame_c::GetInstance()->getType0PosFlag() != 0) {
                const SpawnInfoExt &info = dScGame_c::GetInstance()->getSpawnInfo();
                pos = info.mType0Pos;
                linkPos = &pos;
                ang.y = info.mType0RotY;
                linkAngle = &ang;
                sc->setType0PosFlag(0);
            } else {
                const PLY *entrance = room->getEntranceById(dScGame_c::currentSpawnInfo.entrance);
                linkActorParams = entrance->linkParams;
                linkPos = &entrance->position;
                linkAngle = &entrance->angle;
                param = true;
            }
        }

        // TODO constify args
        dAcObjBase_c::create(
            fProfile::PLAYER, -1, linkActorParams, (mVec3_c *)linkPos, (mAng3_c *)linkAngle, nullptr, -1
        );
        if (!preventSetRespawnInfo) {
            dScGame_c::GetInstance()->setRespawnInfo(*linkPos, *linkAngle, param);
        }
        dStage_c::GetInstance()->handleRoomChange(roomid);
        dBase_c::createBase(fProfile::CAMERA, dStage_c::GetInstance(), 0, fBase_c::OTHER);
        spawnLightObject();
        spawnWeatherTag();
    }
}

extern "C" u16 getActorIdForObjName2(const char *objname);

// The above function suffered from tons of regswaps that ended up being fixed very quickly
// (swapping *pos and *ang). The below four functions have a single regswap that I don't
// understand and that I spent more time trying to fix than in the above function.

static void handleBzsObjn(int roomid, const BzsSectionHead *section) {
    const OBJN *objn = OFS_TO_PTR(OBJN, section);
    const char *buf = reinterpret_cast<const char *>(objn);
    for (s32 i = 0; i < section->mCount; i++) {
        dStageMgr_c::GetInstance()->addObjId(getActorIdForObjName2(buf + objn->offset));
        objn++;
    }
}

static void handleBzsArcn(int roomid, const BzsSectionHead *section) {
    const ARCN *arcn = OFS_TO_PTR(ARCN, section);
    const char *buf = reinterpret_cast<const char *>(arcn);
    for (s32 i = 0; i < section->mCount; i++) {
        dStageMgr_c::GetInstance()->addStageArcn(buf + arcn->offset);
        arcn++;
    }
}

static void handleBzsLayerObjn(int roomid, const BzsSectionHead *section) {
    const OBJN *objn = OFS_TO_PTR(OBJN, section);
    const char *buf = reinterpret_cast<const char *>(objn);
    for (s32 i = 0; i < section->mCount; i++) {
        u16 id = getActorIdForObjName2(buf + objn->offset);
        dStageMgr_c::GetInstance()->addActorId(id);
        objn++;
    }
}

static void handleBzsLayerArcn(int roomid, const BzsSectionHead *section) {
    const ARCN *arcn = OFS_TO_PTR(ARCN, section);
    const char *buf = reinterpret_cast<const char *>(arcn);
    for (s32 i = 0; i < section->mCount; i++) {
        dStageMgr_c::GetInstance()->addLayerArcn(buf + arcn->offset);
        arcn++;
    }
}

static u16 getEnemyDefeatFlagFromId(s32 roomid, u16 id) {
    u16 val = id & 0x3FF;
    if (val >= 0x400) {
        // Interesting condition
        return -1;
    }
    if (roomid == -1) {
        return val + (0x3F << 10);
    }
    return val + (roomid << 10);
}

static s8 getViewClipIndexFromId(u16 id) {
    s8 val = (id >> 10) & 0x3F;
    if (val == 0x3F) {
        return -1;
    }
    return val;
}

static void handleBzsObj(int roomid, const BzsSectionHead *section) {
    dStage_c *stage = dStage_c::GetInstance();
    const OBJ *obj = OFS_TO_PTR(OBJ, section);
    for (s32 i = 0; i < section->mCount; i++) {
        u16 flag = getEnemyDefeatFlagFromId(roomid, obj->id);
        s8 viewClipIdx = getViewClipIndexFromId(obj->id);
        if (stage->checkEnemyDefeatFlag(flag)) {
            dAcObjBase_c::create(
                obj->name, roomid, obj->params1, &obj->position, &obj->angle, nullptr, obj->params2, flag, viewClipIdx
            );
        }
        obj++;
    }
}

static void handleBzsSobj(int roomid, const BzsSectionHead *section) {
    dStage_c *stage = dStage_c::GetInstance();
    const SOBJ *obj = OFS_TO_PTR(SOBJ, section);
    for (s32 i = 0; i < section->mCount; i++) {
        u16 flag = getEnemyDefeatFlagFromId(roomid, obj->id);
        s8 viewClipIdx = getViewClipIndexFromId(obj->id);
        if (stage->checkEnemyDefeatFlag(flag)) {
            dAcObjBase_c::create(
                obj->name, roomid, obj->params1, &obj->position, &obj->angle, (mVec3_c *)&obj->scale, obj->params2,
                flag, viewClipIdx
            );
        }
        obj++;
    }
}

static void handleBzsStas(int roomid, const BzsSectionHead *section) {
    dStage_c *stage = dStage_c::GetInstance();
    const SOBJ *obj = OFS_TO_PTR(SOBJ, section);
    for (s32 i = 0; i < section->mCount; i++) {
        u16 flag = getEnemyDefeatFlagFromId(roomid, obj->id);
        s8 viewClipIdx = getViewClipIndexFromId(obj->id);
        if (stage->checkEnemyDefeatFlag(flag)) {
            dAcObjBase_c::createActorUnkGroup3(
                obj->name, roomid, obj->params1, &obj->position, &obj->angle, &obj->scale, obj->params2, flag,
                viewClipIdx
            );
        }
        obj++;
    }
}

void parseLayerBzs(int roomid, u32 layer, const LAY *head, const BzsTagToParseFunc *handlerTable, s32 tableSize) {
    const LAY *lay = head + layer;
    const BzsSectionHead *newHead = OFS_TO_PTR(BzsSectionHead, lay);
    for (s32 i = 0; i < lay->mCount; i++) {
        findAndRunStageTagParseFunc(roomid, newHead++, handlerTable, tableSize);
    }
}

static const BzsTagToParseFunc layerHandlers[] = {
    {'OBJN', handleBzsLayerObjn},
    {'ARCN', handleBzsLayerArcn},
};

void handleBzsLay(int roomid, const BzsSectionHead *section) {
    const LAY *lay = OFS_TO_PTR(LAY, section);
    parseLayerBzs(roomid, 0, lay, layerHandlers, ARRAY_LENGTH(layerHandlers));
    if (dScGame_c::currentSpawnInfo.layer != 0) {
        parseLayerBzs(roomid, dScGame_c::currentSpawnInfo.layer, lay, layerHandlers, ARRAY_LENGTH(layerHandlers));
    }
}

static const BzsTagToParseFunc layerHandlers2[] = {
    {'OBJ ',  handleBzsObj},
    {'SOBJ', handleBzsSobj},
    {'STAG', handleBzsStas},
};

void handleBzsLay2(int roomid, const BzsSectionHead *section) {
    const LAY *lay = OFS_TO_PTR(LAY, section);
    parseLayerBzs(roomid, 0, lay, layerHandlers2, ARRAY_LENGTH(layerHandlers2));
    if (dScGame_c::currentSpawnInfo.layer != 0) {
        parseLayerBzs(roomid, dScGame_c::currentSpawnInfo.layer, lay, layerHandlers2, ARRAY_LENGTH(layerHandlers2));
    }
}

static const BzsTagToParseFunc layerHandlers3[] = {
    {'OBJS',  handleBzsObj},
    {'SOBS', handleBzsSobj},
    {'STAS', handleBzsStas},
    {'SNDT', handleBzsStas},
    {'TBOX',  handleBzsObj},
    {'DOOR',  handleBzsObj},
};

void handleBzsLay3(int roomid, const BzsSectionHead *section) {
    const LAY *lay = OFS_TO_PTR(LAY, section);
    parseLayerBzs(roomid, 0, lay, layerHandlers3, ARRAY_LENGTH(layerHandlers3));
    if (dScGame_c::currentSpawnInfo.layer != 0) {
        parseLayerBzs(roomid, dScGame_c::currentSpawnInfo.layer, lay, layerHandlers3, ARRAY_LENGTH(layerHandlers3));
    }
}

static const BzsTagToParseFunc defaultHandlers[] = {
    {'PCAM', handleBzsPcam},
    {'LYSE', handleBzsLyse},
    {'STIF', handleBzsStif},
    {'RMPL', handleBzsRmpl},
    {'SCEN', handleBzsScen},
    {'CAM ',  handleBzsCam},
    {'EVNT', handleBzsEvnt},
    {'PATH', handleBzsPath},
    {'PNT ',  handleBzsPnt},
    {'BPNT', handleBzsBpnt},
    {'SPTH', handleBzsSpth},
    {'SPNT', handleBzsSpnt},
    {'SBPT', handleBzsSbpt},
    {'AREA', handleBzsArea},
};

static void findAndRunStageTagParseFunc(
    int roomid, const BzsSectionHead *head, const BzsTagToParseFunc *handlerTable, s32 tableSize
) {
    if (handlerTable == nullptr) {
        tableSize = ARRAY_LENGTH(defaultHandlers);
        handlerTable = defaultHandlers;
    }
    while (tableSize > 0) {
        if (head->mTag == handlerTable->mTag) {
            handlerTable->mParseFunc(roomid, head);
            return;
        }
        handlerTable++;
        tableSize--;
    }
}

static const BzsTagToParseFunc roomStageHandlers[] = {
    {'OBJN', handleBzsObjn},
    {'ARCN', handleBzsArcn},
};

void parseRoomStageBzs(int roomid, const void *bzs) {
    const BzsHeader *header = static_cast<const BzsHeader *>(bzs);
    const BzsSectionHead *head = OFS_TO_PTR(BzsSectionHead, header);
    for (s32 i = 0; i < header->mSectionCount; i++) {
        findAndRunStageTagParseFunc(roomid, head++, roomStageHandlers, ARRAY_LENGTH(roomStageHandlers));
    }
}

static const BzsTagToParseFunc stageRoomHandlers[] = {
    {'LYLT', handleBzsLylt},
    {'LAY ',  handleBzsLay},
};

void parseBzsStageRoom(int roomid, const void *bzs) {
    const BzsHeader *header = static_cast<const BzsHeader *>(bzs);
    const BzsSectionHead *head = OFS_TO_PTR(BzsSectionHead, header);
    for (s32 i = 0; i < header->mSectionCount; i++) {
        findAndRunStageTagParseFunc(roomid, head++, stageRoomHandlers, ARRAY_LENGTH(stageRoomHandlers));
    }
}

void parseStageBzs(int roomid, const void *bzs) {
    const BzsHeader *header = static_cast<const BzsHeader *>(bzs);
    const BzsSectionHead *head = OFS_TO_PTR(BzsSectionHead, header);
    for (s32 i = 0; i < header->mSectionCount; i++) {
        // Run "default handlers"
        findAndRunStageTagParseFunc(roomid, head++, nullptr, 0);
    }
}

static const BzsTagToParseFunc roomHandlers[] = {
    {'FILE', handleBzsFile},
    {'SCEN', handleBzsScen},
    {'CAM ',  handleBzsCam},
    {'EVNT', handleBzsEvnt},
    {'PATH', handleBzsPath},
    {'PNT ',  handleBzsPnt},
    {'BPNT', handleBzsBpnt},
    {'SPTH', handleBzsSpth},
    {'SPNT', handleBzsSpnt},
    {'SBPT', handleBzsSbpt},
    {'AREA', handleBzsArea},
    {'PLY ',  handleBzsPly},
};

void parseRoomBzs(int roomid, const void *bzs) {
    const BzsHeader *header = static_cast<const BzsHeader *>(bzs);
    const BzsSectionHead *head = OFS_TO_PTR(BzsSectionHead, header);
    for (s32 i = 0; i < header->mSectionCount; i++) {
        findAndRunStageTagParseFunc(roomid, head++, roomHandlers, ARRAY_LENGTH(roomHandlers));
    }
}

static const BzsTagToParseFunc roomReactivateHandlers[] = {
    {'SOBJ', handleBzsSobj},
    {'STAG', handleBzsStas},
    {'LAY ', handleBzsLay2},
};

void parseRoomReactivateBzs(int roomid, const void *bzs) {
    const BzsHeader *header = static_cast<const BzsHeader *>(bzs);
    const BzsSectionHead *head = OFS_TO_PTR(BzsSectionHead, header);
    for (s32 i = 0; i < header->mSectionCount; i++) {
        findAndRunStageTagParseFunc(roomid, head++, roomReactivateHandlers, ARRAY_LENGTH(roomReactivateHandlers));
    }
}

static const BzsTagToParseFunc roomActivateHandlers[] = {
    {'LAY ', handleBzsLay3},
};

void parseRoomActivateBzs(int roomid, const void *bzs) {
    const BzsHeader *header = static_cast<const BzsHeader *>(bzs);
    const BzsSectionHead *head = OFS_TO_PTR(BzsSectionHead, header);
    for (s32 i = 0; i < header->mSectionCount; i++) {
        findAndRunStageTagParseFunc(roomid, head++, roomActivateHandlers, ARRAY_LENGTH(roomActivateHandlers));
    }
}
