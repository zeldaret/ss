#ifndef TOBESORTED_ITEM_MDL_MISC_H
#define TOBESORTED_ITEM_MDL_MISC_H

#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "m/types_m.h"
#include "toBeSorted/item_mdl.h"

class dItemMdlPut_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlPut_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) override;
    /* 0x10 */ virtual void vt_0x10(u8) override;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) override;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) override;
    /* 0x1C */ virtual void draw() override;
    /* 0x20 */ virtual void vt_0x20(u16 itemId) override;
    /* 0x24 */ virtual void setPriorityDraw() override;
    /* 0x28 */ virtual void unsetPriorityDraw() override;

    static bool isMdlForItemId(u16 itemId);

private:
    struct MdlConfig {
        const char *putResFile;
        const char *putResMdl;
        const char *getResFile;
        const char *getResMdl;
    };
    static const MdlConfig sMdlConfig[];
    static const u8 sMdlIdx[];

    /* 0x18 */ m3d::smdl_c mGetMdl;
    /* 0x34 */ m3d::smdl_c mPutMdl;
};

class dItemMdlShield_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlShield_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) override;
    /* 0x10 */ virtual void vt_0x10(u8) override;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) override;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) override;
    /* 0x1C */ virtual void draw() override;
    /* 0x20 */ virtual void vt_0x20(u16 itemId) override;
    /* 0x24 */ virtual void setPriorityDraw() override;
    /* 0x28 */ virtual void unsetPriorityDraw() override;

    static bool isMdlForItemId(u16 itemId);

private:
    struct MdlConfig {
        f32 frame;
        const char *resFile;
        const char *mdlName;
        const char *anmName;
    };
    static const MdlConfig sMdlConfig[];
    static const u8 sMdlIdx[];

    /* 0x18 */ m3d::smdl_c mMdl;
    /* 0x34 */ m3d::anmTexPat_c mAnmTexPat;
};

// Maybe a bad name because fairies etc. are also supported
class dItemMdlBottle_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlBottle_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) override;
    /* 0x10 */ virtual void vt_0x10(u8) override;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) override;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) override;
    /* 0x1C */ virtual void draw() override;
    /* 0x20 */ virtual void vt_0x20(u16 itemId) override;
    /* 0x24 */ virtual void setPriorityDraw() override;
    /* 0x28 */ virtual void unsetPriorityDraw() override;
    /* 0x2C */ virtual m3d::smdl_c *getMdl() override {
        return &mMdl.getModel();
    }

    static bool isMdlForItemId(u16 itemId);

private:
    struct MdlConfig {
        const char *resFile;
        const char *mdlName;
        const char *anmName;
    };
    static const MdlConfig sMdlConfig[];
    static const u8 sMdlIdx[];

    /* 0x18 */ m3d::mdlAnmChr mMdl;
    /* 0x80 */ u32 mFlags;
};

class dItemMdlTear_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlTear_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) override;
    /* 0x10 */ virtual void vt_0x10(u8) override;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) override;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) override;
    /* 0x1C */ virtual void draw() override;
    /* 0x20 */ virtual void vt_0x20(u16 itemId) override;
    /* 0x24 */ virtual void setPriorityDraw() override;
    /* 0x28 */ virtual void unsetPriorityDraw() override;

    static bool isMdlForItemId(u16 itemId);

private:
    struct MdlConfig {
        f32 frame;
    };
    static const MdlConfig sMdlConfig[];
    static const u8 sMdlIdx[];

    /* 0x18 */ m3d::smdl_c mMdl1;
    /* 0x34 */ m3d::smdl_c mMdl2;
    /* 0x50 */ m3d::anmTexPat_c mAnmTexPat1;
    /* 0x7C */ m3d::anmTexPat_c mAnmTexPat2;
    /* 0xA8 */ m3d::anmTexSrt_c mAnmTexSrt1;
    /* 0xD4 */ m3d::anmTexSrt_c mAnmTexSrt2;
};

class dItemMdlFirefly_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlFirefly_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) override;
    /* 0x10 */ virtual void vt_0x10(u8) override;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) override;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) override;
    /* 0x1C */ virtual void draw() override;
    /* 0x20 */ virtual void vt_0x20(u16 itemId) override;
    /* 0x24 */ virtual void setPriorityDraw() override;
    /* 0x28 */ virtual void unsetPriorityDraw() override;
    /* 0x2C */ virtual m3d::smdl_c *getMdl() override;

    static bool isMdlForItemId(u16 itemId);

private:
    /* 0x18 */ m3d::smdl_c mMdl;
    /* 0x34 */ m3d::anmMatClr_c mAnmMatClr;
};

// Maybe a bad name because crystals are also supported
class dItemMdlPotion_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlPotion_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) override;
    /* 0x10 */ virtual void vt_0x10(u8) override;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) override;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) override;
    /* 0x1C */ virtual void draw() override;
    /* 0x20 */ virtual void vt_0x20(u16 itemId) override;
    /* 0x24 */ virtual void setPriorityDraw() override;
    /* 0x28 */ virtual void unsetPriorityDraw() override;
    /* 0x2C */ virtual m3d::smdl_c *getMdl() override;

    static bool isMdlForItemId(u16 itemId);

private:
    struct MdlConfig {
        const char *resFile;
        const char *mdlName;
        const char *anmChrName;
        const char *anmTexSrtName;
    };
    static const MdlConfig sMdlConfig[];
    static const u8 sMdlIdx[];

    /* 0x18 */ m3d::smdl_c mMdl;
    /* 0x34 */ m3d::anmChr_c mAnmChr;
    /* 0x6C */ m3d::anmTexSrt_c mAnmTexSrt;
};

#endif
