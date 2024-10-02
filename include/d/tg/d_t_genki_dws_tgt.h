#ifndef D_T_GENKI_DWS_TGT_H
#define D_T_GENKI_DWS_TGT_H

#include <d/tg/d_tg.h>
#include <toBeSorted/dowsing_target.h>

class dTgGenkiDwsTgt_c : public dTg_c {
public:
    dTgGenkiDwsTgt_c() : mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}
    virtual ~dTgGenkiDwsTgt_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;


private:
    u16 getSceneFlagFromParams();

    DowsingTarget mDowsingTarget; // 0xFC
};

#endif
