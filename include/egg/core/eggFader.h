#ifndef EGG_FADER_H
#define EGG_FADER_H

#include <common.h>

namespace EGG {

class Fader {
public:
    enum EStatus {
        STATUS_PREPARE_IN,
        STATUS_PREPARE_OUT,
        STATUS_FADE_IN,
        STATUS_FADE_OUT,
    };

public:
    // vtable at 0x00
    /* vt 0x08 | 00000000 */ virtual void setStatus(EStatus) = 0;
    /* vt 0x0C | 00000000 */ virtual EStatus getStatus() const = 0;
    /* vt 0x10 | 00000000 */ virtual bool fadeIn() = 0;
    /* vt 0x14 | 00000000 */ virtual bool fadeOut() = 0;
    /* vt 0x18 | 00000000 */ virtual bool calc() = 0;
    /* vt 0x1C | 00000000 */ virtual bool draw() = 0;
};

} // namespace EGG

#endif
