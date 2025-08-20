#ifndef D_SC_TITLE_H
#define D_SC_TITLE_H

#include "d/d_sc_game.h"
#include "s/s_FPhase.h"
#include "s/s_State.hpp"

class dScTitle_c : public dScGame_c {
public:
    dScTitle_c();
    virtual ~dScTitle_c();

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dScTitle_c, dScGame_c, Stanby);
    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dScTitle_c, dScGame_c, Action);

    static dScTitle_c *sInstance;

    static void setLbl_80575787(bool v) {
        lbl_80575787 = v;
    }

    static u8 lbl_80575785;
    static u8 lbl_80575786;
    static u8 lbl_80575787;

    static void loadTitleScreen(u32 params);
    static void setSomethingSkbRelated();

private:
    static bool sPreventEmptyFilesTitle;
    static bool checkAllSaveFilesEmpty();

    virtual int create() override;
    virtual int execute() override;
    virtual int doDelete() override;
    virtual int draw() override;

    static void *dvdCallback(void *data);

    static sFPhase<dScTitle_c>::phaseCallback sCallbacks[];

    /* 0x2A4 */ mDvd_toMainRam_normal_c *mpSkbArc;
    /* 0x2A8 */ mDvd_toMainRam_normal_c *mpSkbFont;
    /* 0x2AC */ u8 field_0x2AC;
    /* 0x2AD */ u8 field_0x2AD;
};

#endif
