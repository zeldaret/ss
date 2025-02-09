#ifndef NW4R_PUBLIC_G3D_H
#define NW4R_PUBLIC_G3D_H

#include <nw4r/g3d/g3d_3dsmax.h>
#include <nw4r/g3d/g3d_anmchr.h>
#include <nw4r/g3d/g3d_anmclr.h>
#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/g3d_anmscn.h>
#include <nw4r/g3d/g3d_anmshp.h>
#include <nw4r/g3d/g3d_anmtexpat.h>
#include <nw4r/g3d/g3d_anmtexsrt.h>
#include <nw4r/g3d/g3d_anmvis.h>
#include <nw4r/g3d/g3d_basic.h>
#include <nw4r/g3d/g3d_calcmaterial.h>
#include <nw4r/g3d/g3d_calcview.h>
#include <nw4r/g3d/g3d_calcvtx.h>
#include <nw4r/g3d/g3d_calcworld.h>
#include <nw4r/g3d/g3d_camera.h>
#include <nw4r/g3d/g3d_dcc.h>
#include <nw4r/g3d/g3d_draw.h>
#include <nw4r/g3d/g3d_draw1mat1shp.h>
#include <nw4r/g3d/g3d_fog.h>
#include <nw4r/g3d/g3d_init.h>
#include <nw4r/g3d/g3d_light.h>
#include <nw4r/g3d/g3d_maya.h>
#include <nw4r/g3d/g3d_obj.h>
#include <nw4r/g3d/g3d_rtti.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/g3d/g3d_scnmdl1mat1shp.h>
#include <nw4r/g3d/g3d_scnmdlsmpl.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/g3d/g3d_scnproc.h>
#include <nw4r/g3d/g3d_scnrfl.h>
#include <nw4r/g3d/g3d_scnroot.h>
#include <nw4r/g3d/g3d_state.h>
#include <nw4r/g3d/g3d_workmem.h>
#include <nw4r/g3d/g3d_xsi.h>

/******************************************************************************
 *
 * Resources
 *
 ******************************************************************************/

#include <nw4r/g3d/res/g3d_resanm.h>
#include <nw4r/g3d/res/g3d_resanmamblight.h>
#include <nw4r/g3d/res/g3d_resanmcamera.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/g3d/res/g3d_resanmclr.h>
#include <nw4r/g3d/res/g3d_resanmfog.h>
#include <nw4r/g3d/res/g3d_resanmlight.h>
#include <nw4r/g3d/res/g3d_resanmscn.h>
#include <nw4r/g3d/res/g3d_resanmshp.h>
#include <nw4r/g3d/res/g3d_resanmtexpat.h>
#include <nw4r/g3d/res/g3d_resanmtexsrt.h>
#include <nw4r/g3d/res/g3d_resanmvis.h>
#include <nw4r/g3d/res/g3d_rescommon.h>
#include <nw4r/g3d/res/g3d_resdict.h>
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_reslightset.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/g3d/res/g3d_respltt.h>
#include <nw4r/g3d/res/g3d_resshp.h>
#include <nw4r/g3d/res/g3d_restev.h>
#include <nw4r/g3d/res/g3d_restex.h>
#include <nw4r/g3d/res/g3d_resvtx.h>

/******************************************************************************
 *
 * Platform-specific
 *
 ******************************************************************************/

#include <nw4r/g3d/platform/g3d_allocator.h>
#include <nw4r/g3d/platform/g3d_cpu.h>
#include <nw4r/g3d/platform/g3d_gpu.h>
#include <nw4r/g3d/platform/g3d_tmem.h>

#endif
