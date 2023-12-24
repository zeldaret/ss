#ifndef NW4R_G3D_RESFILE_H
#define NW4R_G3D_RESFILE_H
#include "ut_binaryFileFormat.h"
#include "g3d_rescommon.h"
#include "g3d_resdict.h"
#include "types_nw4r.h"

namespace nw4r
{
	namespace g3d
	{
		
		struct ResFileData
		{
			ut::BinaryFileHeader mHeader; // at 0x0
			
			ut::BinaryBlockHeader mRootHeader; // at 0x10
			ResDicData mRootData; // at 0x18
		};
		
		struct ResFile
		{
			ResCommon<ResFileData> mFile; // at 0x0
			
			inline ResFileData & ref() const
			{
				return mFile.ref();
			}
			
			ResMdl GetResMdl(const char *) const;
			ResMdl GetResMdl(int) const;
			ResMdl GetResMdl(u32) const; //inlined
			
			ResPltt GetResPltt(const char *) const;
			ResPltt GetResPltt(ResName) const;
			ResPltt GetResPltt(int) const;
			ResPltt GetResPltt(u32) const; //inlined
			
			ResTex GetResTex(const char *) const;
			ResTex GetResTex(ResName) const;
			ResTex GetResTex(int) const;
			ResTex GetResTex(u32) const; //inlined
			
			ResAnmChr GetResAnmChr(const char *) const;
			ResAnmChr GetResAnmChr(int) const;
			ResAnmChr GetResAnmChr(u32) const; //inlined
			
			ResAnmVis GetResAnmVis(const char *) const;
			ResAnmVis GetResAnmVis(int) const;
			ResAnmVis GetResAnmVis(u32) const; //inlined
			
			ResAnmClr GetResAnmClr(const char *) const;
			ResAnmClr GetResAnmClr(int) const;
			ResAnmClr GetResAnmClr(u32) const; //inlined
			
			ResAnmTexPat GetResAnmTexPat(const char *) const;
			ResAnmTexPat GetResAnmTexPat(int) const;
			ResAnmTexPat GetResAnmTexPat(u32) const; //inlined
			
			ResAnmTexSrt GetResAnmTexSrt(const char *) const;
			ResAnmTexSrt GetResAnmTexSrt(int) const;
			ResAnmTexSrt GetResAnmTexSrt(u32) const; //inlined
			
			ResAnmShp GetResAnmShp(const char *) const;
			ResAnmShp GetResAnmShp(int) const;
			ResAnmShp GetResAnmShp(u32) const; //inlined
			
			ResAnmScn GetResAnmScn(const char *) const;
			ResAnmScn GetResAnmScn(int) const;
			ResAnmScn GetResAnmScn(u32) const; //inlined
			
			u32 GetResMdlNumEntries() const; //inlined
			u32 GetResPlttNumEntries() const; //inlined
			u32 GetResTexNumEntries() const; //inlined
			u32 GetResAnmChrNumEntries() const; //inlined
			u32 GetResAnmVisNumEntries() const; //inlined
			u32 GetResAnmClrNumEntries() const; //inlined
			u32 GetResAnmTexPatNumEntries() const; //inlined
			u32 GetResAnmTexSrtNumEntries() const; //inlined
			u32 GetResAnmShpNumEntries() const; //inlined
			u32 GetResAnmScnNumEntries() const;
			
			bool Bind(ResFile);
			void Release();
			void Init();
			void Terminate();
			bool CheckRevision() const;
		};
	}
}

#endif