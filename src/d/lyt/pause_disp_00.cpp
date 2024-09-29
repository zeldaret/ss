#include <d/lyt/pause_disp_00.h>
#include <nw4r/lyt/lyt_group.h>

STATE_DEFINE(dLytPauseDisp00_c, None);
STATE_DEFINE(dLytPauseDisp00_c, In);
STATE_DEFINE(dLytPauseDisp00_c, Wait);
STATE_DEFINE(dLytPauseDisp00_c, Select);
STATE_DEFINE(dLytPauseDisp00_c, Ring);
STATE_DEFINE(dLytPauseDisp00_c, GetDemo);
STATE_DEFINE(dLytPauseDisp00_c, Out);

extern d2d::LytBrlanMapping S_MAPPINGS[];
extern "C" char lbl_804E8898[];

extern const char *B_LIST[];

dLytPauseDisp00_c::dLytPauseDisp00_c() : mStateMgr(*this, sStateID::null) {}

extern "C" void fn_800AC3C0(nw4r::lyt::Group *, nw4r::ut::LinkList<d2d::dLytStructB, 0> *);
extern "C" d2d::dLytStructD *lbl_80575260;

void dLytPauseDisp00_c::init() {
    mLytBase.build("pause_00.brlyt", nullptr);
    mLytBase.mPriority = 0x86;

    for (int i = 0; i < 0x7E; i++) {
        field_0x00D0[i].init(S_MAPPINGS[i].mFile, nullptr, mLytBase.getLayout(), S_MAPPINGS[i].mName);
    }

    for (int i = 0; i < 0x18; i++) {
        field_0x2050[i].init(nullptr, lbl_804E8898[i]);
        mList.PushBack(&field_0x2050[i]);
    }

    if (mLytBase.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *group = mLytBase.getLayout()->GetGroupContainer()->FindGroupByName("G_ref_00");
        if (group != nullptr) {
            fn_800AC3C0(group, &mList);
        }
    }

    field_0xE29C.init(mLytBase.getLayout()->GetRootPane(), 1, 0, 0);
    lbl_80575260->append(&field_0xE29C);
    for (int i = 0; i < 0x25; i++) {
        if (i != 0x1F && i != 0x20) {
            nw4r::lyt::Bounding *b = mLytBase.findBounding(B_LIST[i]);
            // TODO
        }
    }
}

void dLytPauseDisp00_c::displayElement(int i, float frame) {
    d2d::AnmGroup_c *s = &field_0x00D0[i];
    s->setFrame(frame);
}

dLytPauseDisp00_c::~dLytPauseDisp00_c() {}
