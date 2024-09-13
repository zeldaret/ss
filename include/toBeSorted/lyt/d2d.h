#ifndef D2D_H
#define D2D_H

#include <m/m2d.h>

namespace d2d
{

class ResAccIf_c : public m2d::ResAccIf_c {
public:
    /* 0xB4 */ nw4r::lyt::FontRefLink mFontRefLinks[5];

    void fn_800A9D30();
    void fn_800A9D90(void *data, const char *name);
};

struct LytBrlanMapping {
    const char *mFile;
    const char *mName;
};

class Layout_c : public nw4r::lyt::Layout {
public:
    virtual ~Layout_c() {}

    virtual bool Build(const void *lytResBuf, nw4r::lyt::ResourceAccessor *pResAcsr) override;
    virtual nw4r::lyt::AnimTransform *CreateAnimTransform(const void *animResBuf,
            nw4r::lyt::ResourceAccessor *pResAcsr) override;
};

class Multi_c : public m2d::Base_c {
public:
    Multi_c();
    virtual ~Multi_c() {}
    virtual void draw() override;
    virtual void calc();
    virtual bool fn_Multi_c_0x14();
    virtual bool build(const char *name, m2d::ResAccIf_c *acc);

    void calcBefore();
    void calcAfter();

    Layout_c *getLayout() {
        return &mLayout;
    }

    void setResAcc(m2d::ResAccIf_c *resAcc) {
        mpResAcc = resAcc;
    }

protected:
    Layout_c mLayout;
    nw4r::lyt::DrawInfo mDrawInfo;
    m2d::ResAccIf_c *mpResAcc;
    u32 field_0x88;
};

class LytBase_c : public Multi_c {
public:
    LytBase_c();
    virtual ~LytBase_c();
    virtual void draw() override {
        mLayout.Draw(mDrawInfo);
    };

    LytBase_c *fn_800AAAD0(const char *name);
    void fn_800AAB70();

private:
    void *mpMsbtInfo;
};

struct dLytStructA {
    dLytStructA() {}
    ~dLytStructA() {}

    void init(const char *fileName, m2d::ResAccIf_c* acc, d2d::Layout_c *layout, const char *animName);

    void fn_800AC6D0(bool);
    void fn_800AC7D0();
    void fn_800AC860();
    void fn_800AC870(bool);
    void fn_800AC910();

    inline void setFrame(f32 frame) {
        fn_800AC6D0(false);
        fn_800AC870(true);
        mpFrameCtrl->setFrame(frame);
        fn_800AC910();
    }

    u8 field_0x00[0x08 - 0x00];

    /* 0x08 */ m2d::FrameCtrl_c *mpFrameCtrl;

    u8 field_0x0C[0x40 - 0x0C];
};

struct dLytStructB {
    dLytStructB();
    ~dLytStructB();

    void init(void *, u8);

    u8 field_0x00[0x808 - 0x00];
};

struct dLytStructC {
    dLytStructC();
    ~dLytStructC();

    u8 field_0x00[0x10 - 0x00];
};

struct dLytStructD_Base {
    dLytStructD_Base()
        : field_0x04(0), field_0x08(0), field_0x0C(0), field_0x10(0), field_0x14(0), field_0x015(0), field_0x016(0) {}
    virtual ~dLytStructD_Base();
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u16 field_0x14;
    u8 field_0x015;
    u8 field_0x016;
};

struct dLytStructD : dLytStructD_Base {
    dLytStructD() : field_0x18(0), field_0x1C(0), field_0x20(0), field_0x22(0), field_0x23(0), field_0x24(0) {}

    virtual ~dLytStructD();

    void init(nw4r::lyt::Pane*, u16, u8, u8);
    void append(dLytStructD *other);
    void detach(dLytStructD *other);
    void fn_80065E70(nw4r::lyt::Pane*, s32, s32, s32);
    void fn_80065F70();

    u32 field_0x18;
    u32 field_0x1C;
    u16 field_0x20;
    u8 field_0x22;
    u8 field_0x23;
    u32 field_0x24;
};

} // namespace d2d

#endif
