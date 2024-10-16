#include "m/m2d.h"

#include "egg/core/eggExpHeap.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggScreen.h"
#include "nw4r/lyt/lyt_init.h"
#include "rvl/GX.h" // IWYU pragma: export
#include "rvl/SC.h" // IWYU pragma: export

namespace m2d {

nw4r::ut::List l_list;
mAllocator_c *l_allocator;

void reset() {
    nw4r::ut::List_Init(&l_list, 0);
}

bool init(mAllocator_c *allocator) {
    nw4r::lyt::LytInit();
    l_allocator = allocator;
    nw4r::lyt::Layout::mspAllocator = allocator;
    reset();
    return true;
}

void create(EGG::Heap *heap, u32 size) {
    if (size != 0) {
        heap = EGG::ExpHeap::create(size, heap, 0);
        // Supposed to be 2D表示用ヒープ(m2d::create), but creates a linux compile issue
        heap->setName("2D\x95\x5C示用ヒープ(m2d::create)");
    }
    mAllocator_c *allocator = new (heap, 0x04) mAllocator_c();
    allocator->attach(heap, 4);
    init(allocator);
}

// TODO This is literally the first sbss2 symbol.
extern "C" GXColor lbl_8057FFA0;

void defaultSet() {
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(false, GX_ALWAYS, false);
    GXSetFog(GX_FOG_NONE, lbl_8057FFA0, 0.0f, 0.0f, 0.0f, 0.0f);
}

void draw(u8 priority) {
    for (Base_c *base = static_cast<Base_c *>(nw4r::ut::List_GetFirst(&l_list)); base != nullptr;
         base = static_cast<Base_c *>(nw4r::ut::List_GetNext(&l_list, base))) {
        if (base->mPriority >= priority) {
            break;
        }
        base->draw();
    }
}

void drawBefore(u8 priority) {
    for (Base_c *base = static_cast<Base_c *>(nw4r::ut::List_GetFirst(&l_list)); base != nullptr;
         base = static_cast<Base_c *>(nw4r::ut::List_GetNext(&l_list, base))) {
        if (base->mPriority > priority) {
            base->draw();
        }
    }
}

// Despite the name this *updates* Lyt's allocator
void getAllocator() {
    nw4r::lyt::Layout::mspAllocator = l_allocator;
}

nw4r::lyt::AnimTransform *Layout_c::CreateAnimTransform(const void *animResBuf, nw4r::lyt::ResourceAccessor *pResAcsr) {
    nw4r::lyt::AnimTransform *result = nw4r::lyt::Layout::CreateAnimTransform(animResBuf, pResAcsr);
    if (result != nullptr) {
        GetAnimTransformList().Erase(result);
    }
    return result;
}

ResAccIfBase_c::~ResAccIfBase_c() {}
ResAccIf_c::~ResAccIf_c() {}

nw4r::lyt::ArcResourceAccessor *ResAccIf_c::getAccessor() {
    return &mAccessor;
}

void ResAccIf_c::noop() {}

bool ResAccIf_c::attach(void *data, const char *name) {
    return mAccessor.Attach(data, name);
}

void ResAccIf_c::detach() {
    mAccessor.Detach();
}

FrameCtrl_c::~FrameCtrl_c() {}

void FrameCtrl_c::play() {
    mPrevFrame = mCurrFrame;
    f32 newFrame;
    if (isBackwards()) {
        if (mCurrFrame >= mRate) {
            newFrame = mCurrFrame - mRate;
        } else {
            if (notLooping()) {
                newFrame = 0.0f;
            } else {
                newFrame = mCurrFrame + (mEndFrame - mRate);
            }
        }
    } else {
        newFrame = mCurrFrame + mRate;
        if (notLooping()) {
            f32 forwardsEnd = mEndFrame - 1.0f;
            if (newFrame > forwardsEnd) {
                newFrame = forwardsEnd;
            }
        } else {
            if (newFrame >= mEndFrame) {
                newFrame = newFrame - mEndFrame;
            }
        }
    }

    mCurrFrame = newFrame;
}

void FrameCtrl_c::set(f32 endFrame, u8 flags, f32 rate, f32 currFrame) {
    if (currFrame < 0.0f) {
        currFrame = 0.0f;
    }
    mEndFrame = endFrame;
    mCurrFrame = currFrame;
    setRate(rate);
    mFlags = flags;
    mPrevFrame = mCurrFrame;
}

void FrameCtrl_c::setFrame(f32 frame) {
    mCurrFrame = frame;
    mPrevFrame = frame;
}

void FrameCtrl_c::setRate(f32 rate) {
    mRate = rate;
}

bool FrameCtrl_c::isStop() const {
    f32 actualEnd;
    switch (mFlags) {
        case FLAG_NO_LOOP:                  actualEnd = mEndFrame - 1.0f; return mCurrFrame >= actualEnd;
        case FLAG_NO_LOOP | FLAG_BACKWARDS: return mCurrFrame <= 0;
        default:                            return false;
    }
}

f32 FrameCtrl_c::getEndFrame() const {
    if (isBackwards()) {
        if (notLooping()) {
            return mEndFrame - 1.0f;
        }
        return mEndFrame;
    }

    return 0.0f;
}

f32 FrameCtrl_c::getEndFrame2() const {
    if (isBackwards()) {
        return 0.0f;
    }
    if (notLooping()) {
        return mEndFrame - 1.0f;
    }
    return mEndFrame;
}

bool FrameCtrl_c::isStop2() const {
    if (isBackwards()) {
        return mCurrFrame <= 0.0f;
    }
    if (notLooping()) {
        f32 actualEnd = mEndFrame - 1.0f;
        return mCurrFrame >= actualEnd;
    }
    return mCurrFrame >= mEndFrame;
}

Anm_c::~Anm_c() {
    destroySomething();
}

bool Anm_c::doSomething(const char *path, ResAccIf_c *acc) {
    if (mpTransform != nullptr) {
        destroySomething();
    }

    static Layout_c l_layout;

    void *resource = acc->getAccessor()->GetResource(0, path, nullptr);
    nw4r::lyt::AnimTransform *transform = l_layout.CreateAnimTransform(resource, acc->getAccessor());
    if (transform == nullptr) {
        return false;
    }

    setTransform(transform);
    return true;
}

void Anm_c::setTransform(nw4r::lyt::AnimTransform *transform) {
    mpTransform = transform;
    u8 flags = 1;
    if (mpTransform->IsLoopData()) {
        flags = 0;
    }
    set(mpTransform->GetFrameSize(), flags, 1.0f, -1.0);
}

void Anm_c::destroySomething() {
    if (mpTransform != nullptr) {
        mpTransform->~AnimTransform();
        nw4r::lyt::Layout::FreeMemory(mpTransform);
        mpTransform = nullptr;
    }
}

void Anm_c::set(f32 endFrame, u8 flags, f32 rate, f32 currFrame) {
    mFrameCtrl.set(endFrame, flags, rate, currFrame);
    updateFrame();
}

void Anm_c::setFrame(f32 frame) {
    mFrameCtrl.setFrame(frame);
    updateFrame();
}

void Anm_c::updateFrame() {
    mpTransform->SetFrame(mFrameCtrl.getFrame());
}

void Anm_c::play() {
    mFrameCtrl.play();
    updateFrame();
}

Base_c::~Base_c() {}
void Base_c::draw() {}

void Base_c::addToDrawList() {
    Base_c *target = static_cast<Base_c *>(nw4r::ut::List_GetFirst(&l_list));
    while (target != nullptr) {
        if (target->mPriority > mPriority) {
            nw4r::ut::List_Insert(&l_list, target, this);
            return;
        }

        target = static_cast<Base_c *>(nw4r::ut::List_GetNext(&l_list, target));
    }
    nw4r::ut::List_Append(&l_list, this);
}

Simple_c::~Simple_c() {}
Simple_c::Simple_c() : Base_c(0x80), mLayout(), mDrawInfo(), mpResAcc(nullptr), mVec(0.0f, 0.0f, 0.0f), field_0x94(0) {
    if (SCGetAspectRatio() == 1) {
        mDrawInfo.SetLocationAdjustScale(nw4r::math::VEC2(EGG::Screen::GetAdjustScale(), 1.0f));
        mDrawInfo.SetLocationAdjust(true);
    }
}

void Simple_c::calc() {
    calcBefore();
    nw4r::math::MTX34 mtx;
    PSMTXIdentity(mtx.m);
    // Probably inlines
    mVec3_c v = mVec;
    PSMTXTransApply(mtx.m, mtx.m, v.x, v.y, v.z);
    mDrawInfo.SetViewMtx(mtx);
    calcAfter();
}

void Simple_c::calcBefore() {
    u32 flag = 0;
    if ((field_0x94 & 1) != 0) {
        flag = 1;
    }
    mLayout.Animate(flag);
}

void Simple_c::calcAfter() {
    mDrawInfo.SetViewRect(mLayout.GetLayoutRect());
    mLayout.CalculateMtx(mDrawInfo);
}

void Simple_c::draw() {
    nw4r::ut::Rect r = mLayout.GetLayoutRect();
    f32 near = 0.0f;
    f32 far = 500.0f;
    EGG::Screen s;
    bool needsAdjust = EGG::Screen::GetTVMode() == EGG::Screen::TV_MODE_2;
    f32 f1 = EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_2);
    f32 f2 = EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_1);

    f32 left = needsAdjust ? f1 * r.left / f2 : r.left;
    f32 right = needsAdjust ? f1 * r.right / f2 : r.right;
    s.SetProjectionType(EGG::Frustum::PROJ_ORTHO);
    s.ResetOrthographic(r.top, r.bottom, left, right, near, far);
    if (needsAdjust) {
        s.SetScale(nw4r::math::VEC3(f2 / f1, 1.0f, 1.0f));
    }
    s.SetProjectionGX();
    mLayout.Draw(mDrawInfo);
}

bool Simple_c::build(const char *name, ResAccIf_c *acc) {
    if (mLayout.GetRootPane() != nullptr) {
        return true;
    }
    if (acc == nullptr) {
        acc = mpResAcc;
        if (acc == nullptr) {
            return false;
        }
    }

    void *resource = acc->getAccessor()->GetResource(0, name, nullptr);
    if (resource == nullptr) {
        return false;
    } else {
        bool success = mLayout.Build(resource, acc->getAccessor());
        if (success) {
            calc();
        }
        return success;
    }
}

void Simple_c::bind(Anm_c *anm) {
    anm->setBound();
    mLayout.BindAnimation(anm->getTransform());
}

void Simple_c::unbind(Anm_c *anm) {
    anm->setUnbound();
    mLayout.UnbindAnimation(anm->getTransform());
}

} // namespace m2d
