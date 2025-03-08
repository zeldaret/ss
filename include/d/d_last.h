#ifndef D_LAST_H
#define D_LAST_H

#include "d/d_base.h"

class dLast_c : public dBase_c {
public:
    typedef void (*executeCallback)(void);

    static void setExecuteCallback(executeCallback cb) {
        sExecuteCallback = cb;
    }
private:
    static executeCallback sExecuteCallback;
};

#endif
