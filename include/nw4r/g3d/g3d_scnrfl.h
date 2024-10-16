#ifndef NW4R_G3D_SCNRFL_H
#define NW4R_G3D_SCNRFL_H
#include "RFL/RFL_MiddleDatabase.h"
#include "RFL/RFL_Model.h"
#include "common.h"
#include "nw4r/g3d/g3d_scnobj.h"


namespace nw4r {
namespace g3d {
class ScnRfl : public ScnLeaf {
public:
    struct RflData {
        bool SetupCharModel(RFLDataSource, u16, RFLMiddleDB *);
    };

public:
    static ScnRfl *Construct(MEMAllocator *, u32 *, RFLResolution, u32, u32);
    ScnRfl(MEMAllocator *, ScnRfl *, RflData *, void *, u32);

    virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
    {
        return (other == GetTypeObjStatic());
    }
    virtual void G3dProc(u32, u32, void *);  // at 0xC
    virtual ~ScnRfl();                       // at 0x10
    virtual const TypeObj GetTypeObj() const // at 0x14
    {
        return TypeObj(TYPE_NAME);
    }
    virtual const char *GetTypeName() const // at 0x18
    {
        return GetTypeObj().GetTypeName();
    }

    bool GetExpression(RFLExpression *);
    bool SetExpression(RFLExpression);

    void SetFogIdx(int);
    void SetLightSetIdx(int);

    static const nw4r::g3d::G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

private:
    NW4R_G3D_TYPE_OBJ_DECL(ScnRfl);
};
} // namespace g3d
} // namespace nw4r

#endif
