#include <m/m_fader.h>

// nw4r::g3d::G3DState::Invalidate
extern "C" void fn_8044E3B0(u32 flags);
// m3d::resetMaterial according to NSMBW
extern "C" void fn_802E46D0();

void mFader_c::draw() {
    fn_8044E3B0(0x7ff);
    fn_802E46D0();
    mpFader->draw();
}

bool mFader_c::setFader(mFaderBase_c *fader) {
    if (mpFader != nullptr) {
        bool isFading = isStatus(mFaderBase_c::FADING_IN) || isStatus(mFaderBase_c::FADING_OUT);

        if (isFading) {
            return false;
        }
    }

    mFaderBase_c::EStatus status = mpFader != nullptr ? mpFader->getStatus() : fader->getStatus();

    mpFader = fader;
    switch (status) {
    case mFaderBase_c::FADED_OUT:
        fader->setStatus(mFaderBase_c::FADED_OUT);
        break;
    case mFaderBase_c::FADED_IN:
        fader->setStatus(mFaderBase_c::FADED_IN);
        break;
    }

    return true;
}
