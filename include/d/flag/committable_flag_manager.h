#ifndef D_FLAG_COMMITTABLE_FLAG_MANAGER_H
#define D_FLAG_COMMITTABLE_FLAG_MANAGER_H

class CommittableFlagManager {
public:
    bool mNeedsCommit;

    virtual void doCommit() = 0;
    virtual ~CommittableFlagManager() {}
    bool commit();
    void setNeedsCommit(bool commit) {
        mNeedsCommit = commit;
    }
    CommittableFlagManager() {
        mNeedsCommit = false;
    }
    CommittableFlagManager(bool commit) {
        mNeedsCommit = commit;
    }
};


#endif
