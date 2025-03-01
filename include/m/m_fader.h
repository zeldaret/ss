#ifndef M_FADER_H
#define M_FADER_H

#include "m/m_fader_base.h"

class mFader_c {
public:
    void draw();
    bool setFader(mFaderBase_c *fader);

    bool isStatus(mFaderBase_c::EStatus status) {
        return mpFader->getStatus() == status;
    }

    bool isNotStatus(mFaderBase_c::EStatus status) {
        return isStatus(status) != true;
    }

    void setStatus(mFaderBase_c::EStatus status) {
        mpFader->setStatus(status);
    }

    bool calc() {
        return mpFader->calc();
    }
protected:
    mFaderBase_c *mpFader;
};

#endif
