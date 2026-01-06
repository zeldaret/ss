#ifndef TO_BE_SORTED_D_CAMERA_GAME_H
#define TO_BE_SORTED_D_CAMERA_GAME_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "m/m_vec.h"
#include "toBeSorted/d_camera_base.h"
#include "toBeSorted/deg_angle_util.h"

// Size: 0xC
struct CamParamHeader {
    /* 0x00 */ u32 magic;
    /* 0x04 */ s16 numFloats;
    /* 0x06 */ s16 numFlags;
    /* 0x08 */ s16 numStyles;
    /* 0x0A */ s16 numTrends;
};

// Size: 0xA
#pragma pack(push, 2)
struct CamParamStyle {
    /* 0x00 */ u32 id;
    /* 0x04 */ u8 _0x04[6];
};
#pragma pack(pop)

// Size: 0x24
struct CamParamTrend {
    /* 0x00 */ char name[16]; // not sure
    /* 0x10 */ u8 _0x10[0x24 - 0x10];
};

enum CamTrendIndex_e {
    CAM_TREND_IDX_BalanceBall,
    CAM_TREND_IDX_Swim,
    CAM_TREND_IDX_Surface,
    CAM_TREND_IDX_SkyDive,
    CAM_TREND_IDX_Dive,
    CAM_TREND_IDX_Lift,
    CAM_TREND_IDX_Crawl,
    CAM_TREND_IDX_Field,
    CAM_TREND_IDX_Dungeon,
    CAM_TREND_IDX_mogu,
    CAM_TREND_IDX_Tornade,
    CAM_TREND_IDX_Rope,
    CAM_TREND_IDX_Tightrope,
    CAM_TREND_IDX_Parachute,
    CAM_TREND_IDX_DialKey,
    CAM_TREND_IDX_BirdRide,
    CAM_TREND_IDX_Trolley,
    CAM_TREND_IDX_Boomerang,
    CAM_TREND_IDX_Room,
    CAM_TREND_IDX_AfterCrawl,
    CAM_TREND_IDX_TerryShip_Island,
    CAM_TREND_IDX_Catapult,
    CAM_TREND_IDX_TightropeHang,
    CAM_TREND_IDX_BossKey,
    CAM_TREND_IDX_SkyDiveNear,
    CAM_TREND_IDX_mogu2,
    CAM_TREND_IDX_ClawShot,
    CAM_TREND_IDX_Town,
    CAM_TREND_IDX_Cannon,
    CAM_TREND_IDX_BirdRideLD,
    CAM_TREND_IDX_Nushi,
    CAM_TREND_IDX_Island,
    CAM_TREND_IDX_Windmill,

    CAM_TREND_IDX_MAX,
};

enum CamStyleIndex_e {
    CAM_STYLE_IDX_TA22,

    CAM_STYLE_IDX_MAX,
};

class dCameraGame_c;

struct CamId {
    CamId() : roomCamId(-1), roomId(-1), field_0x04(0) {}
    s16 roomCamId;
    s16 roomId;
    s16 field_0x04;

    void clear() {
        roomCamId = -1;
        roomId = dAcPy_c::GetLinkM()->mRoomID;
        field_0x04 = 0;
    }

    bool someCheck() const {
        if (field_0x04 < 3 && roomId != dAcPy_c::GetLinkM()->mRoomID) {
            return false;
        } else {
            return field_0x04 >= -1 && roomCamId >= 0 && roomCamId < 0xFF;
        }
    }
};

struct UnkCamCheck {
    /* 0x000 */ u8 field_0x000;
    /* 0x004 */ dBgS_CamGndChk chk;
    /* 0x094 */ f32 field_0x094;
    /* 0x098 */ CamId camId;
};

struct UnkCamRef {
    /* 0x00 */ bool field_0x00;
    /* 0x04 */ mVec3_c field_0x04;
    /* 0x10 */ dAcRef_c<dAcObjBase_c> mMoveBgActorRef;
};

struct UnkCamChecks {
    /* 0x000 */ UnkCamCheck mChk1;
    /* 0x0A0 */ UnkCamCheck mChk2;
    /* 0x140 */ UnkCamRef mRef;
    /* 0x15C */ UNKWORD field_0x15C;
    /* 0x160 */ UNKWORD field_0x160;
    /* 0x164 */ CamId field_0x164;
    /* 0x16A */ u8 field_0x16A;
    /* 0x16B */ u8 field_0x16B;
    /* 0x16C */ CamId field_0x16C;
    /* 0x174 */ dCameraGame_c *mpCamera;

    void fn_8007E130(dCameraGame_c *system);
    void fn_8007E1B0(dAcObjBase_c *ac);
};

class dCameraGame_c : public dCameraBase_c {
public:
    static bool initCamParamDat();
    static s32 getTrendIndex(const char *name);
    static s32 getStyleIndex(u32 id);

    static s32 sTrendIdxes[CAM_TREND_IDX_MAX];
    static s32 sStyleIdxes[CAM_STYLE_IDX_MAX];

    dCameraGame_c();
    /* vt 0x08 */ virtual void vt_0x08() override;
    /* vt 0x0C */ virtual void vt_0x0C() override;
    /* vt 0x10 */ virtual bool create() override;
    /* vt 0x14 */ virtual bool remove() override;
    /* vt 0x18 */ virtual bool execute() override;
    /* vt 0x1C */ virtual bool draw() override;
    /* vt 0x20 */ virtual ~dCameraGame_c() {}
    /* vt 0x30 */ virtual void setView(const CamView &view) override;

    CamId getCamId0x60() {
        return field_0x060;
    }

    void clearCamIds();
    
    bool isCurrentTrend(const char *name) const;

    void fn_80080960(s32, s32, s8, s32);

    void onFlag(u32 flag) {
        mFlags |= flag;
    }

    void offFlag(u32 flag) {
        mFlags &= ~flag;
    }

private:
    static void cacheIdxes();

    static CamParamHeader *sHeader;
    static f32 *sFloats;
    static u16 *sFlags;
    static CamParamStyle *sStyles;
    static CamParamTrend *sTrends;

    /* 0x02C */ dPolar field_0x02C;
    /* 0x038 */ CamView mView2;
    /* 0x058 */ u32 mFlags;
    /* 0x05C */ UNKWORD field_0x05C;
    /* 0x060 */ CamId field_0x060;
    /* 0x068 */ s32 mCurrentTrendIdx;
    /* 0x06C */ UNKWORD field_0x06C;
    /* 0x070 */ UNKWORD field_0x070;
    /* 0x074 */ UNKWORD field_0x074;
    /* 0x078 */ u8 field_0x078;
    /* 0x079 */ u8 field_0x079;
    /* 0x07A */ u8 field_0x07A;
    /* 0x07C */ UNKWORD field_0x07C;
    /* 0x080 */ CamId field_0x080;
    /* 0x086 */ CamId field_0x086;
    /* 0x08C */ UNKWORD field_0x08C;
    /* 0x090 */ UNKWORD field_0x090;
    /* 0x094 */ s16 field_0x094;
    /* 0x096 */ s16 field_0x096;
    /* 0x098 */ dAcObjBase_c *field_0x098;
    /* 0x09C */ dAcRef_c<dAcObjBase_c> mRef;
    /* 0x0A8 */ UNKWORD field_0x0A8;
    /* 0x0AC */ f32 field_0x0AC;
    /* 0x0B0 */ f32 field_0x0B0;
    /* 0x0B4 */ UnkCamChecks mChk;
    /* 0x22C */ UNKWORD field_0x22C;
    /* 0x230 */ UNKWORD field_0x230;
    /* 0x234 */ mVec3_c field_0x234;
    /* 0x240 */ mVec3_c field_0x240;
    /* 0x24C */ u8 _0x24C[0x264 - 0x24C];
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ u8 _0x0[0x474 - 0x26C];
};

#endif
