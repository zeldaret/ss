#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

class dAcOInsect_c : public dAcObjBase_c {
public:
    dAcOInsect_c() {}
    virtual ~dAcOInsect_c() {}

    /* 0x80 */ virtual f32 dAcOInsect_0x80();
    /* 0x84 */ virtual f32 dAcOInsect_0x84() {
        return 0.0f;
    }
    /* 0x88 */ virtual f32 dAcOInsect_0x88() {
        return 0.0f;
    }
    /* 0x8C */ virtual f32 dAcOInsect_0x8C() {
        return 0.0f;
    }
    /* 0x90 */ virtual f32 getAttentionField() {
        return 0.0f;
    }
    /* 0x94 */ virtual f32 dAcOInsect_0x94() {
        return 0.0f;
    }
    /* 0x98 */ virtual bool shouldAddAttentionTarget() {
        return isLinkUsingBugNet();
    }
    /* 0x9C */ virtual void addAttentionTarget();
    /* 0xA0 */ virtual void preAttention();
    /* 0xA4 */ virtual s32 dAcOInsect_0xA4() {
        return 0;
    }
    /* 0xA8 */ virtual bool dAcOInsect_0xA8() {
        return checkForLinkScare();
    }
    /* 0xAC */ virtual void dAcOInsect_0xAC() {}
    /* 0xB0 */ virtual void dAcOInsect_0xB0();

protected:
    void goPoof();
    bool isLinkNearby(f32 radius);
    bool isLinkNearbyZ(f32 distance);
    void someBaseFunction();
    bool someBaseFunction0();
    bool checkForLinkScare();
    bool isLinkCloseAndFlag();
    s32 getLinkSpeedLevel();
    static bool isLinkUsingBugNet();
    bool resetScale();
    bool fn_8018FAA0();
    bool fn_80190180(mAng &outAng);

private:
    /* 0x330 */ s32 field_0x330;
    /* 0x334 */ u8 field_0x334;
    /* 0x335 */ u8 field_0x335;
    /* 0x338 */ u32 field_0x338;
    /* 0x33C */ mVec3_c field_0x33C;
    /* 0x348 */ u8 field_0x348[0x35C - 0x348];
    /* 0x35C */ f32 field_0x35C;
    /* 0x360 */ u8 field_0x360[0x3BE - 0x360];
    /* 0x3BE */ u8 field_0x3BE;
    /* 0x3BF */ u8 field_0x3BF;
    /* 0x3C0 */ mMtx_c field_0x3C0;
    /* 0x3F0 */ u8 field_0x3F0[0x3F8 - 0x3F0];
    /* 0x3F8 */ u32 mLinkNearby;
    /* 0x3FC */ dAcRef_c<dAcBase_c> field_0x3FC;
    /* 0x408 */ u8 field_0x408[0x40D - 0x408];
    /* 0x40D */ u8 field_0x40D;
    /* 0x410 */ mVec3_c field_0x410[3];
    /* 0x434 */ mVec3_c field_0x434[3];
    /* 0x458 */ u8 field_0x458[0x45C - 0x458];
    /* 0x45C */ mVec3_c field_0x45C;
};
