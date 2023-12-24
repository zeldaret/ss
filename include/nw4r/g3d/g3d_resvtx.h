#ifndef NW4R_G3D_RESVTX_H
#define NW4R_G3D_RESVTX_H
#include <GXAttr.h>
#include "g3d_rescommon.h"

namespace nw4r
{
	namespace g3d
	{
		struct ResVtxPosData
		{
			u32 SIZE_0x0;
			char UNK_0x4[0x4];
			u32 mDataOffset; // at 0x8
			char UNK_0xC[0x11];
			u8 BYTE_0x1D;
		};
		
		struct ResVtxNrmData
		{
			u32 SIZE_0x0;
			char UNK_0x4[0x4];
			u32 mDataOffset; // at 0x8
			char UNK_0xC[0x11];
			u8 BYTE_0x1D;
		};
		
		struct ResVtxClrData
		{
			u32 SIZE_0x0;
			char UNK_0x4[0x4];
			u32 mDataOffset; // at 0x8
			char UNK_0xC[0x10];
			u8 BYTE_0x1C;
		};
		
		struct ResVtxTexCoordData
		{
			u32 SIZE_0x0;
			char UNK_0x4[0x4];
			u32 mDataOffset; // at 0x8
			char UNK_0xC[0x11];
			u8 BYTE_0x1D;
		};
		
		struct ResVtxPos
		{
			ResCommon<ResVtxPosData> mPos;
			
			inline ResVtxPos(void * vptr) : mPos(vptr) {}
			
			inline ResVtxPosData & ref() const
			{
				return mPos.ref();
			}
			
			inline const void * GetData() const
			{
				return mPos.ofs_to_ptr<void>(ref().mDataOffset);
			}
			
			inline UNKTYPE Init()
			{
				DCStore(false);
			}

			void SetArray();
			void GetArray(const void **, u8 *) const;
			UNKTYPE CopyTo(void *) const;
			
			UNKTYPE DCStore(bool);
		};
		
		struct ResVtxNrm
		{
			ResCommon<ResVtxNrmData> mNrm;
			
			inline ResVtxNrm(void * vptr) : mNrm(vptr) {}
			
			inline ResVtxNrmData & ref() const
			{
				return mNrm.ref();
			}
			
			inline const void * GetData() const
			{
				return mNrm.ofs_to_ptr<void>(ref().mDataOffset);
			}
			
			inline UNKTYPE Init()
			{
				DCStore(false);
			}
	
			void SetArray();
			void GetArray(const void **, u8 *) const;
			UNKTYPE CopyTo(void *) const;
			
			UNKTYPE DCStore(bool);
		};
		
		struct ResVtxClr
		{
			ResCommon<ResVtxClrData> mClr;
			
			inline ResVtxClr(void * vptr) : mClr(vptr) {}
			
			inline ResVtxClrData & ref() const
			{
				return mClr.ref();
			}
			
			inline const void * GetData() const
			{
				return mClr.ofs_to_ptr<void>(ref().mDataOffset);
			}

			inline UNKTYPE Init()
			{
				DCStore(false);
			}

			void SetArray(GXAttr);
			void GetArray(const void **, u8 *) const;
			UNKTYPE CopyTo(void *) const;
			
			UNKTYPE DCStore(bool);
		};
		
		struct ResVtxTexCoord
		{
			ResCommon<ResVtxTexCoordData> mTexCoord;
			
			inline ResVtxTexCoord(void * vptr) : mTexCoord(vptr) {}
			
			inline ResVtxTexCoordData & ref() const
			{
				return mTexCoord.ref();
			}
			
			inline const void * GetData() const
			{
				return mTexCoord.ofs_to_ptr<void>(ref().mDataOffset);
			}
			
			inline UNKTYPE Init()
			{
				DCStore(false);
			}

			void GetArray(const void **, u8 *) const;
			
			UNKTYPE DCStore(bool);
		};
	}
}

#endif