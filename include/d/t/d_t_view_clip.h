#ifndef D_T_VIEW_CLIP_H
#define D_T_VIEW_CLIP_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"

class dTgViewClip_c : public dTg_c {
public:
    static const s32 sNumEntries = 0x40;

    enum Flag_e {
        FLAG_NORMAL = 0,
        FLAG_FORCE_ON = (1 << 0),
        FLAG_FORCE_OFF = (1 << 1),
    };

    enum Type_e {
        TYPE_FORCE_ON,
        TYPE_FORCE_OFF,
    };

    dTgViewClip_c() {}
    virtual ~dTgViewClip_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    void setViewClip(u8 setBit);
    static void resetViewClip();
    static f32 getCull(s32 idx, f32 cullDistance);
    static bool checkForceOn(s32 idx);

    static u8 getViewClipBit(s32 idx, u8 setBit);
    static void setViewClipBit(s32 idx, u8 setBit);

    static void setViewClip(s32 idx, u8 value);

private:
    /* 0x0DC */ mMtx_c mArea;
    /* 0x12C */ u8 mIndex;
    /* 0x12D */ u8 mType;
    /* 0x12E */ u8 mAreaIndex;

    static u8 sViewClips[sNumEntries];
};

#endif
