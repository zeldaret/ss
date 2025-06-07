#include "d/snd/d_snd_source_mgr.h"

#include "common.h"
#include "d/snd/d_snd_source.h"
#include "nw4r/ut/ut_list.h"

#include <cmath>

SND_DISPOSER_DEFINE(dSndSourceMgr_c);

dSndSourceMgr_c::dSndSourceMgr_c()
    : field_0x0010(0),
      field_0x0011(0),
      field_0x0012(0),
      field_0x0013(0),
      field_0x3860(0),
      field_0x3864(0),
      field_0x3868(0),
      field_0x386C(INFINITY),
      mpPlayerSource(nullptr),
      mpFiTalkSource(nullptr),
      field_0x3878(nullptr),
      field_0x387C(nullptr),
      field_0x3880(nullptr),
      field_0x3884(nullptr) {
    // TODO offsetof
    nw4r::ut::List_Init(&mGroupList1, 0);
    nw4r::ut::List_Init(&mGroupList2, 0);

    // TODO figure out what these are for
    nw4r::ut::List_Init(&mGroupList3, 8);
    nw4r::ut::List_Init(&mSourceList1, 0xE8);
    nw4r::ut::List_Init(&field_0x3848, 0x15C);
    nw4r::ut::List_Init(&field_0x3854, 0x160);

    mpDefaultGroup = new dSndSourceGroup_c(-1, "Default", 0, 0);

    for (dSndSourceGroup_c *group = &mGroups[0]; group < &mGroups[NUM_GROUPS]; group++) {
        nw4r::ut::List_Append(&mGroupList2, group);
    }
}
