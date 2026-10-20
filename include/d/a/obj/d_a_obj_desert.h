#ifndef D_A_OBJ_DESERT_H
#define D_A_OBJ_DESERT_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w_sv.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcODesert : public dAcObjBase_c {
public:
    dAcODesert();
    virtual ~dAcODesert();

    class ScnCallback_c : public nw4r::g3d::IScnObjCallback {
    public:
        ScnCallback_c(dAcODesert *pDesert) : mpDesert(pDesert) {}
        virtual void ExecCallback_CALC_MAT(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *);

        dAcODesert *mpDesert;
    };

    struct Entry {
        Entry() : mIsSet(0) {}

        /* 0x00 */ s32 mIsSet; // Guess
        /* 0x04 */ mVec3_c mPosition;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
    };

    void addEntry(const Entry &entry) {
        field_0x1684++;
        if (field_0x1684 >= 10) {
            field_0x1684 = 0;
        }
        mEntries[field_0x1684] = entry;
    }

private:
    /* 0x 330 */ nw4r::g3d::ResFile mRes;
    /* 0x 334 */ m3d::smdl_c mMdl;
    /* 0x 350 */ dBgWSv mBgW;

    /* 0x 568 */ u8 field_0x568[0x590 - 0x568];

    /* 0x 590 */ ScnCallback_c mScnCallback;
    /* 0x 598 */ bool mbRequestCalcMat; // Rename when better name for fn_308_910 is known

    /* 0x     */ u8 field_0x599[0x1684 - 0x599];
    /* 0x1684 */ s32 field_0x1684;
    /* 0x1688 */ Entry mEntries[1];
};

#endif
