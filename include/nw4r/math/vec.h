#pragma once

#include "nw4r/nw4r_types.h"

// struct Vec {
//     f32 x,y,z;
// };

namespace nw4r
{
    namespace math
    {
        // forward decl
        struct _VEC2;  struct VEC2;
        struct _VEC3;  struct VEC3;
        struct _QUAT;  struct QUAT;
        struct _MTX33; struct MTX33;
        struct _MTX34; struct MTX34;
        struct _MTX44; struct MTX44;
        void VEC3Add(VEC3* pOut, const VEC3* p1, const VEC3* p2);
        void VEC3Sub(VEC3* pOut, const VEC3* p1, const VEC3* p2);
        void VEC3Scale(VEC3* pOut, const VEC3* p, f32 scale);
        f32 VEC3Len(const VEC3* p);
        f32 VEC3LenSq(const VEC3* p);
        f32 VEC3DistSq(const VEC3* p1, const VEC3* p2);
        void VEC3Normalize(VEC3* pOut, const VEC3* p);
        f32 VEC3Dot(const VEC3* p1, const VEC3* p2);
        void VEC3Cross(VEC3* pOut, const VEC3* p1, const VEC3* p2);
        void VEC3Lerp(VEC3* pOut, const VEC3* p1, const VEC3* p2, f32 t);
        void VEC3Transform(VEC3* pOut, const MTX34* pM, const VEC3* pV);
        void VEC3TransformCoord(VEC3* pOut, const MTX34* pM, const VEC3* pV);

        struct _VEC2 {f32 x, y;};
        struct VEC2 : _VEC2 {
            VEC2() {}
            VEC2(f32 fx, f32 fy) { x = fx; y = fy;}
        };

        struct _VEC3 { f32 x,y,z; };
        struct VEC3 : public _VEC3 {
            VEC3() {}
            VEC3(f32 fx, f32 fy, f32 fz) {  x = fx; y = fy; z = fz;}
            // VEC3(const Vec& v) { x = v.x; y = v.y; z = v.z;}
            VEC3(const f32* p) { x = p[0]; x = p[1]; z = p[2];}
            VEC3(const _VEC3& v) { x = v.x; y = v.y; z = v.z; }

            // operator Vec*() { return (Vec*)this; }
            // operator const Vec*() const { return (const Vec*)this; }

            f32 LenSq() const {
                return x*x + y*y + z*z;
            }

            VEC3 operator+(const VEC3& rhs) const {
                VEC3 tmp;
                VEC3Add(&tmp, this, &rhs);
                return tmp;
            }
            VEC3& operator+=(const VEC3& rhs) {
                VEC3Add(this, this, &rhs);
                return *this;
            }
            VEC3 operator-() const {
                return VEC3(-x, -y, -z);
            }
            VEC3 operator-(const VEC3& rhs) const {
                VEC3 tmp;
                VEC3Sub(&tmp, this, &rhs);
                return tmp;
            }
            VEC3& operator-=(const VEC3& rhs) {
                VEC3Sub(this, this, &rhs);
                return *this;
            }
            VEC3 operator*(f32 f) const {
                VEC3 tmp;
                VEC3Scale(&tmp, this, f);
                return tmp;
            }
            VEC3& operator*=(f32 f) {
                VEC3Scale(this, this, f);
                return *this;
            }
            VEC3 operator/(f32 f) const {
                f32 r = 1.0f / f; // DWARF has it
                return operator*(r);
            }
            VEC3& operator/=(f32 f) {
                return operator*=(1.0f / f);
            }
            bool operator!=(const VEC3& rhs) {
                return x != rhs.x || y != rhs.y || z != rhs.z;
            }
        };
        inline void VEC3Add(register VEC3* pOut, register const VEC3* p1, register const VEC3* p2) {
			asm {
				psq_l f2, 0x0(p1), 0, 0
				psq_l f2, 0x0(p2), 0, 0
				ps_add f0, f2, f1
				psq_l f2, 0x8(p1), 1, 0
				psq_l f2, 0x8(p2), 1, 0
				psq_st f0, 0x0(pOut), 0, 0
				ps_add f0, f2, f1
				psq_st f0, 0x8(pOut), 1, 0
			}
        }
        inline void VEC3Sub(register VEC3* pOut, register const VEC3* p1, register const VEC3* p2) {
			asm
			{
				psq_l f2, 0x0(p1), 0, 0
				psq_l f2, 0x0(p2), 0, 0
				ps_sub f0, f2, f1
				psq_l f2, 0x8(p1), 1, 0
				psq_l f2, 0x8(p2), 1, 0
				psq_st f0, 0x0(pOut), 0, 0
				ps_sub f0, f2, f1
				psq_st f0, 0x8(pOut), 1, 0
			}
        }
        inline void VEC3Scale(register VEC3* pOut, register const VEC3* p, register f32 scale) {
			asm
			{
				psq_l f2, 0x0(p), 0, 0
				ps_muls0 f1, f2, scale
				psq_l f2, 0x8(p), 0, 1
				psq_st f1, 0x0(pOut), 0, 0
				ps_muls0 f1, f2, scale
				psq_st f1, 0x8(pOut), 1, 0
			}
        }
        inline float VEC3Dot(register const VEC3 * p1, register const VEC3 * p2)
		{
			register f32 a, b, d, c, e;
			asm
			{
				psq_l a, 0x4(p1), 0, 0
				psq_l b, 0x4(p2), 0, 0
				ps_mul a, a, b
				psq_l c, 0(p1), 1, 0
				psq_l d, 0(p2), 1, 0
				ps_madd b, c, d, a
				ps_sum0 e, b, a, a
			}
			return e;
		}
    } // namespace math

} // namespace nw4r