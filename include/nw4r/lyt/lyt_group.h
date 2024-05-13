#ifndef NW4R_LYT_GROUP_H
#define NW4R_LYT_GROUP_H
#include "common.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/ut/ut_LinkList.h"

namespace nw4r {
namespace lyt {
namespace res {
struct Group {
    char UNK_0x0[0x8];
    char mName[NW4R_RES_NAME_SIZE]; // at 0x8
    u16 SHORT_0x18;
};
} // namespace res

namespace detail {
struct PaneLink {
    ut::LinkListNode mNode; // at 0x0
    Pane *PANE_0x8;
};
} // namespace detail

struct Group {
    Group(const res::Group *, Pane *);
    virtual ~Group();
    void AppendPane(Pane *);
    void Init();

    ut::LinkListNode mNode;                      // at 0x4
    ut::LinkList<detail::PaneLink, 0> mPaneList; // at 0xC
    char mName[NW4R_RES_NAME_SIZE];              // at 0x18
    bool mIsUserAllocated;                       // at 0x29
};

struct GroupContainer {
    ~GroupContainer();
    void AppendGroup(Group *);
    Group *FindGroupByName(const char *);

    ut::LinkList<Group, 4> mGroups; // at 0x4
};
} // namespace lyt
} // namespace nw4r

#endif
