#include <nw4r/g3d.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {

void CalcTexMtx(math::MTX34* pMtx, bool set, const TexSrt& rSrt,
                TexSrt::Flag flag, TexSrtTypedef::TexMatrixMode mode) {
    bool ident = true;

    if (mode == TexSrtTypedef::TEXMATRIXMODE_MAYA) {
        ident = !detail::dcc::CalcTexMtx_Maya(pMtx, set, rSrt, flag);
    } else if (mode == TexSrtTypedef::TEXMATRIXMODE_XSI) {
        ident = !detail::dcc::CalcTexMtx_Xsi(pMtx, set, rSrt, flag);
    } else /* TEXMATRIXMODE_3DSMAX */ {
        ident = !detail::dcc::CalcTexMtx_3dsmax(pMtx, set, rSrt, flag);
    }

    if (ident && set) {
        math::MTX34Identity(pMtx);
    }
}

} // namespace g3d
} // namespace nw4r
