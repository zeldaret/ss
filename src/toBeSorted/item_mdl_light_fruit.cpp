#include "toBeSorted/item_mdl_light_fruit.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "toBeSorted/arc_managers/oarc_manager.h"


bool dItemMdlLightFruit_c::isMdlForItemId(u16 itemId) {
    return itemId == ITEM_LIGHT_FRUIT;
}

bool dItemMdlLightFruit_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    if (item == nullptr) {
        return false;
    }

    {
        void *resData1 = OarcManager::GetInstance()->getMdlFromArc2("PutHoly");
        if (resData1 == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile res1(resData1);
        if (!res1.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl mdl1 = res1.GetResMdl("PutHoly");
        if (!mdl1.IsValid()) {
            return false;
        }
        if (!mPutMdl.create(mdl1, allocator, 0x324)) {
            return false;
        }

        nw4r::g3d::ResAnmTexSrt anmTexSrt1 = res1.GetResAnmTexSrt("Holy");
        if (!anmTexSrt1.IsValid()) {
            return false;
        }
        TRY_CREATE(mPutAnmTexSrt.create(mdl1, anmTexSrt1, allocator, nullptr, 1));
        mPutMdl.setAnm(mPutAnmTexSrt);
    }

    {
        void *resData2 = OarcManager::GetInstance()->getMdlFromArc2("GetHoly");
        if (resData2 == nullptr) {
            // return false;
        }
        nw4r::g3d::ResFile res2(resData2);
        if (!res2.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl mdl2 = res2.GetResMdl("GetHoly");
        if (!mdl2.IsValid()) {
            return false;
        }
        if (!mGetMdl.create(mdl2, allocator, 0x324)) {
            return false;
        }

        nw4r::g3d::ResAnmTexSrt anmTexSrt2 = res2.GetResAnmTexSrt("Holy");
        if (!anmTexSrt2.IsValid()) {
            return false;
        }
        TRY_CREATE(mGetAnmTexSrt.create(mdl2, anmTexSrt2, allocator, nullptr, 1));
        mGetMdl.setAnm(mGetAnmTexSrt);
    }

    mpItem = item;
    return true;
}

void dItemMdlLightFruit_c::setDrawMode(u8 arg) {
    if (arg == DrawMode_Max) {
        mDrawMode = DrawMode_Put;
    } else {
        mDrawMode = arg;
    }
}

void dItemMdlLightFruit_c::setScale(const mVec3_c &scale) {
    mPutMdl.setScale(scale);
    mGetMdl.setScale(scale);
}

void dItemMdlLightFruit_c::setLocalMtx(const mMtx_c &mtx) {
    mPutMdl.setLocalMtx(mtx);
    mGetMdl.setLocalMtx(mtx);
}

void dItemMdlLightFruit_c::draw() {
    switch (mDrawMode) {
        case DrawMode_Put:
            mPutAnmTexSrt.play();
            mpItem->fn_8002ECD0(&mPutMdl, 7);
            break;
        case DrawMode_Get:
            mGetAnmTexSrt.play();
            mpItem->fn_8002ECD0(&mGetMdl, 6);
            break;
    }
}

void dItemMdlLightFruit_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlLightFruit_c::setPriorityDraw() {
    mPutMdl.setPriorityDraw(0x82, 0x7F);
    mGetMdl.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlLightFruit_c::unsetPriorityDraw() {
    mPutMdl.setPriorityDraw(0x7F, 0x7F);
    mGetMdl.setPriorityDraw(0x7F, 0x7F);
}
