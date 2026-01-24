#include "d/lyt/d_lyt_deposit_box_cursor.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_pad_nav.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_util_items.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

static const d2d::LytBrlanMapping brlanMap[] = {
    {       "depositBoxCursor_00_loop.brlan",        "G_loop_00"},
    {"depositBoxCursor_00_stickCursor.brlan", "G_stickCursor_00"},
};

#define DEPOSIT_BOX_CURSOR_ANIM_LOOP 0
#define DEPOSIT_BOX_CURSOR_ANIM_STICK_CURSOR 1

#define DEPOSIT_BOX_CURSOR_NUM_ANIMS 2

bool dLytDepositBoxCursor_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("depositBoxCursor_00.brlyt", nullptr);
    mLyt.setPriority(0x88);

    for (int i = 0; i < DEPOSIT_BOX_CURSOR_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[DEPOSIT_BOX_CURSOR_ANIM_LOOP].setAnimEnable(true);
    mAnm[DEPOSIT_BOX_CURSOR_ANIM_LOOP].setFrame(0.0f);
    mLyt.calc();

    mpStickCursorPane = mLyt.findPane("N_stickCursor_00");
    mpGetIconPane = mLyt.findPane("N_getIcon_00");
    initIcon();
    mIsVisible = false;
    mIsSellMode = false;
    return true;
}

bool dLytDepositBoxCursor_c::execute() {
    realizeNav();
    mAnm[DEPOSIT_BOX_CURSOR_ANIM_LOOP].play();
    if (mIsVisible) {
        mNode.mpLytPane->execute();
    }
    mLyt.calc();

    return true;
}

bool dLytDepositBoxCursor_c::draw() {
    if (mIsVisible) {
        mLyt.addToDrawList();
    }
    return true;
}

bool dLytDepositBoxCursor_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    mNode.mpLytPane->remove();

    mLyt.unbindAnims();

    for (int i = 0; i < DEPOSIT_BOX_CURSOR_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

static const char *sGroupName = "G_ref_00";

void dLytDepositBoxCursor_c::initIcon() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    d2d::ResAccIf_c resAcc;
    resAcc.attach(data, "");

    mNode.mpLytPane = &mIcon;
    mIcon.build(&resAcc, dLytCommonIconItem_c::POUCH);
    mSubpaneList.PushBack(&mNode);

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mSubpaneList);
        }
    }

    mIcon.setUnk(false);
    mIcon.setHasNumber(false);
    mIcon.setNumber(0);
    mIcon.setVisible(false);

    mNode.mpPane->SetVisible(false);
}

void dLytDepositBoxCursor_c::realizeNav() {
    s32 s;
    if (mSlot < 0) {
        s = mAnm[DEPOSIT_BOX_CURSOR_ANIM_STICK_CURSOR].getLastFrame();
    } else if (dPadNav::isPointerVisible()) {
        s = 4;
    } else if (mItem == LYT_CMN_PouchPotionHealthPlusPlusHalf) {
        s = 5;
    } else if (mIsSellMode) {
        s = 6;
    } else if (mItemLocation == LOC_POUCH) {
        // Flip the cursor for pouch slot 0 so that it's not above the screen edge
        s = mSlot == 0 ? 1 : 0;
        if (mItem == LYT_CMN_PouchBlank4) {
            s += 2;
        }
    } else if (mItem == LYT_CMN_PouchBlank4) {
        s = 2;
    } else {
        s = 0;
    }

    mVec3_c v;
    if (dPadNav::isPointerVisible()) {
        mVec2_c pos = dCsBase_c::GetInstance()->getCursorIf()->getCursorPos();
        v.set(pos.x, pos.y + 15.0f, 0.0f);
    } else {
        nw4r::math::MTX34 mtx = dCsBase_c::GetInstance()->getCursorStickAllPane()->GetGlobalMtx();
        v.set(mtx._03, mtx._13, 0.0f);
    }

    mpStickCursorPane->SetTranslate(v);

    mAnm[DEPOSIT_BOX_CURSOR_ANIM_STICK_CURSOR].setAnimEnable(true);
    mAnm[DEPOSIT_BOX_CURSOR_ANIM_STICK_CURSOR].setFrame(s);
    mLyt.calc();
    mAnm[DEPOSIT_BOX_CURSOR_ANIM_STICK_CURSOR].setAnimEnable(false);
}

void dLytDepositBoxCursor_c::init(s32 location, s32 slot, s32 item, bool isSell) {
    mItemLocation = location;
    mIsSellMode = isSell;

    s32 number;
    f32 durability;
    u8 color;
    if (mItemLocation == LOC_POUCH) {
        number = getPouchItemAmount(slot, false);
        durability = getPouchShieldDurability(slot, false);
        color = getPouchItemNumberColor(slot, false);
        mSlot = slot;
    } else {
        number = getDepositItemAmount(slot);
        durability = getDepositShieldDurability(slot);
        color = getDepositItemNumberColor(slot);
        mSlot = slot % NUM_ICONS_PER_PAGE;
    }

    if (number < 0) {
        mIcon.setHasNumber(false);
    } else {
        mIcon.setHasNumber(true);
        mIcon.setNumber(number);
    }
    mIcon.setUnk(false);
    mIcon.setItem(item);
    if (color != 3) {
        mIcon.setNumberColor(color);
    }
    if (durability >= 0.0f) {
        mIcon.setShieldDurability(durability);
    }

    mNode.mpPane->SetVisible(true);
    mItem = LYT_CMN_PouchBlank4;
    realizeNav();
    mIsVisible = true;
}

void dLytDepositBoxCursor_c::updateSlot(s32 location, s32 slot, s32 item, bool isSell) {
    mItem = item;
    mItemLocation = location;
    mIsSellMode = isSell;
    if (mItemLocation == LOC_POUCH) {
        mSlot = slot;
    } else {
        mSlot = slot % NUM_ICONS_PER_PAGE;
    }
}

void dLytDepositBoxCursor_c::activateSell() {
    mIsSellMode = true;
    realizeNav();
}
