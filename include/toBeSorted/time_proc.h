#ifndef TIME_PROC_H
#define TIME_PROC_H

/**
 * A callback implementation that controls ???
 */

#include "m/m3d/m_proc.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/g3d_scnobj.h"

class TimeCallback_c : public nw4r::g3d::IScnObjCallback {
public:
    TimeCallback_c() : field_0x04(0), field_0x08(0), field_0x09(0) {}
    virtual ~TimeCallback_c() {}
    virtual void ExecCallback_CALC_VIEW(
        nw4r::g3d::ScnObj::Timing /* timing */, nw4r::g3d::ScnObj * /* pObj */, u32 /* param */, void * /* pInfo */
    ) override;

    /* 0x04 */ u32 field_0x04;
    /* 0x08 */ u8 field_0x08;
    /* 0x09 */ u8 field_0x09;
};

class TimeProcBase : public m3d::proc_c {
public:
    virtual ~TimeProcBase() {}
};

class TimeProc : public TimeProcBase {
public:
    TimeProc() {}
    virtual ~TimeProc() {}

    TimeCallback_c mCallback;
};

class MdlWithTimeProc : public m3d::smdl_c {
public:
    int fn_8001F3B0(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption);

    TimeProc mProc;
};

#endif // TIME_PROC_H
