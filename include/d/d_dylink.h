#ifndef D_DYLINK_H
#define D_DYLINK_H

#include <common.h>

class dDynamicModuleControl {
public:
    dDynamicModuleControl() {}
    virtual ~dDynamicModuleControl();

    void set(u16 *ptr, int count);
    BOOL do_link() const;
    BOOL do_unlink();

private:
    u16 *mPtr;
    int mCount;
};

#endif
