#include "egg/core/eggXfbManager.h"

#include "common.h"
#include "egg/core/eggXfb.h"
#include "nw4r/db/db_directPrint.h"
#include "rvl/VI.h" // IWYU pragma: export


namespace EGG {

/* 80498af0 */ bool XfbManager::isRegisterd(Xfb &xfb) const {
    Xfb *x = mNextXfb;
    Xfb *iter = x;

    if (mNextXfb != nullptr) {
        do {
            if (iter == &xfb) {
                return true;
            }
            iter = iter->mNext;
        } while (iter != x);
    }
    return false;
}

/* 80498b30 */ bool XfbManager::attach(Xfb *xfb) {
    int interrupts = OSDisableInterrupts();
    bool u3 = 0;

    if (xfb != nullptr && !isRegisterd(*xfb)) {
        xfb->mState = Xfb::XFB_UNPROCESSED;
        if (mNextXfb == nullptr) {
            mNextXfb = xfb;
            mToCopyXfb = xfb;
            xfb->mNext = xfb;
            xfb->mPrev = xfb;
        } else {
            mNextXfb->mPrev->mNext = xfb;
            xfb->mPrev = mNextXfb->mPrev;
            mNextXfb->mPrev = xfb;
            xfb->mNext = mNextXfb;
        }
        u3 = 1;
        mNumXfbs += 1;
        mNumXfbs_Copy += 1;
    }
    OSRestoreInterrupts(interrupts);
    return u3;
}
/* 80498c10 */ void XfbManager::copyEFB(bool bUpdate) {
    if (mNumXfbs == 1 && mToCopyXfb == nullptr) {
        mToCopyXfb = mNextXfb;
    }

    if (mToCopyXfb != nullptr) {
        if (bUpdate) {
            GXSetZMode(true, GX_ALWAYS, true);
            GXSetAlphaUpdate(true);
            GXSetColorUpdate(true);
        }

        GXCopyDisp(mToCopyXfb->mBuffer, bUpdate);

        GXFlush();
        GXDrawDone();
        u32 interrupts = OSDisableInterrupts();
        mToCopyXfb->mState = Xfb::XFB_COPIED;
        if (mToShowXfb == nullptr) {
            mToShowXfb = mToCopyXfb;
        }

        Xfb *next = mToCopyXfb->mNext->mState == Xfb::XFB_UNPROCESSED ? mToCopyXfb->mNext : nullptr;
        mToCopyXfb = next;
        OSRestoreInterrupts(interrupts);
    }
}

/* 80498d00 */ void XfbManager::setNextFrameBuffer() {
    if (mToShowXfb != nullptr) {
        if (mNumXfbs_Copy > 2) {
            mNumXfbs_Copy -= 1;
        } else {
            VISetNextFrameBuffer(mToShowXfb->mBuffer);
            VIFlush();
            nw4r::db::DirectPrint_ChangeXfb(mToShowXfb->mBuffer, mToShowXfb->mWidth, mToShowXfb->mHeight);
            if (mNumXfbs > 1) {
                mNextXfb->mState = Xfb::XFB_UNPROCESSED;
                if (mToCopyXfb == nullptr) {
                    mToCopyXfb = mNextXfb;
                }
                mToShowXfb->mState = Xfb::XFB_SHOWN;
                mNextXfb = mToShowXfb;
                mToShowXfb = mToShowXfb->mNext->mState == Xfb::XFB_COPIED ? mToShowXfb->mNext : nullptr;
            } else {
                mToShowXfb->mState = Xfb::XFB_UNPROCESSED;
                if (mToCopyXfb == nullptr) {
                    mToCopyXfb = mToShowXfb;
                }
                mToShowXfb = nullptr;
            }
        }
    } else {
        if (mNumXfbs == 1 && mNextXfb != nullptr) {
            mNextXfb->mState = Xfb::XFB_UNPROCESSED;
            mToCopyXfb = mNextXfb;
        }
    }
}

} // namespace EGG
