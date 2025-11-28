#ifndef D_T_STREAM_H
#define D_T_STREAM_H

#include "d/t/d_tg.h"
#include "f/f_list_mg.h"
#include "f/f_list_nd.h"
#include "m/m_mtx.h"

class dTgStream_c : public dTg_c {
public:
    dTgStream_c() : mStreamNode(this) {}
    virtual ~dTgStream_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;

    // The following lists are defined in the dol
    static fLiMgBa_c sStreamList;
    static fLiMgBa_c sSandStreamList;

    void addToList(fLiMgBa_c &list);

private:
    /* 0x0FC */ u8 mSceneflag; ///< Sceneflag to indicate when to flow
    /* 0x0FD */ u8 mAreaIdx;
    /* 0x0FE */ u8 mSpeed;
    /* 0x0FF */ u8 mOrder;
    /* 0x100 */ u8 mIsSand;
    /* 0x101 */ u8 field_0x101;
    /* 0x104 */ mMtx_c mArea;

    /* 0x134 */ fLiNdBa_c mStreamNode;
};

#endif
