#include "nw4r/lyt/lyt_arcResourceAccessor.h"
#include "nw4r/ut/ut_Font.h"

namespace {

// FindNameResource__37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCc
s32 FindResourcename(ARCHandle *pArcHandle, const char *resName) {
    return 1;
}

// GetResourceSub__37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCcUlPCcPUl
void *GetResourceSub(ARCHandle *pArcHandle, const char *resRootDir, u32 resType, const char *name, u32 *pSize) {}

} // namespace

namespace nw4r {
namespace lyt {
// __ct__Q34nw4r3lyt11FontRefLinkFv
FontRefLink::FontRefLink() {}

// Set__Q34nw4r3lyt11FontRefLinkFPCcPQ34nw4r2ut4Font
void FontRefLink::Set(const char *name, ut::Font *pFont) {}

// __ct__Q34nw4r3lyt19ArcResourceAccessorFv
ArcResourceAccessor::ArcResourceAccessor() {}

// Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc
bool ArcResourceAccessor::Attach(void *archiveStart, const char *resourceRootDirectory) {
    return false;
}

// Detach__Q34nw4r3lyt19ArcResourceAccessorFv

// GetResource__Q34nw4r3lyt19ArcResourceAccessorFUlPCcPUl

// RegistFont__Q34nw4r3lyt19ArcResourceAccessorFPQ34nw4r3lyt11FontRefLink

// UnregistFont__Q34nw4r3lyt19ArcResourceAccessorFPQ34nw4r3lyt11FontRefLink

// GetFont__Q34nw4r3lyt19ArcResourceAccessorFPCc

// __dt__Q34nw4r3lyt24MultiArcResourceAccessorFv

} // namespace lyt
} // namespace nw4r

// 29c03b: <88> TAG_global_subroutine
//         AT_sibling(0x29c0dc)
//         AT_name("Detach")
//         AT_lo_user("Detach__Q34nw4r3lyt19ArcResourceAccessorFv")
//         AT_member(0x16271)
//         AT_fund_type(FT_pointer)
//         AT_low_pc(0x802d50c8)
//         AT_high_pc(0x802d5158)

// 29c093: <38> TAG_formal_parameter
//         AT_sibling(0x29c0b9)
//         AT_name("this")
//         AT_mod_u_d_type(<6>MOD_const MOD_pointer_to 0x16271)
//         AT_location(<5>OP_REG(0x1f) )

// 29c0dc: <105> TAG_global_subroutine
//         AT_sibling(0x29c1ec)
//         AT_name("GetResource")
//         AT_lo_user("GetResource__Q34nw4r3lyt19ArcResourceAccessorFUlPCcPUl")
//         AT_member(0x16271)
//         AT_fund_type(FT_pointer)
//         AT_low_pc(0x802d5158)
//         AT_high_pc(0x802d51a4)

// 29c145: <38> TAG_formal_parameter
//         AT_sibling(0x29c16b)
//         AT_name("this")
//         AT_mod_u_d_type(<6>MOD_const MOD_pointer_to 0x16271)
//         AT_location(<5>OP_REG(0x1f) )

// 29c16b: <41> TAG_formal_parameter
//         AT_sibling(0x29c194)
//         AT_name("resType")
//         AT_fund_type(FT_unsigned_long)
//         AT_location(<11>OP_BASEREG(0x1) OP_CONST(0x8) OP_ADD )

// 29c194: <42> TAG_formal_parameter
//         AT_sibling(0x29c1be)
//         AT_name("name")
//         AT_mod_fund_type(<4>MOD_pointer_to MOD_const FT_char)
//         AT_location(<11>OP_BASEREG(0x1) OP_CONST(0xc) OP_ADD )

// 29c1be: <42> TAG_formal_parameter
//         AT_sibling(0x29c1e8)
//         AT_name("pSize")
//         AT_mod_fund_type(<3>MOD_pointer_to FT_unsigned_long)
//         AT_location(<11>OP_BASEREG(0x1) OP_CONST(0x10) OP_ADD )
// 29c1e8: <4>

// 29c1ec: <116> TAG_global_subroutine
//         AT_sibling(0x29c2b6)
//         AT_name("RegistFont")
//         AT_lo_user("RegistFont__Q34nw4r3lyt19ArcResourceAccessorFPQ34nw4r3lyt11FontRefLink")
//         AT_member(0x16271)
//         AT_low_pc(0x802d51a4)
//         AT_high_pc(0x802d5228)

// 29c260: <44> TAG_formal_parameter
//         AT_sibling(0x29c28c)
//         AT_name("this")
//         AT_mod_u_d_type(<6>MOD_const MOD_pointer_to 0x16271)
//         AT_location(<11>OP_BASEREG(0x1) OP_CONST(0x8) OP_ADD )

// 29c28c: <38> TAG_formal_parameter
//         AT_sibling(0x29c2b2)
//         AT_name("pLink")
//         AT_mod_u_d_type(<5>MOD_pointer_to 0x1613f)
//         AT_location(<5>OP_REG(0x1d) )
// 29c2b2: <4>

// 29c2b6: <120> TAG_global_subroutine
//         AT_sibling(0x29c384)
//         AT_name("UnregistFont")
//         AT_lo_user("UnregistFont__Q34nw4r3lyt19ArcResourceAccessorFPQ34nw4r3lyt11FontRefLink")
//         AT_member(0x16271)
//         AT_low_pc(0x802d5228)
//         AT_high_pc(0x802d52ac)

// 29c32e: <44> TAG_formal_parameter
//         AT_sibling(0x29c35a)
//         AT_name("this")
//         AT_mod_u_d_type(<6>MOD_const MOD_pointer_to 0x16271)
//         AT_location(<11>OP_BASEREG(0x1) OP_CONST(0x8) OP_ADD )

// 29c35a: <38> TAG_formal_parameter
//         AT_sibling(0x29c380)
//         AT_name("pLink")
//         AT_mod_u_d_type(<5>MOD_pointer_to 0x1613f)
//         AT_location(<5>OP_REG(0x1d) )
// 29c380: <4>

// 29c384: <97> TAG_global_subroutine
//         AT_sibling(0x29c43f)
//         AT_name("GetFont")
//         AT_lo_user("GetFont__Q34nw4r3lyt19ArcResourceAccessorFPCc")
//         AT_member(0x16271)
//         AT_mod_u_d_type(<5>MOD_pointer_to 0x15c72)
//         AT_low_pc(0x802d52ac)
//         AT_high_pc(0x802d52e0)

// 29c3e5: <44> TAG_formal_parameter
//         AT_sibling(0x29c411)
//         AT_name("this")
//         AT_mod_u_d_type(<6>MOD_const MOD_pointer_to 0x16271)
//         AT_location(<11>OP_BASEREG(0x1) OP_CONST(0x8) OP_ADD )

// 29c411: <42> TAG_formal_parameter
//         AT_sibling(0x29c43b)
//         AT_name("name")
//         AT_mod_fund_type(<4>MOD_pointer_to MOD_const FT_char)
//         AT_location(<11>OP_BASEREG(0x1) OP_CONST(0xc) OP_ADD )
// 29c43b: <4>

// 29c43f: <95> TAG_global_subroutine
//         AT_sibling(0x29c4c8)
//         AT_name("__dt")
//         AT_lo_user("__dt__Q34nw4r3lyt24MultiArcResourceAccessorFv")
//         AT_member(0x63219)
//         AT_fund_type(FT_pointer)
//         AT_low_pc(0x802d533c)
//         AT_high_pc(0x802d53cc)
//         (0x29c83e)

// 29c49e: <38> TAG_formal_parameter
//         AT_sibling(0x29c4c4)
//         AT_name("this")
//         AT_mod_u_d_type(<6>MOD_const MOD_pointer_to 0x63219)
//         AT_location(<5>OP_REG(0x1f) )
// 29c4c4: <4>

// AT_name("DetachAll")
// AT_lo_user("DetachAll__Q34nw4r3lyt24MultiArcResourceAccessorFv")

// AT_name("GetResource")
// AT_lo_user("GetResource__Q34nw4r3lyt24MultiArcResourceAccessorFUlPCcPUl")
// AT_fund_type(FT_pointer)
// AT_name("this")
// AT_mod_u_d_type(<6>MOD_const MOD_pointer_to 0x63219)
// AT_name("resType")
// AT_fund_type(FT_unsigned_long)
// AT_name("name")
// AT_mod_fund_type(<4>MOD_pointer_to MOD_const FT_char)
// AT_name("pSize")
// AT_mod_fund_type(<3>MOD_pointer_to FT_unsigned_long)

// AT_lo_user("GetFont__Q34nw4r3lyt24MultiArcResourceAccessorFPCc")
// AT_name("name")
// AT_mod_fund_type(<4>MOD_pointer_to MOD_const FT_char)

// "__vt__Q34nw4r3lyt24MultiArcResourceAccessor"
// "__vt__Q34nw4r3lyt19ArcResourceAccessor"
