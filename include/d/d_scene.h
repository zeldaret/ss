#ifndef D_SCENE_H
#define D_SCENE_H

#include "d/d_base.h"
#include "d/d_fader.h"
#include "f/f_profile_name.h"

class dScene_c : public dBase_c {
public:
    dScene_c();
    virtual ~dScene_c() {}

    virtual void postCreate(MAIN_STATE_e state) override;
    virtual void postDelete(MAIN_STATE_e state) override;
    virtual int preExecute() override;
    virtual void postExecute(MAIN_STATE_e status) override;

    static void staticCreate();
    static dBase_c *staticExecute();
    static void setRootActor(fProfile::PROFILE_NAME_e rootActor, u32 params, s32 fadeInType, s32 fadeOutType);

protected:
    static dFader_c sFader;
};

#endif
