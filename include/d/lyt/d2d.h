#ifndef D2D_H
#define D2D_H

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
    /* vt 0x0C */ virtual void draw() override;
    /* vt 0x10 */ virtual void animate();
    /* vt 0x18 */ virtual void calc();
    /* vt 0x18 */ virtual bool build(const char *name, m2d::ResAccIf_c *acc);

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
    /* vt 0x0C */ virtual void draw() override {
        mLayout.Draw(mDrawInfo);
    };

    /* vt 0x18 */ virtual bool build(const char *name, m2d::ResAccIf_c *acc) override;
    dTextBox_c *getTextBox(const char *name);
    dTextBox_c *getSizeBoxInWindow(const char *windowName);
    dWindow_c *getWindow(const char *name);
    nw4r::lyt::Group *findGroupByName(const char *name);

    bool fn_800AB940(const char *name, int arg);
    bool fn_800AB9A0(dTextBox_c *textbox, int arg);

    bool fn_800ABE50(dTextBox_c *textbox, wchar_t *destBuf, u32 maxLen);

private:
    void setPropertiesRecursive(nw4r::lyt::Pane *pane, f32, f32, f32, f32, f32);
    void setProperties(nw4r::lyt::Pane *pane, f32, f32, f32, f32, f32);
    dTextBox_c *getTextBoxViaUserData(nw4r::lyt::Pane *pane, const char *name);
    bool fn_800ABB80(dTextBox_c *textbox1, dTextBox_c *textbox2, int arg);
    bool fn_800ABCE0(const nw4r::lyt::res::ExtUserData *userDatum, dTextBox_c *textbox1, dTextBox_c *textbox2, int arg);

    bool fn_800AC040(dTextBox_c *textbox1, dTextBox_c *textbox2, wchar_t *destBuf, u32 maxLen);
    bool fn_800AC1AC(
        const nw4r::lyt::res::ExtUserData *userDatum, dTextBox_c *textbox1, dTextBox_c *textbox2, wchar_t *destBuf, u32 maxLen
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

class dSubPane;

struct SubPaneListNode {
    /* 0x00 */ nw4r::ut::LinkListNode mNode;
    /** The d lyt pane, set by the UI element */
    /* 0x08 */ dSubPane *mpLytPane;
    /** The nw4r lyt pane, set by linkMeters */
    /* 0x0C */ nw4r::lyt::Pane *mpPane;
};

typedef nw4r::ut::LinkList<SubPaneListNode, offsetof(SubPaneListNode, mNode)> SubPaneList;

class dSubPane {
public:
    dSubPane() : field_0x04(false), field_0x05(0) {}
    /* vt 0x08 */ virtual ~dSubPane() {}
    /* vt 0x0C */ virtual bool build(ResAccIf_c *resAcc) = 0;
    /* vt 0x10 */ virtual bool remove() = 0;
    /* vt 0x14 */ virtual bool execute() = 0;
    /* vt 0x18 */ virtual nw4r::lyt::Pane *getPane() = 0;
    /* vt 0x1C */ virtual LytBase_c *getLyt() = 0;
    /* vt 0x20 */ virtual const char *getName() const = 0;
    /* vt 0x24 */ virtual bool LytMeter0x24() const {
        return field_0x04;
    }
    /* vt 0x28 */ virtual void LytMeter0x28(bool arg) {
        field_0x04 = arg;
    }
    /* vt 0x2C */ virtual u8 LytMeter0x2C() const {
        return field_0x05;
    }

    /* vt 0x30 */ virtual void LytMeter0x30(u8 arg) {
        field_0x05 = arg;
    }

    static void linkMeters(nw4r::lyt::Group *group, SubPaneList *meterGroup);

    bool field_0x04;
    u8 field_0x05;
};

#define ANMGROUP_FLAG_BOUND 1
#define ANMGROUP_FLAG_ENABLE 2

struct AnmGroupBase_c {
    AnmGroupBase_c(m2d::FrameCtrl_c *frameCtrl) : field_0x04(nullptr), mFlags(0), mpFrameCtrl(frameCtrl) {}
    virtual ~AnmGroupBase_c() {}

    bool init(const char *fileName, m2d::ResAccIf_c *acc, d2d::Layout_c *layout, const char *animName);
    bool init(nw4r::lyt::AnimTransform *transform, const char *fileName, m2d::ResAccIf_c *acc, nw4r::lyt::Group *group);

    bool bind(bool bDisable);
    bool unbind();
    bool remove();
    void setAnimEnable(bool);
    void setAnmFrame(f32);
    void syncAnmFrame();
    void setForward();
    void setBackward();

    inline void play() {
        mpFrameCtrl->play();
        syncAnmFrame();
    }

    inline void setFrame(f32 frame) {
        mpFrameCtrl->setFrame(frame);
        syncAnmFrame();
    }

    inline void setFrameRatio(f32 frame) {
        f32 end = getEndFrameRaw() - 1.0f;
        setFrame(end * frame);
    }

    inline f32 getEndFrameRaw() const {
        return mpFrameCtrl->getEndFrameRaw();
    }

    inline f32 getFrame() const {
        return mpFrameCtrl->getFrame();
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
    }

    inline bool isEnabled() const {
        return (mFlags & ANMGROUP_FLAG_ENABLE) != 0;
    }

    inline bool isStop() const {
        return mpFrameCtrl->isStop();
    }

    inline bool isStop2() const {
        return mpFrameCtrl->isStop2();
    }

    inline void setBackwardsOnce() {
        mpFrameCtrl->setFlags(FLAG_NO_LOOP | FLAG_BACKWARDS);
    }

    inline bool isPlayingBackwardsOnce() const {
        return mpFrameCtrl->getFlags() == (FLAG_NO_LOOP | FLAG_BACKWARDS);
    }

    inline bool isPlayingForwardsOnce() const  {
        return mpFrameCtrl->getFlags() == FLAG_NO_LOOP;
    }

    inline void setForwardOnce() {
        mpFrameCtrl->setFlags(FLAG_NO_LOOP);
    }

    inline void setToStart() {
        m2d::FrameCtrl_c &ctrl = *mpFrameCtrl;
        ctrl.setCurrFrame(ctrl.getStartFrame());
        syncAnmFrame();
    }

    inline void setToEnd2() {
        m2d::FrameCtrl_c &ctrl = *mpFrameCtrl;
        ctrl.setCurrFrame(ctrl.getEndFrame());
        syncAnmFrame();
    }

    inline void setRatio(f32 ratio) {
        mpFrameCtrl->setRatio(ratio);
        syncAnmFrame();
    }

    inline f32 getRatio() const {
        return mpFrameCtrl->getRatio();
    }

    inline void setBackwardsRatio(f32 ratio) {
        mpFrameCtrl->setBackwardsRatio(ratio);
        syncAnmFrame();
    }

    inline f32 getNextFrame() const {
        return mpFrameCtrl->getNextFrame();
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

void pushToEnd(nw4r::lyt::Pane *);

// This abstraction is apparently only ever used in CsGame
class AnmGroups {
public:
    AnmGroups(d2d::AnmGroup_c *g, u32 num) : mpAnmGroups(g), mNumAnmGroups(num) {}
    virtual ~AnmGroups() {}

    bool init(
        d2d::AnmGroup_c *pGroups, const LytBrlanMapping *mMappings, u32 num, m2d::ResAccIf_c *acc, d2d::Layout_c *layout
    );
    void remove();

    d2d::AnmGroup_c &operator[](int idx) {
        return mpAnmGroups[idx];
    }

    const d2d::AnmGroup_c &operator[](int idx) const {
        return mpAnmGroups[idx];
    }

private:
    /* 0x04 */ d2d::AnmGroup_c *mpAnmGroups;
    /* 0x08 */ u32 mNumAnmGroups;
};

} // namespace d2d

#endif
