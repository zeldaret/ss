#ifndef D_T_SCENE_CHANGE_H
#define D_T_SCENE_CHANGE_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

class dTgSceneChange_c : public dTg_c {
public:
    // TODO: Move this somewhere not in d_t_scene_change.
    //       The Variable gets passed to dAcPy_c::triggerExitRelated
    enum Type_e {
        /* 0x0 */ TYPE_0,
        /* 0x1 */ TYPE_1, // Restricting Movement?
        /* 0x2 */ TYPE_2,
        /* 0x3 */ TYPE_3, // Forced Walk
        /* 0x4 */ TYPE_4, // Loftwing
        /* 0x5 */ TYPE_5, // Landing on Skyloft
        /* 0x6 */ TYPE_6, // Enter/Exit Dungeon
        /* 0x7 */ TYPE_7, // Minecart
        /* 0x8 */ TYPE_8, // Cistern Rope to Basement
        /* 0x9 */ TYPE_9, // Light Pillar?
        /* 0xA */ TYPE_A, // Boat
        /* 0xB */ TYPE_B, // Actor Related
    };

    dTgSceneChange_c() {}
    virtual ~dTgSceneChange_c() {}

    virtual int create() override;
    virtual int draw() override;
    virtual int actorExecute() override;

private:
    bool checkDisabled() const;
    bool checkScrapper();

    void savePlayerPosition();

private:
    /* 0x0FC */ mMtx_c mArea;
    /* 0x12C */ u8 mSceneLink;
    /* 0x12D */ u8 mSceneType;
    /* 0x12E */ u8 mPathId;
    /* 0x12F */ u8 mEnableSceneflag;
    /* 0x130 */ u16 mEnableStoryflag;
    /* 0x132 */ u16 mDisableStoryflag;
    /* 0x134 */ bool field_0x134;
    /* 0x135 */ bool field_0x135;
    /* 0x136 */ bool field_0x136;
    /* 0x138 */ mVec3_c field_0x138; // Player position on Scrapper Stop
};

#endif
