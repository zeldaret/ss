#ifndef D_A_OBJ_BLOCK_UNDERGROUND_H
#define D_A_OBJ_BLOCK_UNDERGROUND_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_underground.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "f/f_list_nd.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"

class dAcOBlockUnderground : public dAcObjBase_c {
public:
    dAcOBlockUnderground();
    virtual ~dAcOBlockUnderground();

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    void setUndergroundRef(dAcOUnderground_c *);
    bool undergroundRefEquals(dAcOUnderground_c *);
    void setBlockUndergroundPosition(mVec3_c *);
    void fn_459_840(u16, u16);
    u16 getField_0x57C();
    u16 getField_0x57E();
    void registBg();
    void releaseBg();

private:
    static u32 getCylTgType();
    static const char *getOarcName();
    void setSubtype(u32);
    u8 getSubtype();
    u8 getFirstSceneFlag();
    u8 getSecondSceneFlag();
    u32 getSubtypeCylVariant();
    u32 getMdlDzbPlcIdx();
    bool noCyl();
    bool shouldSetCylTgType();
    const char *getMdlName();
    bool hasCyl();
    void spawnEffect(u16) const;

    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ fLiNdBa_c mRef;
    /* 0x358 */ dAcRef_c<dAcOUnderground_c> mUndergroundRef;
    /* 0x364 */ dBgW mBgW;
    /* 0x574 */ dCcD_Cyl *mCyl;
    /* 0x578 */ u32 mSubtype;
    /* 0x57C */ u16 mField_0x57C;
    /* 0x57E */ u16 mField_0x57E;

    struct SubtypeData {
        s32 mSubtype;
        s32 mCylVariant;
        s32 mMdlDzbPlcIdx;
    };

    static const SubtypeData sSubtypeData[4];
    static const char *const sOarcName;
    static const char *const sMdlNames[4];
    static const char *const sDzbPlcNames[4];
    static const dCcD_SrcCyl sCylSrc;
};

#endif
