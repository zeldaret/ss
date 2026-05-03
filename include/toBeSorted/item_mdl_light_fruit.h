#ifndef TOBESORTED_ITEM_MDL_LIGHT_FRUIT_H
#define TOBESORTED_ITEM_MDL_LIGHT_FRUIT_H

#include "m/m3d/m_smdl.h"
#include "m/m3d/m_anmtexsrt.h"
#include "toBeSorted/item_mdl.h"


class dItemMdlLightFruit_c : public dItemMdl_c {
public:
    /* 0x08 */ virtual ~dItemMdlLightFruit_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) override;
    /* 0x10 */ virtual void setDrawMode(u8) override;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) override;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) override;
    /* 0x1C */ virtual void draw() override;
    /* 0x20 */ virtual void changeItemId(u16 itemId) override;
    /* 0x24 */ virtual void setPriorityDraw() override;
    /* 0x28 */ virtual void unsetPriorityDraw() override;

    static bool isMdlForItemId(u16 itemId);

private:
    /* 0x18 */ m3d::smdl_c mPutMdl;
    /* 0x34 */ m3d::smdl_c mGetMdl;
    /* 0x50 */ m3d::anmTexSrt_c mPutAnmTexSrt;
    /* 0x7C */ m3d::anmTexSrt_c mGetAnmTexSrt;
};

#endif
