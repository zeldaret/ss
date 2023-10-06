#pragma once

#include "types.h"


class mFaderBase_c;
class mFader_c;
class mColorFader_c;
class mWipeFader_c;

// Heap Stuff
struct mHeap; // Maybe just a namespace?
class mAllocator_c;
class mHeapAllocator_c;

// Math Stuff
class mMtx_c;
class mQuat_c;
struct mAng;
class mAng3_c;
class mVec3_c;
class mFrustum_c;

namespace m2d {
    class Layout_c;
    class AnmResV2_c;
    class AnmGroup_c;
    class Simple_c;
    class FrameCtrl_c;
    class AnmGroupBase_c;
    class Base_c;
    class ResAccIf_c;
} // namespace m2d

namespace m3d {
    class mdl_c; // also contains ::callback_c ::mdlCallback_c
    class smdl_c;
    class bmdl_c;

    class scnLeaf_c;

    class banm_c;
    class fanm_c;
    
    class anmChr_c;
    class anmChrBlend_c;
    class anmMatClr_c; // also contains ::child_c
    class anmTexPat_c; // also contains ::child_c
    class anmTexSrt_c; // also contains ::child_c


    class anmVis_c;

    class capture_c;
    class calcRatio_c;
} // namespace m3d

class mDvd_command_c;
class mDvd_param_c;
class mDvd_toMainRam_c;
class mDvd_callback_c;
class mDvd_mountMemArchive_c;
namespace mDvd
{
    class MyThread_c;

    template <typename T> // types are from EGG::StreamDecomp[LH/LRC/RL/LZ]
    class TUncompressedInfo_c; 

} // namespace mDvd
