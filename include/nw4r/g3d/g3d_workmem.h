#ifndef NW4R_G3D_WORK_MEM_H
#define NW4R_G3D_WORK_MEM_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanmshp.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace workmem {

/******************************************************************************
 *
 * MdlZ
 *
 ******************************************************************************/
struct MdlZ {
    f32 Z;        // at 0x0
    u16 priority; // at 0x4
    u16 nodeID;   // at 0x6
    u16 matID;    // at 0x8
    u16 shpID;    // at 0xA
};

/******************************************************************************
 *
 * ShpAnmResultBuf
 *
 ******************************************************************************/
struct ShpAnmResultBuf {
    ShpAnmResult resultBuf;      // at 0x0
    const ShpAnmResult *pResult; // at 0x218
    f32 weight;                  // at 0x21C
};

/******************************************************************************
 *
 * Work memory size constants
 *
 ******************************************************************************/
static const int WORKMEM_SIZE = 0x18000;

static const int WORKMEM_NUMTMPSCALE = 2048;
static const int WORKMEM_NUMMTXID = 2048;
static const int WORKMEM_NUMBYTECODE = WORKMEM_SIZE;
static const int WORKMEM_NUMMDLZ = 2048;
static const int WORKMEM_NUMSKINNINGMTX = 2048;
static const int WORKMEM_NUMBBMTX = 2048;
static const int WORKMEM_NUMSHPANMRESULT = WORKMEM_SIZE / sizeof(ShpAnmResultBuf);

/******************************************************************************
 *
 * Work memory accessor functions
 *
 ******************************************************************************/
math::VEC3 *GetScaleTemporary();
u32 *GetMtxIDTemporary();
MdlZ *GetMdlZTemporary();
math::MTX34 *GetSkinningMtxTemporary();
math::MTX34 *GetBillboardMtxTemporary();
ShpAnmResultBuf *GetShpAnmResultBufTemporary();

} // namespace workmem
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
