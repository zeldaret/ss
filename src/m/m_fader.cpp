#include <m/m_fader.h>
#include <m/m3d/m3d.h>
#include <nw4r/g3d/g3d_state.h>

void mFader_c::draw() {
    nw4r::g3d::G3DState::Invalidate(0x7ff);
    m3d::resetMaterial();
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
