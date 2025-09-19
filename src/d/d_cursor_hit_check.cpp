#include "d/d_cursor_hit_check.h"

#include "common.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_gfx.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_stage_mgr.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"
#include "toBeSorted/d_d3d.h"

dCsMgr_c::dCsMgr_c() {
    // NONMATCHING - TList ctor issues
    field_0x18 = true;
    sInstance = this;
}

void dCsMgr_c::execute() {
    // NONMATCHING - TList
    if (field_0x18 == 0 && !dPadNav::isPointerVisible()) {
        for (CursorList::Iterator it = mList1.GetBeginIter(); it != mList1.GetEndIter(); ++it) {
            it->mpHit = nullptr;
        }
        return;
    }

    for (CursorList::Iterator it = mList1.GetBeginIter(); it != mList1.GetEndIter(); ++it) {
        mVec2_c &pos = it->getCursorPos();
        s16 x = pos.x;
        s16 y = pos.y;
        it->mpHit = 0;
        u32 mask = it->mCursorMask;

        if (mask) {
            for (HitCheckList::Iterator chkIt = mList2.GetBeginIter(); chkIt != mList2.GetEndIter(); ++chkIt) {
                if ((chkIt->mMask & mask) != 0) {
                    bool hit = chkIt->checkHit(x, y);
                    if (chkIt->checkOverrideCallback(hit)) {
                        it->mpHit = &*chkIt;
                        break;
                    }
                }
            }
        }
    }
}

void dCsMgr_c::registCursor(dCursorInterface_c *cursor) {
    mList1.append(cursor);
}

void dCsMgr_c::unregistCursor(dCursorInterface_c *cursor) {
    // NONMATCHING - TList
    mList1.remove(cursor);
}

void dCsMgr_c::registCursorTarget(dCursorHitCheck_c *target) {
    // NONMATCHING - TList
    HitCheckList::Iterator it = mList2.GetBeginIter();
    HitCheckList::Iterator end = mList2.GetEndIter();
    while (it != end) {
        if (target->mPriority < it->mPriority) {
            if (it == end) {
                mList2.prepend(target);
                return;
            }

            mList2.insertBefore(&*it, target);
            return;
        }
        ++it;
    }
    mList2.append(target);
}

void dCsMgr_c::unregistCursorTarget(dCursorHitCheck_c *target) {
    // NONMATCHING - TList
    for (CursorList::Iterator it = mList1.GetBeginIter(); it != mList1.GetEndIter(); ++it) {
        if (target == it->mpHit) {
            it->mpHit = 0;
        }
    }
    mList2.remove(target);
}

bool dCsMgr_c::isRegist(dCursorHitCheck_c *target) {
    return mList2.GetPosition(target) != mList2.GetEndIter();
}

dCsMgr_c *dCsMgr_c::create(EGG::Heap *heap) {
    return new (heap) dCsMgr_c();
}

bool dCursorHitCheck_c::init(u8 prio, u8 a, u16 mask) {
    if (dCsMgr_c::GetInstance()->isRegist(this)) {
        return false;
    }
    mPriority = prio;
    field_0x017 = a;
    mMask = mask;
    return true;
}

bool dCursorHitCheck_c::checkOverrideCallback(bool b) {
    if (mOverrideHitCallback == nullptr) {
        return b;
    }
    return mOverrideHitCallback(b, this, mCallbackData);
}

bool dCursorHitCheckCC_c::checkHit(s32 x, s32 y) {
    // NONMATCHING - TODO

    mVec3_c pos(
        (x - dGfx_c::getWidth4x3LeftF()) / dGfx_c::getWidth4x3F() * 2.f - 1.f,
        (y - dGfx_c::getCurrentScreenTopF()) / dGfx_c::getCurrentScreenHeightF() * -2.f - 1.f, 10000.f
    );

    mVec3_c v;
    d3d::fn_80016B60(v, pos);

    nw4r::g3d::Camera cam = dStageMgr_c::GetInstance()->getCamera(0);

    mMtx_c mtx;
    mVec3_c a;

    cam.GetCameraMtx(mtx);
    mtx.inverse();
    mtx.getTranslation(a);

    dBgS_LinChk ck;
    ck.Set(&a, &v, nullptr);
    dBgS::GetInstance()->LineCross(&ck);
    cCcD_ShapeAttr *attr = nullptr;
    if (dCcS::GetInstance()->fn_80357c90(a, v, 0, &attr, 15.0f)) {
        // TODO - this branches into unsplit code in col/
        return true;
    } else {
        return false;
    }
}

bool dCursorHitCheckCC_c::init(u16 mask, u8 prio, u8 a) {
    return dCursorHitCheck_c::init(prio, a, mask);
}

dCursorHitCheckLyt_c::~dCursorHitCheckLyt_c() {
    delete[] mpBoundingData;
    mpBoundingData = nullptr;
}

bool dCursorHitCheckLyt_c::checkHit(s32 x, s32 y) {
    dCsCheckLyt_BoundingData *p = &mpBoundingData[0];
    for (u16 i = 0; i < mNumBoundings; i++) {
        if (p->mMinX <= x && x <= p->mMaxX && p->mMaxY >= y && y >= p->mMinY) {
            mpHitPane = p->mpPane;
            if (mpHitPane->GetAlpha() != 0 && mpHitPane->IsVisible()) {
                return true;
            }
        }
        p++;
    }

    mpHitPane = nullptr;
    return false;
}

bool dCursorHitCheckLyt_c::init(nw4r::lyt::Pane *pPane, u16 mask, u8 prio, u8 a) {
    if (!dCursorHitCheck_c::init(prio, a, mask)) {
        return false;
    }

    mpRootPane = pPane;
    mNumBoundings = 0;

    countBoundings(pPane);

    dCsCheckLyt_BoundingData *pData = new dCsCheckLyt_BoundingData[mNumBoundings]();
    mpBoundingData = pData;
    dCsCheckLyt_BoundingData *pEnd = pData + mNumBoundings;
    gatherBoundings(&pEnd, mpRootPane);

    return true;
}

void dCursorHitCheckLyt_c::execute() {
    if (!mHasCalculatedBoundingData) {
        dCsCheckLyt_BoundingData *dat = &mpBoundingData[0];
        for (u16 i = 0; i < mNumBoundings; i++) {
            nw4r::lyt::Pane *pane = dat->mpPane;
            f32 xx = pane->GetGlobalMtx()._00;
            switch (pane->GetPositionH()) {
                case 0:
                    dat->mMinX = (s16)pane->GetGlobalMtx()._03;
                    dat->mMaxX = (s16)pane->GetGlobalMtx()._03 + xx * pane->GetSize().width;
                    break;
                case 1:
                    dat->mMinX = (s16)pane->GetGlobalMtx()._03 - xx * pane->GetSize().width * 0.5f;
                    dat->mMaxX = (s16)pane->GetGlobalMtx()._03 + xx * pane->GetSize().width * 0.5f;
                    break;
                case 2:
                    dat->mMinX = (s16)pane->GetGlobalMtx()._03 - xx * pane->GetSize().width;
                    dat->mMaxX = (s16)pane->GetGlobalMtx()._03;
                    break;
            }

            switch (pane->GetPositionV()) {
                case 0:
                    dat->mMaxY = (s16)pane->GetGlobalMtx()._13;
                    dat->mMinY = (s16)pane->GetGlobalMtx()._13 - pane->GetSize().height;
                    break;
                case 1:
                    dat->mMaxY = (s16)pane->GetGlobalMtx()._13 + (pane->GetSize().height * 0.5f);
                    dat->mMinY = (s16)pane->GetGlobalMtx()._13 - (pane->GetSize().height * 0.5f);
                    break;
                case 2:
                    dat->mMaxY = (s16)pane->GetGlobalMtx()._13 + pane->GetSize().height;
                    dat->mMinY = (s16)pane->GetGlobalMtx()._13;
                    break;
            }

            dat++;
        }

        mHasCalculatedBoundingData = true;
    }
}

void dCursorHitCheckLyt_c::countBoundings(nw4r::lyt::Pane *pane) {
    nw4r::lyt::Bounding *b = nw4r::ut::DynamicCast<nw4r::lyt::Bounding *>(pane);
    if (b != nullptr) {
        mNumBoundings++;
    }

    for (nw4r::lyt::Pane::ChildList::ReverseIterator it = pane->GetChildList()->GetEndReverseIter();
         it != pane->GetChildList()->GetBeginIter(); ++it) {
        countBoundings(&*it);
    }
}

void dCursorHitCheckLyt_c::gatherBoundings(dCsCheckLyt_BoundingData **pEnd, nw4r::lyt::Pane *pane) {
    nw4r::lyt::Bounding *b = nw4r::ut::DynamicCast<nw4r::lyt::Bounding *>(pane);
    if (b != nullptr) {
        (*pEnd)--;
        (*pEnd)->mpPane = pane;
    }

    for (nw4r::lyt::Pane::ChildList::ReverseIterator it = pane->GetChildList()->GetEndReverseIter();
         it != pane->GetChildList()->GetBeginIter(); ++it) {
        gatherBoundings(pEnd, &*it);
    }
}

mVec2_c &dCursorInterface_c::getCursorPos() {
    return dPad::ex_c::getInstance()->mDpdPosScreen;
}
