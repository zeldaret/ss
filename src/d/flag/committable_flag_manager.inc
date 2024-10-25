#include "d/flag/committable_flag_manager.h"

/* 0x800BE7B0 */
bool CommittableFlagManager::commit() {
    if (mNeedsCommit) {
        doCommit();
        mNeedsCommit = false;
        return true;
    } else {
        return false;
    }
}
