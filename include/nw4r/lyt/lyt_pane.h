#ifndef NW4R_LYT_PANE_H
#define NW4R_LYT_PANE_H
#include "types_nw4r.h"
#include "math_types.h"
#include "ut_LinkList.h"
#include "ut_RuntimeTypeInfo.h"
#include "ut_Color.h"
#include "ut_algorithm.h"
#include "lyt_animation.h"
#include <string.h>

#define PANE_NAME_SIZE 17
#define PANE_USERDATA_SIZE 9
#define ANIMTARGET_PANE_MAX 10

namespace nw4r
{
    namespace lyt
    {
        void LytInit();

        struct Size
        {
            inline Size() : x(), y() {}

            inline Size& operator=(const Size& rhs)
            {
                x = rhs.x;
                y = rhs.y;
                return *this;
            }

            f32 x, y;
        };

        namespace detail
        {
            struct PaneBase
            {
                inline PaneBase() : mNode() {}

                virtual ~PaneBase();
                ut::LinkListNode mNode; // at 0x4
            };
        }

        namespace res
        {
            struct Pane
            {
                u32 mMagic; // "pan1", at 0x0
                u32 mLength; // at 0x4
                u8 BYTE_0x8;
                u8 BYTE_0x9;
                u8 BYTE_0xA;
                u8 BYTE_0xB; // NO REFERENCES
                char mName[PANE_NAME_SIZE]; // at 0xC
                char mUserData[PANE_USERDATA_SIZE]; // at 0x1C
                math::VEC3 VEC3_0x24;
                math::VEC3 VEC3_0x30;
                math::VEC2 VEC2_0x3C;
                Size mSize; // at 0x44
            };
        }

        struct Pane : detail::PaneBase
        {
            // For use with TestBit<Uc>
            enum PaneBits
            {
                VISIBLE,
                INFLUENCED_ALPHA,
                LOCATION_ADJUST
            };

            inline void Init()
            {
                mParent = NULL;
                mMaterial = NULL;
                BYTE_0xD6 = 0;
                INT_0xB4 = 0;
            }

            inline void SetName(const char *pName)
            {
                strncpy(mName, pName, PANE_NAME_SIZE);
                mName[PANE_NAME_SIZE - 1] = '\0';
            }

            inline void SetUserData(const char *pData)
            {
                strncpy(mUserData, pData, PANE_USERDATA_SIZE);
                mUserData[PANE_USERDATA_SIZE - 1] = '\0';
            }

            inline void InsertChild(ut::LinkList<lyt::Pane, 4>::Iterator iter, lyt::Pane *pChild)
            {
                mChildren.Insert(iter, pChild);
                pChild->mParent = this;
            }

            Pane(const res::Pane *);
            virtual ~Pane(); // at 0x8
            virtual const ut::detail::RuntimeTypeInfo * GetRuntimeTypeInfo() const; // at 0xC
            virtual UNKTYPE CalculateMtx(const DrawInfo&); // at 0x10
            virtual UNKTYPE Draw(const DrawInfo&); // at 0x14
            virtual void DrawSelf(const DrawInfo&); // at 0x18
            virtual UNKTYPE Animate(u32); // at 0x1C
            virtual UNKTYPE AnimateSelf(u32); // at 0x20
            virtual ut::Color GetVtxColor(u32) const; // at 0x24
            virtual void SetVtxColor(u32, ut::Color); // at 0x28
            virtual UNKTYPE GetColorElement(u32) const; // at 0x2C
            virtual UNKTYPE SetColorElement(u32, u8); // at 0x30
            virtual u8 GetVtxColorElement(u32) const; // at 0x34
            virtual void SetVtxColorElement(u32, u8); // at 0x38
            virtual Pane * FindPaneByName(const char *, bool); // at 0x3C
            virtual UNKTYPE FindMaterialByName(const char *, bool); // at 0x40
            virtual UNKTYPE BindAnimation(AnimTransform *, bool); // at 0x44
            virtual UNKTYPE UnbindAnimation(AnimTransform *, bool); // at 0x48
            virtual UNKTYPE UnbindAllAnimation(bool); // at 0x4C
            virtual UNKTYPE UnbindAnimationSelf(AnimTransform *); // at 0x50
            virtual UNKTYPE FindAnimationLink(AnimTransform *); // at 0x54
            virtual UNKTYPE SetAnimationEnable(AnimTransform *, bool, bool); // at 0x58
            virtual UNKTYPE GetMaterial() const; // at 0x5C
            virtual UNKTYPE LoadMtx(const DrawInfo&); // at 0x60
            void AppendChild(Pane *);
            void RemoveChild(Pane *);
            UNKTYPE GetPaneRect(const DrawInfo&) const;
            UNKTYPE AddAnimationLink(AnimationLink *);
            UNKTYPE GetVtxPos() const;
            
            ut::LinkListNode mNode; // at 0x4
            Pane *mParent; // at 0x8
            ut::LinkList<Pane, 4> mChildren; // at 0x14
            ut::LinkList<AnimationLink, 0> mAnims; // at 0x20;
            Material *mMaterial; // at 0x2C
            math::VEC3 mTranslate; // at 0x30
            math::VEC3 VEC3_0x3C;
            math::VEC2 mScale; // at 0x48
            Size mSize; // at 0x50
            math::MTX34 MTX34_0x58;
            math::MTX34 mGlobalMtx; // at 0x84
            int INT_0xB4;
            u8 mAlpha; // at 0xB8
            u8 BYTE_0xB9;
            u8 BYTE_0xBA;
            u8 mFlags; // at 0xBB
            char mName[PANE_NAME_SIZE]; // at 0xBC
            char mUserData[PANE_USERDATA_SIZE]; // at 0xCD
            u8 BYTE_0xD6;

            static ut::detail::RuntimeTypeInfo typeInfo;
        };
    }
}

#endif