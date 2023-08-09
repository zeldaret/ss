#ifndef RVL_SDK_MTX_QUAT_H
#define RVL_SDK_MTX_QUAT_H
#include <lib/rvl/MTX/mtx.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void PSQUATMultiply(const Quaternion*, const Quaternion*, Quaternion*);
void PSQUATNormalize(const Quaternion*, Quaternion*);
void C_QUATMtx(Quaternion*, const Mtx);
void C_QUATSlerp(const Quaternion*, const Quaternion*, Quaternion*, f32);

#ifdef __cplusplus
}
#endif
#endif
