#ifndef NW4R_LYT_ANIMATION_H
#define NW4R_LYT_ANIMATION_H
#include "types_nw4r.h"
#include "ut_LinkList.h"
#include "lyt_resourceAccessor.h"
#include "lyt_common.h"

namespace nw4r
{
    namespace lyt
    {
        namespace res
        {
            struct AnimationBlock
            {
                
            };
        }

        struct AnimationBlock
        {
            res::DataBlockHeader header; // at 0x0
            u16 frameSize; // at 0x8
            bool loop; // at 0xA
            // . . .
        };

        struct AnimTransform
        {
            inline AnimTransform() : mNode(), mResource(NULL), mFrame(0.0f) {}

            u16 GetFrameSize() const;
            bool IsLoopData() const;

            ut::LinkListNode mNode; // at 0x0
            AnimationBlock *mResource; // at 0xC
            f32 mFrame; // at 0x10
        };

        struct AnimTransformBasic : AnimTransform
        {
            AnimTransformBasic();
            virtual ~AnimTransformBasic();
            virtual UNKTYPE SetResource(const res::AnimationBlock *, ResourceAccessor *);
            virtual UNKTYPE Bind(Pane *, bool);
            virtual UNKTYPE Bind(Material *);
            virtual UNKTYPE Animate(u32, Pane *);
            virtual UNKTYPE Animate(u32, Material *);

            u32 INT_0x14;
            u32 INT_0x18;
            u16 INT_0x1C;
        };

        struct AnimationLink
        {
            inline AnimationLink() : mNode()
            {
                Reset();
            }

            inline void Reset()
            {
                Set(NULL, 0, false);
            }

            inline void Set(AnimTransform *pTransform, u16 us, bool b)
            {
                mTransform = pTransform;
                SHORT_0xC = us;
                BOOL_0xE = b;
            }

            ut::LinkListNode mNode; // at 0x0
            AnimTransform *mTransform; // at 0x8
            u16 SHORT_0xC;
            bool BOOL_0xE;
        };
    }
}

#endif