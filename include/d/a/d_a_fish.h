#ifndef D_A_FISH_H
#define D_A_FISH_H

#include "d/a/d_a_fish_base.h"

class dAcFish_c : public dAcFishBase_c {
public:
    dAcFish_c() {}
    virtual ~dAcFish_c() {}

    virtual const char *getResFileName() override;
    virtual const char *getMdlName() override;
    virtual const char *getAnmName() override;
    virtual UNKWORD vt_0xB0() override;
    virtual f32 vt_0xB8() override;
    virtual f32 vt_0xBC() override;
    virtual void vt_0xC4() override;
    virtual void vt_0xC8() override;

private:
};

#endif
