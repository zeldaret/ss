#include "d/lyt/d_screen_fader.h"

#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggScreen.h"
#include "m/m_mtx.h"
#include "nw4r/ut/ut_algorithm.h"


dScreenFader_c::dScreenFader_c(const mColor &color, EStatus status) : mFaderBase_c(color, status), mProgress(0.0f) {}

dScreenFader_c::~dScreenFader_c() {}

void dScreenFader_c::setStatus(EStatus status) {
    if (status == FADED_OUT) {
        mStatus = FADED_OUT;
        mProgress = EGG::Screen::GetSizeYMax() * 0.5f;
    } else if (status == FADED_IN) {
        mStatus = FADED_IN;
        mProgress = 0.0f;
    }
}

bool dScreenFader_c::calc() {
    bool res = mFaderBase_c::calc();

    f32 h = EGG::Screen::GetSizeYMax() * 0.5f;
    u16 elapsed = mElapsed;
    if (elapsed > mFrame) {
        elapsed = mFrame;
    }

    switch (mStatus) {
        case FADING_IN:  mProgress = h - (h * nw4r::math::SinRad((float)elapsed * M_PI / ((float)(mFrame * 2)))); break;
        case FADING_OUT: mProgress = h - (h * nw4r::math::CosRad((float)elapsed * M_PI / ((float)(mFrame * 2)))); break;
    }

    return res;
}

extern "C" void fn_80016200();

void dScreenFader_c::draw() {
    fn_80016200();
    EGG::DrawGX::BeginDrawScreen(false, false, true);
    f32 scaleX = EGG::Screen::GetSizeXMax();
    f32 scaleY = EGG::Screen::GetSizeYMax();

    mMtx_c mtx;
    PSMTXScale(mtx, scaleX, mProgress, 1.0f);
    PSMTXTransApply(mtx, mtx, -scaleX * 0.5f, scaleY * 0.5f - mProgress, 0.0f);

    GXColor a = EGG::DrawGX::BLACK;
    EGG::DrawGX::DrawDL(EGG::DrawGX::DL_17, mtx, a);

    PSMTXScale(mtx, scaleX, mProgress, 1.0f);
    PSMTXTransApply(mtx, mtx, -scaleX * 0.5f, -scaleY * 0.5f, 0.0f);
    EGG::DrawGX::DrawDL(EGG::DrawGX::DL_17, mtx, a);
}

bool dScreenFader_c::fadeIn() {
    return mFaderBase_c::fadeIn();
}

bool dScreenFader_c::fadeOut() {
    return mFaderBase_c::fadeOut();
}
