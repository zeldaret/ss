#include "toBeSorted/item_mdl_rupee.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "toBeSorted/arc_managers/oarc_manager.h"

const dItemMdlRupee_c::MdlConfig dItemMdlRupee_c::sMdlConfig[] = {
    { ITEM_GREEN_RUPEE, 0.0f},
    {  ITEM_BLUE_RUPEE, 1.0f},
    {   ITEM_RED_RUPEE, 2.0f},
    {ITEM_SILVER_RUPEE, 3.0f},
    {  ITEM_GOLD_RUPEE, 4.0f},
    {      ITEM_RUPOOR, 5.0f},
    {                0, 0.0f},
};

bool dItemMdlRupee_c::isMdlForItemId(u16 itemId) {
    // NON-DETERMINISTIC CODEGEN AAAAAAAAAAAAAAAAAAAAAA
    return itemId == ITEM_GREEN_RUPEE || itemId == ITEM_BLUE_RUPEE || itemId == ITEM_RED_RUPEE ||
           itemId == ITEM_SILVER_RUPEE || itemId == ITEM_GOLD_RUPEE || itemId == ITEM_RUPOOR;
}

bool dItemMdlRupee_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    if (item == nullptr) {
        return false;
    }

    // no-op
    int i = 0;
    bool found = false;
    while (!found && i < ARRAY_LENGTH(sMdlConfig)) {
        if (itemId == sMdlConfig[i].itemId) {
            found = true;
        } else {
            i++;
        }
    }

    if (!found) {
        return false;
    }

    {
        void *resData1 = OarcManager::GetInstance()->getMdlFromArc2("PutRupee");
        if (resData1 == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile res1(resData1);
        if (!res1.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl mdl1 = res1.GetResMdl("PutRupee");
        if (!mdl1.IsValid()) {
            return false;
        }
        if (!mMdl1.create(mdl1, allocator, 0x123)) {
            return false;
        }

        nw4r::g3d::ResAnmTexPat anmTexPat1 = res1.GetResAnmTexPat("Rupee");
        if (!anmTexPat1.IsValid()) {
            return false;
        }
        if (!mAnmTexPat1.create(mdl1, anmTexPat1, allocator, nullptr, 1)) {
            return false;
        }
        mMdl1.setAnm(mAnmTexPat1);
        mAnmTexPat1.setFrame(sMdlConfig[i].texPatFrame, 0);
    }

    {
        void *resData2 = OarcManager::GetInstance()->getMdlFromArc2("GetRupee");
        if (resData2 == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile res2(resData2);
        if (!res2.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl mdl2 = res2.GetResMdl("GetRupee");
        if (!mdl2.IsValid()) {
            return false;
        }
        if (!mMdl2.create(mdl2, allocator, 0x123)) {
            return false;
        }

        nw4r::g3d::ResAnmTexPat anmTexPat2 = res2.GetResAnmTexPat("Rupee");
        if (!anmTexPat2.IsValid()) {
            return false;
        }
        anmTexPat2.Bind(res2);
        if (!mAnmTexPat2.create(mdl2, anmTexPat2, allocator, nullptr, 1)) {
            return false;
        }
        mMdl2.setAnm(mAnmTexPat2);
        mAnmTexPat2.setFrame(sMdlConfig[i].texPatFrame, 0);
    }
    

    mpItem = item;
    return true;
}

void dItemMdlRupee_c::vt_0x10(u8 arg) {
    if (arg == ITEM_MDL_UNK0x14_3) {
        field_0x14 = ITEM_MDL_UNK0x14_1;
    } else {
        field_0x14 = arg;
    }
}

void dItemMdlRupee_c::setScale(const mVec3_c &scale) {
    mMdl1.setScale(scale);
    mMdl2.setScale(scale);
}

void dItemMdlRupee_c::setLocalMtx(const mMtx_c &mtx) {
    mMdl1.setLocalMtx(mtx);
    mMdl2.setLocalMtx(mtx);
}

void dItemMdlRupee_c::draw() {
    switch (field_0x14) {
        case ITEM_MDL_UNK0x14_1: mpItem->fn_8002ECD0(&mMdl1, 7); break;
        case ITEM_MDL_UNK0x14_2: mpItem->fn_8002ECD0(&mMdl2, 6); break;
    }
}

void dItemMdlRupee_c::vt_0x20(u16 itemId) {
    // no-op
    int i = 0;
    bool found = false;
    while (!found && i < ARRAY_LENGTH(sMdlConfig)) {
        if (itemId == sMdlConfig[i].itemId) {
            found = true;
        } else {
            i++;
        }
    }

    if (found) {
        mAnmTexPat1.setFrame(sMdlConfig[i].texPatFrame, 0);
        mAnmTexPat2.setFrame(sMdlConfig[i].texPatFrame, 0);
    }
}

void dItemMdlRupee_c::setPriorityDraw() {
    mMdl1.setPriorityDraw(0x82, 0x7F);
    mMdl2.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlRupee_c::unsetPriorityDraw() {
    mMdl1.setPriorityDraw(0x7F, 0x7F);
    mMdl2.setPriorityDraw(0x7F, 0x7F);
}
