#ifndef NW4R_G3D_RESNODE_H
#define NW4R_G3D_RESNODE_H
#include "types_nw4r.h"
#include "g3d_rescommon.h"
#include "g3d_anmchr.h"
#include "math_types.h"

namespace nw4r
{
    namespace g3d
    {
        struct ResNodeData
        {
            u32 INT_0x0;
            s32 INT_0x4;
            u16 SHORT_0x8;
            u16 SHORT_0xA;
            UNKWORD WORD_0xC;
            UNKWORD WORD_0x10;
            u32 mFlags; // at 0x14
            UNKWORD WORD_0x18;
            UNKWORD WORD_0x1C;
            math::VEC3 VEC3_0x20;
            math::VEC3 VEC3_0x2C;
            f32 FLOAT_0x38;
            f32 FLOAT_0x3C;
            f32 FLOAT_0x40;
            // . . .
        };

        struct ResNode
        {
            enum ResNodeFlags
            {
                NODE_IS_VISIBLE = 0x100
            };

            ResCommon<ResNodeData> mNode;

            inline ResNode(void * vptr) : mNode(vptr) {}
            
            bool IsValid() const { return mNode.IsValid(); }

            UNKWORD GetID() const
            {
                if (IsValid())
                    return mNode.ptr()->WORD_0xC;
                return 0;
            }

            void SetVisibility(bool visible)
            {
                if (IsValid())
                {
                    if (visible)
                        mNode.ptr()->mFlags |= NODE_IS_VISIBLE;
                    else
                        mNode.ptr()->mFlags &= ~NODE_IS_VISIBLE;
                }
            }

            void PatchChrAnmResult(ChrAnmResult *) const;
            void CalcChrAnmResult(ChrAnmResult *) const;
        };
    }
}

#endif