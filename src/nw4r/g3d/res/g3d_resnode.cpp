#include <nw4r/g3d.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {

void ResNode::PatchChrAnmResult(ChrAnmResult *pResult) const {
    if (!IsValid()) {
        return;
    }

    const ResNodeData &r = ref();
    u32 flags = pResult->flags;

    if (flags & ChrAnmResult::FLAG_PATCH_SCALE) {
        if (r.flags & ResNodeData::FLAG_SCALE_ONE) {
            flags |= ChrAnmResult::FLAG_SCALE_ONE | ChrAnmResult::FLAG_SCALE_UNIFORM;
            pResult->s.z = 1.f;
            pResult->s.y = 1.f;
            pResult->s.x = 1.f;
        } else {
            u32 flag_ = flags & ~ChrAnmResult::FLAG_SCALE_UNIFORM;

            if (r.flags & ResNodeData::FLAG_SCALE_UNIFORM) {
                flag_ = flags | ChrAnmResult::FLAG_SCALE_UNIFORM;
            }

            flags = flag_ & ~(ChrAnmResult::FLAG_SCALE_ONE | ChrAnmResult::FLAG_MTX_IDENT);

            pResult->s = static_cast<math::VEC3 &>(r.scale);
        }
    }

    if (flags & ChrAnmResult::FLAG_PATCH_ROT) {
        math::VEC3 trans(pResult->rt[0][3], pResult->rt[1][3], pResult->rt[2][3]);
        if (r.flags & ResNodeData::FLAG_ROT_ZERO) {
            math::MTX34Identity(&pResult->rt);
            flags |= ChrAnmResult::FLAG_ROT_ZERO;
        } else {
            math::MTX34RotXYZDeg(&pResult->rt, r.rot.x, r.rot.y, r.rot.z);
            flags &= ~(ChrAnmResult::FLAG_ROT_ZERO | ChrAnmResult::FLAG_ROT_TRANS_ZERO | ChrAnmResult::FLAG_MTX_IDENT);
        }

        flags |= ChrAnmResult::FLAG_ROT_RAW_FMT;
        pResult->rt._03 = trans.x;
        pResult->rt._13 = trans.y;
        pResult->rt._23 = trans.z;
    }

    if (flags & ChrAnmResult::FLAG_PATCH_TRANS) {
        if (r.flags & ResNodeData::FLAG_TRANS_ZERO) {
            flags |= ChrAnmResult::FLAG_TRANS_ZERO;

            pResult->rt._23 = 0.f;
            pResult->rt._13 = 0.f;
            pResult->rt._03 = 0.f;
        } else {
            flags &=
                ~(ChrAnmResult::FLAG_TRANS_ZERO | ChrAnmResult::FLAG_ROT_TRANS_ZERO | ChrAnmResult::FLAG_MTX_IDENT);

            pResult->rt._03 = r.translate.x;
            pResult->rt._13 = r.translate.y;
            pResult->rt._23 = r.translate.z;
        }
    }

    if ((flags & ChrAnmResult::FLAG_ROT_ZERO) && (flags & ChrAnmResult::FLAG_TRANS_ZERO)) {
        flags |= ChrAnmResult::FLAG_ROT_TRANS_ZERO;
        if (flags & ChrAnmResult::FLAG_SCALE_ONE) {
            flags |= ChrAnmResult::FLAG_MTX_IDENT;
        }
    }

    pResult->flags =
        flags & ~(ChrAnmResult::FLAG_PATCH_SCALE | ChrAnmResult::FLAG_PATCH_ROT | ChrAnmResult::FLAG_PATCH_TRANS);
}

void ResNode::CalcChrAnmResult(ChrAnmResult *pResult) const {
    if (!IsValid()) {
        return;
    }

    const ResNodeData &r = ref();
    u32 flags = 0;

    if (r.flags & ResNodeData::FLAG_SCALE_ONE) {
        flags |= ChrAnmResult::FLAG_SCALE_ONE | ChrAnmResult::FLAG_SCALE_UNIFORM;

        pResult->s.z = 1.0f;
        pResult->s.y = 1.0f;
        pResult->s.x = 1.0f;
    } else {
        if (r.flags & ResNodeData::FLAG_SCALE_UNIFORM) {
            flags |= ChrAnmResult::FLAG_SCALE_UNIFORM;
        }

        pResult->s = static_cast<math::VEC3 &>(r.scale);
    }

    if (r.flags & ResNodeData::FLAG_ROT_ZERO) {
        PSMTXIdentity(pResult->rt);
        flags |= ChrAnmResult::FLAG_ROT_ZERO;
    } else {
        pResult->rawR = math::VEC3(r.rot);
        math::MTX34RotXYZDeg(&pResult->rt, r.rot.x, r.rot.y, r.rot.z);
    }

    if (r.flags & ResNodeData::FLAG_TRANS_ZERO) {
        flags |= ChrAnmResult::FLAG_TRANS_ZERO;
    } else {
        pResult->rt._03 = r.translate.x;
        pResult->rt._13 = r.translate.y;
        pResult->rt._23 = r.translate.z;
    }

    if (flags & ChrAnmResult::FLAG_ROT_ZERO) {
        if (flags & ChrAnmResult::FLAG_TRANS_ZERO) {
            flags |= ChrAnmResult::FLAG_ROT_TRANS_ZERO;

            if (flags & ChrAnmResult::FLAG_SCALE_ONE) {
                flags |= ChrAnmResult::FLAG_MTX_IDENT;
            }
        }
    }

    flags |= ChrAnmResult::FLAG_ROT_RAW_FMT;
    flags |= ChrAnmResult::FLAG_ANM_EXISTS;

    if (r.flags & ResNodeData::FLAG_SSC_APPLY) {
        flags |= ChrAnmResult::FLAG_SSC_APPLY;
    }

    if (r.flags & ResNodeData::FLAG_SSC_PARENT) {
        flags |= ChrAnmResult::FLAG_SSC_PARENT;
    }

    pResult->flags = flags;
}

} // namespace g3d
} // namespace nw4r
