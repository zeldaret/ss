#include "nw4r/g3d.h" // IWYU pragma: export

#include "nw4r/math.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {
namespace detail {

using nw4r::math::MTX33;
using nw4r::math::MTX34;
using nw4r::math::VEC3;

const f32 dat_epsilon = 1e-36f;

// clang-format off
asm bool CalcViewNrmMtx(register math::MTX33 *pOut, const register math::MTX34 *pW) {
    nofralloc
    psq_l      f0,  MTX34._00(pW), 1, 0
    psq_l      f1,  MTX34._01(pW), 0, 0
    psq_l      f2,  MTX34._10(pW), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3,  MTX34._11(pW), 0, 0
    psq_l      f4,  MTX34._20(pW), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5,  MTX34._21(pW), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, dat_epsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, dat_epsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6,  0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        _lbl_80442B80
    psq_st     f0,  MTX33._00(pOut), 1, 0
    psq_st     f2,  MTX33._10(pOut), 1, 0
    psq_st     f4,  MTX33._20(pOut), 1, 0
    psq_st     f1,  MTX33._01(pOut), 0, 0
    psq_st     f3,  MTX33._11(pOut), 0, 0
    psq_st     f5,  MTX33._21(pOut), 0, 0
    li         r3,  0x0
    blr

_lbl_80442B80:
    fres       f5,  f7
    ps_mul     f8,  f1,  f2
    ps_sub     f6,  f6,  f6
    ps_add     f1,  f5,  f5
    ps_msub    f8,  f0,  f3,  f8
    ps_mul     f2,  f7,  f5
    ps_nmsub   f0,  f5,  f2,  f1
    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, MTX33._00(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, MTX33._10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, MTX33._20(pOut), 0, 0
    ps_muls0   f9,  f9,  f0
    psq_st     f10, MTX33._02(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    psq_st     f9,  MTX33._12(pOut), 1, 0
    psq_st     f8,  MTX33._22(pOut), 1, 0
    li         r3,  0x1
    blr
}

asm bool CalcViewTexMtx(register math::MTX34 *pOut, const register math::MTX34 *pW) {
    nofralloc
    psq_l      f0,  MTX34._00(pW), 1, 0
    psq_l      f1,  MTX34._01(pW), 0, 0
    psq_l      f2,  MTX34._10(pW), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3,  MTX34._11(pW), 0, 0
    psq_l      f4,  MTX34._20(pW), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5,  MTX34._21(pW), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, dat_epsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, dat_epsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6,  0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        _lbl_80442C88
    cmplw      pOut,  pW
    beq        _lbl_80442C80
    psq_st     f0, MTX34._00(pOut), 1, 0
    psq_st     f2, MTX34._10(pOut), 1, 0
    psq_st     f4, MTX34._20(pOut), 1, 0
    ps_sub     f6,  f6,  f6
    psq_st     f1, MTX34._01(pOut), 0, 0
    psq_st     f3, MTX34._11(pOut), 0, 0
    psq_st     f5, MTX34._21(pOut), 0, 0
    psq_st     f6, MTX34._03(pOut), 1, 0
    psq_st     f6, MTX34._13(pOut), 1, 0
    psq_st     f6, MTX34._23(pOut), 1, 0
_lbl_80442C80:
    li         r3, 0x0
    blr
_lbl_80442C88:
    fres       f5,  f7
    ps_mul     f8,  f1,  f2
    ps_sub     f6,  f6,  f6
    ps_add     f1,  f5,  f5
    ps_msub    f8,  f0,  f3,  f8
    psq_st     f6,  MTX34._03(pOut), 1, 0
    ps_mul     f2,  f7,  f5
    psq_st     f6,  MTX34._13(pOut), 1, 0
    psq_st     f6,  MTX34._23(pOut), 1, 0
    ps_nmsub   f0,  f5,  f2,  f1
    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, MTX34._00(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, MTX34._10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, MTX34._20(pOut), 0, 0
    ps_muls0   f9,  f9,  f0
    psq_st     f10, MTX34._02(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    psq_st     f9,  MTX34._12(pOut), 1, 0
    psq_st     f8,  MTX34._22(pOut), 1, 0
    li         r3,  0x1
    blr
}

asm bool CalcInvWorldMtx(register math::MTX34 *pOut, const register math::MTX34 *pW) {
    nofralloc
    psq_l      f0, MTX34._00(pW), 1, 0
    psq_l      f1, MTX34._01(pW), 0, 0
    psq_l      f2, MTX34._10(pW), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3, MTX34._11(pW), 0, 0
    psq_l      f4, MTX34._20(pW), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5, MTX34._21(pW), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, dat_epsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, dat_epsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6, 0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        _lbl_80442D68
    li         r3, 0x0
    blr
_lbl_80442D68:
    fres       f6,  f7
    ps_mul     f8,  f1,  f2
    ps_add     f5,  f6,  f6
    ps_mul     f4,  f7,  f6
    ps_msub    f8,  f0,  f3,  f8
    ps_nmsub   f0,  f6,  f4,  f5
    lfs        f1,  MTX34._03(pW)
    ps_muls0   f13, f13, f0
    lfs        f2,  MTX34._13(pW)
    ps_muls0   f12, f12, f0
    lfs        f3,  MTX34._23(pW)
    ps_muls0   f11, f11, f0
    ps_merge00 f5,  f13, f12
    ps_merge11 f4,  f13, f12
    ps_mul     f6,  f13, f1
    psq_st     f5,  MTX34._00(pOut), 0, 0
    psq_st     f4,  MTX34._10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    ps_muls0   f9,  f9,  f0
    ps_madd    f6,  f12, f2,  f6
    psq_st     f10, MTX34._20(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    ps_nmadd   f6,  f11, f3,  f6
    psq_st     f9,  MTX34._21(pOut), 1, 0
    ps_mul     f7,  f10, f1
    ps_merge00 f5,  f11, f6
    psq_st     f8,  MTX34._22(pOut), 1, 0
    ps_madd    f7,  f9,  f2,  f7
    ps_merge11 f4,  f11, f6
    psq_st     f5,  MTX34._02(pOut), 0, 0
    ps_nmadd   f7,  f8,  f3,  f7
    psq_st     f4,  MTX34._12(pOut), 0, 0
    psq_st     f7,  MTX34._23(pOut), 1, 0
    li         r3,  0x1
    blr
}

// clang-format on

} // namespace detail
} // namespace g3d
} // namespace nw4r
