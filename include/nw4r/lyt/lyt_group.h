#ifndef NW4R_LYT_GROUP_H
#define NW4R_LYT_GROUP_H
#include "common.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/ut/ut_LinkList.h"

namespace nw4r {
namespace lyt {

namespace detail {
struct PaneLink {
    ut::LinkListNode mLink; // at 0x0
    Pane *mTarget;          // at 0x08
};
} // namespace detail

typedef ut::LinkList<detail::PaneLink, 0> PaneList;

class Group {
public:
    Group(const res::Group *pResGroup, Pane *pRootPane);

    virtual ~Group();
    void AppendPane(Pane *pPane);
    void Init();

    PaneList *GetPaneList() {
        return &mPaneListLink;
    }
    bool IsUserAllocated() const {
        return mbUserAllocated;
    }
    const char *GetName() const {
        return mName;
    }

private:
    ut::LinkListNode mLink;                          // at 0x04
    PaneList mPaneListLink; // at 0x0C
    char mName[NW4R_RES_NAME_SIZE + 1];              // at 0x18
    bool mbUserAllocated;                            // at 0x29
    u8 mPadding[2];                                  // at 0x2A
};

struct GroupContainer {
    ~GroupContainer();
    void AppendGroup(Group *pGroup);
    Group *FindGroupByName(const char *findName);

    ut::LinkList<Group, 4> mGroupList; // at 0x4
};
} // namespace lyt
} // namespace nw4r

#endif
