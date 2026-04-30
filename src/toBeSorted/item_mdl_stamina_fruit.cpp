#include "toBeSorted/item_mdl_stamina_fruit.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "toBeSorted/arc_managers/oarc_manager.h"

bool dItemMdlStaminaFruit_c::isMdlForItemId(u16 itemId) {
    return itemId == ITEM_STAMINA_FRUIT;
}

bool dItemMdlStaminaFruit_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    void *resData1 = OarcManager::GetInstance()->getMdlFromArc2("PutGuts");
    if (resData1 == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res1(resData1);
    if (!res1.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl1 = res1.GetResMdl("PutGuts");
    if (!mdl1.IsValid()) {
        return false;
    }
    if (!mMdl1.create(mdl1, allocator, 0x324)) {
        return false;
    }

    void *resData2 = OarcManager::GetInstance()->getMdlFromArc2("GetGuts");
    if (resData2 == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res2(resData2);
    if (!res2.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl2 = res2.GetResMdl("GetGuts");
    if (!mdl2.IsValid()) {
        return false;
    }
    if (!mMdl2.create(mdl2, allocator, 0x324)) {
        return false;
    }

    nw4r::g3d::ResAnmTexSrt anmTexSrt1 = res1.GetResAnmTexSrt("GutsLight");
    if (!anmTexSrt1.IsValid()) {
        return false;
    }
    TRY_CREATE(mAnmTexSrt1.create(mdl1, anmTexSrt1, allocator, nullptr, 1));
    mMdl1.setAnm(mAnmTexSrt1);

    nw4r::g3d::ResAnmTexSrt anmTexSrt2 = res2.GetResAnmTexSrt("GutsLight");
    if (!anmTexSrt2.IsValid()) {
        return false;
    }
    TRY_CREATE(mAnmTexSrt2.create(mdl2, anmTexSrt2, allocator, nullptr, 1));
    mMdl2.setAnm(mAnmTexSrt2);

    mpItem = item;

    return true;
}

void dItemMdlStaminaFruit_c::vt_0x10(u8 arg) {
    if (arg == ITEM_MDL_UNK0x14_3) {
        field_0x14 = ITEM_MDL_UNK0x14_1;
    } else {
        field_0x14 = arg;
    }
}

void dItemMdlStaminaFruit_c::setScale(const mVec3_c &scale) {
    mMdl1.setScale(scale);
    mMdl2.setScale(scale);
}

void dItemMdlStaminaFruit_c::setLocalMtx(const mMtx_c &mtx) {
    mMdl1.setLocalMtx(mtx);
    mMdl2.setLocalMtx(mtx);
}

void dItemMdlStaminaFruit_c::draw() {
    switch (field_0x14) {
        case ITEM_MDL_UNK0x14_1:
            mAnmTexSrt1.play();
            mpItem->fn_8002ECD0(&mMdl1, 7);
            break;
        case ITEM_MDL_UNK0x14_2:
            mAnmTexSrt2.play();
            mpItem->fn_8002ECD0(&mMdl2, 6);
            break;
    }
}

void dItemMdlStaminaFruit_c::vt_0x20(u16 itemId) {
    // no-op
}

void dItemMdlStaminaFruit_c::setPriorityDraw() {
    mMdl1.setPriorityDraw(0x82, 0x7F);
    mMdl2.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlStaminaFruit_c::unsetPriorityDraw() {
    mMdl1.setPriorityDraw(0x7F, 0x7F);
    mMdl2.setPriorityDraw(0x7F, 0x7F);
}
