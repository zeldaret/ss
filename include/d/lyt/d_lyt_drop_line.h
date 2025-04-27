#ifndef D_LYT_DROP_LINE_H
#define D_LYT_DROP_LINE_H

#include "common.h"
#include "d/a/d_a_item.h"
#include "nw4r/lyt/lyt_pane.h"

class dLytDropLine_c {
public:
    dLytDropLine_c() {
        sInstance = this;
    }
    virtual ~dLytDropLine_c() {
        sInstance = nullptr;
    }

    static dLytDropLine_c *GetInstance() {
        return sInstance;
    }

    static nw4r::lyt::Pane *getActivePane();
    static nw4r::lyt::Pane *setPane(nw4r::lyt::Pane *pane);
    static nw4r::lyt::Pane *update(nw4r::lyt::Pane *pane, nw4r::lyt::Pane *otherPane, dAcItem_c::Trial_e, bool);
    static bool finishPartMaybe();

private:
    static dLytDropLine_c *sInstance;
};

#endif
