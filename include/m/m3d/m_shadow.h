#ifndef M3D_M_SHADOW_H
#define M3D_M_SHADOW_H

#include "egg/core/eggFrmHeap.h"
#include "egg/core/eggHeap.h"
#include "m/m3d/m_proc.h"
#include "m/m_color.h"
#include "m/m_frustum.h"
#include "m/m_heap.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"

namespace m3d {

/**
 * A configuration struct for our global shadow manager.
 * The game uses one for most rooms and one for the Koloktos boss fight.
 */
struct mShadowCircleConfig {
    u32 heapSize;
    u8 count;
    u8 unk1;
    u8 unk2;
    u8 texBufferSize;
    u8 drawOpaPriority;
};

/**
 * A callback that allows the stage manager to customize parts of
 * the shadow pipeline.
 */
class mShadowCallback_c {
public:
    virtual ~mShadowCallback_c() {}
    virtual void beforeDraw() = 0;
    virtual void draw(const mMtx_c &, u32) = 0;
    virtual void afterDraw() = 0;
};

class mShadowCircle_c;

/**
 * Represents an internal shadow circle.
 */
class mShadowChild_c {
    friend class mShadow_c;

public:
    nw4r::ut::Node mNode;

    mShadowChild_c() : mpCircle(nullptr), mpLeaves(nullptr), field_0x154(0), mNumLeaves(0) {}
    /* vt at 0x08 */
    virtual ~mShadowChild_c();

    bool create(u8, EGG::Heap *);
    void set(const mVec3_c &pos, f32 dist, mColor color);
    bool setGeom(const GXTexObj *texObj, const mMtx_c &mtx, const mQuat_c &quat);
    void draw();
    void updateMtx();
    void drawMdl();

    bool addMdl(scnLeaf_c &mdl, const mQuat_c &quat);

    void set0x154(UNKWORD arg) {
        field_0x154 = arg;
    }

private:
    /* 0x00C */ EGG::Heap *mpHeap;
    /* 0x010 */ mShadowCircle_c *mpCircle;
    /* 0x014 */ u32 mPriorityMaybe;
    /* 0x018 */ GXTexObj mTexObj;
    /* 0x038 */ mColor mShadowColor;
    /* 0x03C */ mQuat_c mQuat;
    /* 0x04C */ mFrustum_c mFrustum;
    /* 0x13C */ f32 field_0x13C;
    /* 0x140 */ scnLeaf_c **mpLeaves;
    /* 0x144 */ mVec3_c mPositionMaybe;
    /* 0x150 */ f32 mOffsetMaybe;
    /* 0x154 */ UNKWORD field_0x154;
    /* 0x158 */ u8 mMaxNumLeaves;
    /* 0x159 */ u8 mNumLeaves;
    /* 0x15A */ u8 mColorChanIdx;
};

/**
 * An RAII handle to a shadow circle.
 */
class mShadowCircle_c {
    friend class mShadow_c;
    friend class mShadowChild_c;

public:
    mShadowCircle_c() : mpChild(nullptr) {}
    virtual ~mShadowCircle_c();

private:
    mShadowChild_c *mpChild;
};

/**
 * The global shadow circle manager.
 */
class mShadow_c : public proc_c {
    friend class mShadowCircle_c;
    friend class mShadowChild_c;

public:
    mShadow_c(EGG::Heap *heap)
        : mpHeap(heap), mpCurrentHeap(nullptr), mpChilds(nullptr), mpTexBuf(nullptr), mpCallback(nullptr),
          mCurrentHeapIdx(0), mFreeChildIdx(0), field_0x66(true) {
        nw4r::ut::List_Init(&mList, 0);
    }
    virtual ~mShadow_c();
    virtual void remove() override;
    virtual void drawOpa() override;

    bool drawMdl(
        mShadowCircle_c *circle, u32 priority, scnLeaf_c &mdl, const mQuat_c &quat, mVec3_c &pos, mColor color,
        u32 param9, f32 dist
    );
    bool drawTexObj(
        mShadowCircle_c *circle, u32 priority, const GXTexObj *texObj, const mMtx_c &mtx, const mQuat_c &quat,
        mVec3_c &pos, mColor color, u32 param9, f32 dist
    );

    bool addMdlToCircle(mShadowCircle_c *circle, scnLeaf_c &mdl, const mQuat_c &quat);

    void beforeDraw();
    void draw(const mMtx_c &, u32);
    void afterDraw();

    void changeHeap(int index) {
        mCurrentHeapIdx = index % 2;
        mpCurrentHeap = mpFrmHeaps[mCurrentHeapIdx];
    }
    void swapHeaps();
    void
    create(int count, u8 unk1, int unk2, u16 texBufferSize, u32 drawOpaPriority, nw4r::g3d::ResMdl mdl, u32 heapSize);
    void reset();

    bool addCircle(mShadowCircle_c *circle, u32 priority, u32 unk);
    void removeCircle(mShadowCircle_c *circle);

    void drawAllShadows();

    static void create(const mShadowCircleConfig *, nw4r::g3d::ResMdl mdl, EGG::Heap *heap);
    static void destroy();

    static mShadow_c *sInstance;

private:
    /* 0x18 */ EGG::Heap *mpHeap;
    /* 0x1C */ mAllocator_c mAllocator;
    /* 0x38 */ EGG::FrmHeap *mpFrmHeaps[2];
    /* 0x40 */ EGG::FrmHeap *mpCurrentHeap;
    /* 0x44 */ nw4r::ut::List mList;
    /* 0x50 */ mShadowChild_c *mpChilds;
    /* 0x54 */ void *mpTexBuf;
    /* 0x58 */ u32 mTexBufferSize;
    /* 0x5C */ mShadowCallback_c *mpCallback;
    /* 0x60 */ u8 mCurrentHeapIdx;
    /* 0x61 */ u8 mChildCount;
    /* 0x62 */ u8 mFreeChildIdx;
    /* 0x63 */ u8 mNumTexBuffers;
    /* 0x64 */ u8 mFreeTexIdx;
    /* 0x65 */ u8 mTexSize;
    /* 0x66 */ bool field_0x66;

public:
    static GXTexObj *sTexObj;
    static nw4r::g3d::ResShp sResShp;
};

class mCustomShadow_c : public scnLeaf_c {
public:
    virtual ~mCustomShadow_c();
    virtual int getType() const override;
    /* 0x24 */ virtual void draw(const mMtx_c &);

    void calc(mMtx_c, mMtx_c &);

    /* 0x18 */ mMtx_c mMtx;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4C;
};

} // namespace m3d

#endif
