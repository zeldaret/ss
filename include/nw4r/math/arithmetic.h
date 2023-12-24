#ifndef NW4R_MATH_ARITHMETIC_H
#define NW4R_MATH_ARITHMETIC_H

#include <nw4r/nw4r_types.h>
#include <rvl/OS.h>
#include <MSL_C/math.h>

namespace nw4r
{
    namespace math
    {
		namespace detail
		{
			f32 FExp(f32);
			f32 FLog(f32);
		}

		f32 FrSqrt(register f32);

		inline u32 F32AsU32(f32 x)
		{
			return *(u32 *)&x;
		}

		inline f32 U32AsF32(u32 x)
		{
			return *(f32 *)&x;
		}

	    inline s32 FGetExpPart(f32 x)
        {
            u32 ul = F32AsU32(x);
            return ((ul >> 23) & 0xFF) - 127;
        }

		inline f32 FGetMantPart(f32 x)
        {
            u32 ul = F32AsU32(x);
            return U32AsF32((ul & 0x807FFFFF) | 0x3F800000);
        }

		inline float FFloor(float arg)
		{
			return floor(arg);
		}
		
		inline float FModf(float arg, float * pIntegralPart)
		{
			double integralPart;
			float fractionalPart = modf(arg, &integralPart);
			
			*pIntegralPart = integralPart;
			
			return fractionalPart;
		}
		
		inline float FInv(register float arg)
		{
			register float tmp;
			register float ret;
			
			asm
			{
				fres ret, arg
				ps_add tmp, ret, ret
				ps_mul ret, ret, ret
				ps_nmsub ret, arg, ret, tmp
			}
			
			return ret;
		}
		
		inline f32 U16ToF32(u16 arg)
		{
			f32 ret;
			
			OSu16tof32(&arg, &ret);
			
			return ret;
		}
		
		inline u16 F32ToU16(f32 arg)
		{
			u16 ret;
			
			OSf32tou16(&arg, &ret);
			
			return ret;
		}
		
		inline f32 S16ToF32(s16 arg)
		{
			f32 ret;

			OSs16tof32(&arg, &ret);

			return ret;
		}

		inline s16 F32ToS16(f32 arg)
		{
			s16 ret;
			
			OSf32tos16(&arg, &ret);
			
			return ret;
		}
    }
}

#endif
