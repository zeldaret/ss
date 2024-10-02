#ifndef D_LYT_FADER_H
#define D_LYT_FADER_H

#include <d/lyt/d2d.h>
#include <m/m_fader_base.h>

class dLytFader_c : public mFaderBase_c {
public:
    dLytFader_c(const mColor &color, EStatus status);
    virtual ~dLytFader_c();

    virtual void setStatus(EStatus status) override;
    virtual bool fadeIn() override;
    virtual bool fadeOut() override;
    virtual bool calc() override;
    virtual void draw() override;

    bool init();

private:
    void fn_801758F0();
    void fn_801759B0();
    void fn_80175A50();
    void fn_80175B10();
    void fn_80175BC0(s32);

    /* 0x014 */ d2d::ResAccIf_c mResAcc;
    /* 0x384 */ d2d::LytBase_c mLytBase;
    /* 0x414 */ d2d::AnmGroup_c mLytStructAs[3];
    /* 0x4D4 */ s32 field_0x4D4;
    /* 0x4D8 */ s32 field_0x4D8;
    /* 0x4DC */ u8 field_0x4DC[0x4DD - 0x4DC];

    bool field_0x4DD;
};

#endif
