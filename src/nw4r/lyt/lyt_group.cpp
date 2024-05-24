#include <nw4r/lyt/lyt_group.h>
#include <nw4r/lyt/lyt_layout.h>

namespace nw4r {

namespace lyt {

// __ct__Q34nw4r3lyt5GroupFPCQ44nw4r3lyt3res5GroupPQ34nw4r3lyt4Pane
Group::Group(const res::Group *pResGroup, Pane *pRootPane) : mLink(), mPaneListLink() {
    Init();
    strncpy(this->mName, pResGroup->mName, NW4R_RES_NAME_SIZE);
    this->mName[NW4R_RES_NAME_SIZE] = '\0';
    const char *paneNameBase = detail::ConvertOffsetToPtr<char>(pResGroup, sizeof(res::Group));

    for (int i = 0; i < pResGroup->paneNum; i++) {
        Pane *pFindPane = pRootPane->FindPaneByName(paneNameBase + i * NW4R_RES_NAME_SIZE, true);
        if (pFindPane) {
            AppendPane(pFindPane);
        }
    }
}

// Init__Q34nw4r3lyt5GroupFv
void Group::Init() {
    this->mbUserAllocated = false;
}

// __dt__Q34nw4r3lyt5GroupFv
Group::~Group() {
    ut::LinkList<detail::PaneLink, 0>::Iterator it = this->mPaneListLink.GetBeginIter();
    while (it != this->mPaneListLink.GetEndIter()) {
        ut::LinkList<detail::PaneLink, 0>::Iterator currIt = it++;
        this->mPaneListLink.Erase(currIt);
        Layout::DeleteObj<detail::PaneLink>(&*currIt);
    }
}

// AppendPane__Q34nw4r3lyt5GroupFPQ34nw4r3lyt4Pane
void Group::AppendPane(Pane *pPane) {
    detail::PaneLink *pPaneLink = Layout::NewObj<detail::PaneLink>();
    if (pPaneLink) {
        pPaneLink->mTarget = pPane;
        this->mPaneListLink.PushBack(pPaneLink);
    }
}

//__dt__Q34nw4r3lyt14GroupContainerFv
GroupContainer::~GroupContainer() {
    ut::LinkList<Group, 4>::Iterator it = this->mGroupList.GetBeginIter();
    while (it != this->mGroupList.GetEndIter()) {
        ut::LinkList<Group, 4>::Iterator currIt = it++;
        this->mGroupList.Erase(currIt);
        if (!currIt->IsUserAllocated()) {
            Layout::DeleteObj<Group>(&*currIt);
        }
    }
}

// AppendGroup__Q34nw4r3lyt14GroupContainerFPQ34nw4r3lyt5Group
void GroupContainer::AppendGroup(Group *pGroup) {
    this->mGroupList.PushBack(pGroup);
}

// FindGroupByName__Q34nw4r3lyt14GroupContainerFPCc
Group *GroupContainer::FindGroupByName(const char *findName) {
    for (ut::LinkList<Group, 4>::Iterator it = this->mGroupList.GetBeginIter(); it != this->mGroupList.GetEndIter();
            it++) {
        if (detail::EqualsResName(it->GetName(), findName)) {
            return &*it;
        }
    }
    return nullptr;
}

} // namespace lyt

} // namespace nw4r
