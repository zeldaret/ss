#ifndef TOBESORTED_ITEM_MDL_H
#define TOBESORTED_ITEM_MDL_H

#include "m/m_allocator.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "m/types_m.h"
#include "toBeSorted/mdl_base.h"

// all names made up

class dAcItem_c;

// vtable at 80533eb0
class dItemMdl_c : public dMdl_c {
public:
    dItemMdl_c() : mDrawMode(DrawMode_None) {}
    /* 0x08 */ virtual ~dItemMdl_c() {}
    /* 0x0C */ virtual bool init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) = 0;
    /* 0x10 */ virtual void setDrawMode(u8) = 0;
    /* 0x14 */ virtual void setScale(const mVec3_c &scale) = 0;
    /* 0x18 */ virtual void setLocalMtx(const mMtx_c &mtx) = 0;
    /* 0x1C */ virtual void draw() = 0;
    /* 0x20 */ virtual void changeItemId(u16 itemId) = 0;
    /* 0x24 */ virtual void setPriorityDraw() = 0;
    /* 0x28 */ virtual void unsetPriorityDraw() = 0;
    /* 0x2C */ virtual m3d::smdl_c *getMdl() {
        return nullptr;
    }

    enum DrawMode_e {
        DrawMode_None,
        DrawMode_Put,
        DrawMode_Get,
        DrawMode_Max,
    };

protected:
    /* 0x10 */ dAcItem_c *mpItem;
    /* 0x14 */ u8 mDrawMode;
};

#endif
