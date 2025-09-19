#ifndef D_CURSOR_HIT_CHECK_H
#define D_CURSOR_HIT_CHECK_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "toBeSorted/tlist.h"

/** An interface for something that can check what the cursor/pointer is hovering over. */
class dCursorHitCheck_c {
    friend class dCsMgr_c;

public:
    dCursorHitCheck_c()
        : mOverrideHitCallback(nullptr), mCallbackData(nullptr), mMask(0), mPriority(0), field_0x017(0) {}
    virtual u32 getType() = 0;
    virtual bool checkHit(s32, s32) = 0;

    bool init(u8, u8, u16 mask);
    bool checkOverrideCallback(bool);

    typedef bool (*overrideHitCallback)(bool, dCursorHitCheck_c *, void *);

private:
    /* 0x04 */ TListNode<dCursorHitCheck_c> mLink;
    /* 0x0C */ overrideHitCallback mOverrideHitCallback;
    /* 0x10 */ void *mCallbackData;
    /* 0x14 */ u16 mMask;
    /* 0x16 */ u8 mPriority;
    /* 0x17 */ u8 field_0x017;
};

/**
 * Checks which object in the 3D world the cursor is pointing at.
 * Purpose unclear, not used for the Sky Keep puzzle.
 */
class dCursorHitCheckCC_c : public dCursorHitCheck_c {
    friend class dCsMgr_c;

public:
    bool init(u16 mask, u8, u8);
    virtual u32 getType() override {
        return 'cc  ';
    }
    virtual bool checkHit(s32, s32) override;
};

struct dCsCheckLyt_BoundingData {
    dCsCheckLyt_BoundingData() {
        mpPane = nullptr;
        mMinX = 0;
        mMaxY = 0;
        mMaxX = 0;
        mMinY = 0;
    }
    virtual ~dCsCheckLyt_BoundingData() {}
    /* 0x04 */ nw4r::lyt::Pane *mpPane;
    /* 0x08 */ s16 mMinX;
    /* 0x0A */ s16 mMaxY;
    /* 0x0C */ s16 mMaxX;
    /* 0x0E */ s16 mMinY;
};

/**
 * Checks which 2D UI Pane the cursor is pointing at. Used for
 * many interactible UI elements.
 */
class dCursorHitCheckLyt_c : public dCursorHitCheck_c {
public:
    dCursorHitCheckLyt_c()
        : mpRootPane(nullptr),
          mpBoundingData(nullptr),
          mNumBoundings(0),
          mHasCalculatedBoundingData(false),
          field_0x23(0),
          mpHitPane(nullptr) {}

    virtual u32 getType() override {
        return 'lyt ';
    }
    virtual bool checkHit(s32, s32) override;
    virtual ~dCursorHitCheckLyt_c();

    bool init(nw4r::lyt::Pane *, u16 mask, u8, u8);
    void execute();

    /** Returns the Pane the cursor is currently hovering over. */
    nw4r::lyt::Pane *getHitPane() const {
        return mpHitPane;
    }

    /** Must be called if the Lyt panes change their position. */
    void resetCachedHitboxes() {
        mHasCalculatedBoundingData = false;
    }

private:
    void countBoundings(nw4r::lyt::Pane *);
    void gatherBoundings(dCsCheckLyt_BoundingData **pEnd, nw4r::lyt::Pane *);

    /* 0x18 */ nw4r::lyt::Pane *mpRootPane;
    /* 0x1C */ dCsCheckLyt_BoundingData *mpBoundingData;
    /* 0x20 */ u16 mNumBoundings;
    /* 0x22 */ bool mHasCalculatedBoundingData;
    /* 0x23 */ u8 field_0x23;
    /* 0x24 */ nw4r::lyt::Pane *mpHitPane;
};

/** The basic cursor position getter. */
class dCursorInterface_c {
    friend class dCsMgr_c;

public:
    dCursorInterface_c() : mCursorMask(0), mpHit(nullptr) {}
    virtual ~dCursorInterface_c() {}
    virtual mVec2_c &getCursorPos();

    void setCursorMask(u16 val) {
        mCursorMask = val;
    }

    dCursorHitCheck_c *getHit() const {
        return mpHit;
    }

private:
    /* 0x04 */ TListNode<dCursorInterface_c> mLink;
    /* 0x0C */ u16 mCursorMask;
    /* 0x10 */ dCursorHitCheck_c *mpHit;
};

/** A variant of the cursor position getter for dCsGame_c */
class dCursorInterfaceGame_c : public dCursorInterface_c {
public:
    dCursorInterfaceGame_c() {}
    virtual ~dCursorInterfaceGame_c() {}
    virtual mVec2_c &getCursorPos();

private:
    /* 0x14 */ mVec2_c field_0x14;
};

/**
 * The cursor manager. Cursors and hit targets are registered here,
 * and this manager will track the target of each pointer.
 */
class dCsMgr_c {
public:
    dCsMgr_c();
    static dCsMgr_c *create(EGG::Heap *heap);

    void execute();

    void registCursor(dCursorInterface_c *cursor);
    void unregistCursor(dCursorInterface_c *cursor);

    void registCursorTarget(dCursorHitCheck_c *target);
    void unregistCursorTarget(dCursorHitCheck_c *target);

    bool isRegist(dCursorHitCheck_c *target);

    static dCsMgr_c *GetInstance() {
        return sInstance;
    }

    typedef TList<dCursorInterface_c, offsetof(dCursorInterface_c, mLink)> CursorList;
    typedef TList<dCursorHitCheck_c, offsetof(dCursorHitCheck_c, mLink)> HitCheckList;

private:
    static dCsMgr_c *sInstance;
    /* 0x00 */ CursorList mList1;
    /* 0x0C */ HitCheckList mList2;
    /* 0x18 */ bool field_0x18;
};

#endif
