#ifndef M_M2D_H
#define M_M2D_H

#include "m/m_allocator.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_arcResourceAccessor.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/ut.h" // IWYU pragma: export

namespace m2d {

extern nw4r::ut::List l_list;
extern mAllocator_c *l_allocator;

class Layout_c : public nw4r::lyt::Layout {
public:
    virtual ~Layout_c() {}
    virtual nw4r::lyt::AnimTransform *
    CreateAnimTransform(const void *animResBuf, nw4r::lyt::ResourceAccessor *pResAcsr) override;
};

class ResAccIfBase_c {
public:
    ~ResAccIfBase_c();
};

class ResAccIf_c : ResAccIfBase_c {
public:
    virtual ~ResAccIf_c();
    virtual void noop();
    virtual nw4r::lyt::ArcResourceAccessor *getAccessor();

    bool attach(void *data, const char *name);
    void detach();

protected:
    nw4r::lyt::ArcResourceAccessor mAccessor;
};

#define FLAG_NO_LOOP 1
#define FLAG_BACKWARDS 2

class FrameCtrl_c {
public:
    FrameCtrl_c() : mEndFrame(0.0f), mCurrFrame(0.0f), mPrevFrame(0.0f), mRate(1.0f) {}
    virtual ~FrameCtrl_c();

    void play();
    void set(f32 endFrame, u8 flags, f32 rate, f32 currFrame);
    void setFrame(f32 frame);
    void setRate(f32 rate);

    bool isStop() const;
    bool isStop2() const;
    f32 getStartFrame() const;
    f32 getEndFrame() const;

    inline f32 getFrame() const {
        return mCurrFrame;
    }

    inline void setCurrFrame(f32 arg) {
        mCurrFrame = arg;
    }

    inline void setFlags(u8 flags) {
        mFlags = flags;
    }

    inline u8 getFlags() const {
        return mFlags;
    }

    inline bool isEndReached() const {
        f32 actualEnd = mEndFrame - 1.0f;
        return mCurrFrame >= actualEnd;
    }

    inline void setToEnd() {
        setFrame(mEndFrame - 1.0f);
    }

    inline void setForward() {
        mFlags = mFlags & ~FLAG_BACKWARDS;
    }

    inline void setBackward() {
        mFlags = mFlags | FLAG_BACKWARDS;
    }

    inline void setRatio(f32 ratio) {
        f32 actualEnd = mEndFrame - 1.0f;
        setFrame(actualEnd * ratio);
    }

    inline f32 getRatio() const {
        f32 actualEnd = mEndFrame - 1.0f;
        return mCurrFrame / actualEnd;
    }

    inline void setBackwardsRatio(f32 ratio) {
        f32 actualEnd = mEndFrame - 1.0f;
        setFrame(actualEnd - (ratio * actualEnd));
    }

    inline f32 getNextFrame() const {
        f32 end = mEndFrame;
        f32 f = mCurrFrame + 1.0f;
        return f >= end ? 0.0f : f;
    }

    inline f32 getEndFrameRaw() const {
        return mEndFrame;
    }

private:
    inline bool notLooping() const {
        return (mFlags & FLAG_NO_LOOP) != 0;
    }

    inline bool isBackwards() const {
        return (mFlags & FLAG_BACKWARDS) != 0;
    }

    /* 0x04 */ f32 mEndFrame;
    /* 0x08 */ f32 mCurrFrame;
    /* 0x0C */ f32 mPrevFrame;
    /* 0x10 */ f32 mRate;
    /* 0x14 */ u8 mFlags;
};

class Anm_c {
public:
    Anm_c() : mpTransform(nullptr), mFlags(0) {}
    virtual ~Anm_c();

    bool doSomething(const char *path, ResAccIf_c *acc);
    void setTransform(nw4r::lyt::AnimTransform *transform);
    void destroySomething();
    void set(f32 endFrame, u8 flags, f32 rate, f32 currFrame);
    void setFrame(f32 frame);
    void play();

    inline nw4r::lyt::AnimTransform *getTransform() {
        return mpTransform;
    }

    inline void setBound() {
        mFlags |= 1;
    }

    inline void setUnbound() {
        mFlags &= ~1;
    }

private:
    void updateFrame();
    nw4r::lyt::AnimTransform *mpTransform;
    FrameCtrl_c mFrameCtrl;
    u8 mFlags;
};

class Base_c {
public:
    Base_c() : mPriority(0x80) {}
    Base_c(u8 priority) : mPriority(priority) {}
    /* 0x00 */ nw4r::ut::Node mLink;

    /* vt offset 0x04 */
    /* vt 0x08 */ virtual ~Base_c();
    /* vt 0x0C */ virtual void draw();

    void setPriority(u8 p) {
        mPriority = p;
    }

    /* 0x0C */ u8 mPriority;

    void addToDrawList();
};

class Simple_c : public Base_c {
public:
    Simple_c();
    /* vt 0x08 */ virtual ~Simple_c();
    /* vt 0x0C */ virtual void draw() override;
    /* vt 0x10 */ virtual void calc();
    /* vt 0x14 */ virtual bool build(const char *name, ResAccIf_c *acc);

    void calcBefore();
    void calcAfter();

    void bind(Anm_c *anm);
    void unbind(Anm_c *anm);

    bool flagSet() {
        return (field_0x94 & 1) ? 1 : 0;
    }

    void setResAcc(ResAccIf_c *resAcc) {
        mpResAcc = resAcc;
    }

private:
    Layout_c mLayout;
    nw4r::lyt::DrawInfo mDrawInfo;
    ResAccIf_c *mpResAcc;
    mVec3_c mVec;
    u32 field_0x94;
};


void getAllocator();

} // namespace m2d

#endif
