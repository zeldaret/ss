#ifndef TOBESORTED_ITEM_MDL_ITEM_H
#define TOBESORTED_ITEM_MDL_ITEM_H

#include "m/m3d/m_smdl.h"
#include "toBeSorted/item_mdl.h"


class dItemMdlItem_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlItem_c() {}
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
    };
    static const MdlConfig sMdlConfig[];
    static const u8 sMdlIdx[];

    /* 0x18 */ m3d::smdl_c mMdl;
};

#endif
