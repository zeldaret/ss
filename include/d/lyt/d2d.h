#ifndef D2D_H
#define D2D_H

#include "d/lyt/meter/d_lyt_meter_base.h"
#include "libms/msgfile.h"
#include "m/m2d.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_picture.h"


class dTextBox_c;
class dWindow_c;

namespace d2d {

class ResAccIf_c : public m2d::ResAccIf_c {
public:
    ResAccIf_c() {}
    virtual ~ResAccIf_c() {}
    /* 0xB4 */ nw4r::lyt::FontRefLink mFontRefLinks[5];

    bool attach(void *data, const char *name);
    void detach();
};

struct LytBrlanMapping {
    const char *mFile;
    const char *mName;
};

class Layout_c : public nw4r::lyt::Layout {
public:
    virtual ~Layout_c() {}
    virtual bool Build(const void *lytResBuf, nw4r::lyt::ResourceAccessor *pResAcsr) override;
    virtual nw4r::lyt::AnimTransform *
    CreateAnimTransform(const void *animResBuf, nw4r::lyt::ResourceAccessor *pResAcsr) override;

    // Bring the overloaded, otherwise shadowed functions in scope
    using nw4r::lyt::Layout::CreateAnimTransform;

private:
    nw4r::lyt::Pane *BuildPaneObj(s32 kind, const void *dataPtr, const nw4r::lyt::ResBlockSet &resBlockSet);
};

class Multi_c : public m2d::Base_c {
public:
    Multi_c();
    virtual ~Multi_c() {}
    virtual void draw() override;
    virtual void animate();
    virtual void calc();
    virtual bool build(const char *name, m2d::ResAccIf_c *acc);

    void calcBefore();
    void calcAfter();
    nw4r::lyt::Pane *findPane(const char *name) const;
    nw4r::lyt::Picture *findPicture(const char *name) const;
    nw4r::lyt::Bounding *findBounding(const char *name) const;
    void unbindAnims();

    const Layout_c *getLayout() const {
        return &mLayout;
    }

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
    u32 mFlags;
};

class LytBase_c : public Multi_c {
public:
    LytBase_c();
    virtual ~LytBase_c();
    virtual void draw() override {
        mLayout.Draw(mDrawInfo);
    };

    virtual bool build(const char *name, m2d::ResAccIf_c *acc) override;
    dTextBox_c *getTextBox(const char *name);
    dTextBox_c *getSizeBoxInWindow(const char *windowName);
    dWindow_c *getWindow(const char *name);
    nw4r::lyt::Group *findGroupByName(const char *name);

    bool fn_800AB940(const char *name, int arg);
    bool fn_800AB9A0(dTextBox_c *textbox, int arg);

    bool fn_800ABE50(dTextBox_c *textbox, int arg, void *unk);

    static void linkMeters(nw4r::lyt::Group *group, LytMeterGroup *meterGroup);

private:
    void setPropertiesRecursive(nw4r::lyt::Pane *pane, f32, f32, f32, f32, f32);
    void setProperties(nw4r::lyt::Pane *pane, f32, f32, f32, f32, f32);
    dTextBox_c *getTextBoxViaUserData(nw4r::lyt::Pane *pane, const char *name);
    bool fn_800ABB80(dTextBox_c *textbox1, dTextBox_c *textbox2, int arg);
    bool fn_800ABCE0(const nw4r::lyt::res::ExtUserData *userDatum, dTextBox_c *textbox1, dTextBox_c *textbox2, int arg);

    bool fn_800AC040(dTextBox_c *textbox1, dTextBox_c *textbox2, int arg, void *unk);
    bool fn_800AC1AC(
        const nw4r::lyt::res::ExtUserData *userDatum, dTextBox_c *textbox1, dTextBox_c *textbox2, int arg, void *unk
    );
    MsbtInfo *getMsbtInfo() const;
    bool fn_800AB930(dTextBox_c *box);

    /* 0x8C */ MsbtInfo *mpMsbtInfo;
};

class dLytSub : public d2d::LytBase_c {
public:
    dLytSub() {}
    virtual bool build(const char *name, m2d::ResAccIf_c *acc) override {
        mpName = name;
        return d2d::LytBase_c::build(name, acc);
    }

    const char *getName() const {
        return mpName;
    }
private:
    /// This name is stored by `build` and usually accessed by dLytMeters' getName functions
    const char *mpName;
};

struct AnmGroupBase_c {
    AnmGroupBase_c(m2d::FrameCtrl_c *frameCtrl) : field_0x04(nullptr), mFlags(0), mpFrameCtrl(frameCtrl) {}
    virtual ~AnmGroupBase_c() {}

    bool init(const char *fileName, m2d::ResAccIf_c *acc, d2d::Layout_c *layout, const char *animName);
    bool init(nw4r::lyt::AnimTransform *transform, const char *fileName, m2d::ResAccIf_c *acc, nw4r::lyt::Group *group);

    bool setDirection(bool backwards);
    bool unbind();
    bool afterUnbind();
    void setAnimEnable(bool);
    void setAnmFrame(f32);
    void syncAnmFrame();
    void setForward();
    void setBackward();

    inline void setFrameAndControlThings(f32 frame) {
        setDirection(false);
        setAnimEnable(true);
        mpFrameCtrl->setFrame(frame);
        syncAnmFrame();
    }

    inline void play() {
        mpFrameCtrl->play();
        syncAnmFrame();
    }

    inline void setFrame(f32 frame) {
        mpFrameCtrl->setFrame(frame);
        syncAnmFrame();
    }

    inline f32 getFrame() const {
        return mpFrameCtrl->getFrame();
    }

    inline void setToStart() {
        mpFrameCtrl->setToStart();
        syncAnmFrame();
    }

    inline void setToEnd() {
        mpFrameCtrl->setToEnd();
        syncAnmFrame();
    }

    inline bool isEndReached() const {
        return mpFrameCtrl->isEndReached();
    }

    inline void setRate(f32 rate) {
        mpFrameCtrl->setRate(rate);
        setAnimEnable(true);
    }

    inline bool isFlag2() const {
        return (mFlags & 2) != 0;
    }

    inline bool isStop2() const {
        return mpFrameCtrl->isStop2();
    }


    inline void playBackwardsOnce() {
        mpFrameCtrl->setFlags(FLAG_NO_LOOP | FLAG_BACKWARDS);
        setToEnd2();
    }

    inline void playLoop() {
        mpFrameCtrl->setFlags(FLAG_NO_LOOP);
        setToEnd2();
    }

    inline void setToEnd2() {
        m2d::FrameCtrl_c &ctrl = *mpFrameCtrl;
        ctrl.setCurrFrame(ctrl.getEndFrame());
        syncAnmFrame();
    }

private:
    /* 0x04 */ void *field_0x04;
    /* 0x08 */ m2d::FrameCtrl_c *mpFrameCtrl;
    /* 0x0C */ u8 mFlags;
    /* 0x10 */ nw4r::lyt::AnimResource mAnmResource;
    /* 0x20 */ nw4r::lyt::Group *mpGroup;
    /* 0x24 */ nw4r::lyt::AnimTransform *mpAnmTransform;
};

// Size: 0x40
struct AnmGroup_c : public AnmGroupBase_c {
    AnmGroup_c() : AnmGroupBase_c(&mFrameCtrl) {}
    virtual ~AnmGroup_c() {}
    /* 0x28 */ m2d::FrameCtrl_c mFrameCtrl;
};

} // namespace d2d

#endif
