
#include "d/snd/d_snd_util.h"

class SndClass80399a40 {
public:
    SndClass80399a40();
    virtual ~SndClass80399a40() {}

    /* 0x04 */ u8 _0x04[0x18 - 0x04];
};

// This is a test for SndMgrDisposer, since this
// class has the disposer at 0x18, which is interesting

class SndMgr80399c20;

extern template class SndMgrDisposer<SndMgr80399c20>;

class SndMgr80399c20 : SndClass80399a40 {
public:
    SndMgrDisposer<SndMgr80399c20> *GetDisposer() {
        return &mDisposer;
    }

    static SndMgr80399c20 *GetInstance() {
        return sInstance;
    }

    static SndMgr80399c20 *sInstance;
    static SndMgrDisposer<SndMgr80399c20> *sDisposer;

private:
    SndMgrDisposer<SndMgr80399c20> mDisposer;
};

template class SndMgrDisposer<SndMgr80399c20>;
