#ifndef EGG_AUDIO_SYSTEM_H
#define EGG_AUDIO_SYSTEM_H

#include <common.h>

namespace EGG {

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    void fn_804B7270(s32 frame);
    void fn_804B7370();
    void fn_804B73D0(s32 frame);
    void calc();

    bool isField0x04Eq2() {
        return field_0x04 == 2;
    }

    bool field0x08NotZero() {
        return field_0x08 != 0;
    }

    bool field0x04NotZero() {
        return field_0x04 != 0;
    }

    static AudioSystem *sInstanse; ///< sic

private:
    f32 field_0x00;
    s32 field_0x04;
    s32 field_0x08;
};

} // namespace EGG

#endif
