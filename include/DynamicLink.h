#ifndef DYNAMICLINK_H
#define DYNAMICLINK_H

#include <common.h>
#include <m/m_dvd.h>
#include <rvl/OS.h>

class DbMapFile {
public:
    DbMapFile(): unk_0(0) {}
    ~DbMapFile();

    void RegisterOnDvd(const char *, const OSModuleInfo *);
    void Unregister();
private:
    UNKWORD unk_0;
};

// https://github.com/zeldaret/tp/blob/main/include/DynamicLink.h

struct DynamicModuleControlBase {
    /* 0x00 */ u16 mLinkCount;
    /* 0x02 */ u16 mDoLinkCount;
    /* 0x04 */ DynamicModuleControlBase* mPrev;
    /* 0x08 */ DynamicModuleControlBase* mNext;

    /* 0x0C */ /*vtable*/
    /* 802df100 */ virtual ~DynamicModuleControlBase();
    /* 802df340 */ virtual const char* getModuleName() const;
    /* 802df350 */ virtual int getModuleSize() const;
    /* 802df360 */ virtual const char* getModuleTypeString() const;
    /* 802df370 */ virtual void dump();
    /* 802df380 */ virtual bool do_load();
    /* 802df390 */ virtual BOOL do_load_async();
    /* 802df3a0 */ virtual bool do_unload();
    /* 802df3b0 */ virtual BOOL do_link();
    /* 802df3c0 */ virtual bool do_unlink();
    /* 802df1a0 */ DynamicModuleControlBase();
    /* 802df1f0 */ BOOL link();
    /* 802df290 */ BOOL unlink();
    /* 802df310 */ BOOL load_async();

    static inline DynamicModuleControlBase* getFirstClass() { return mFirst; }
    inline DynamicModuleControlBase* getNextClass() { return mNext; }
    bool isLinked() const { return mLinkCount != 0; }

    static DynamicModuleControlBase* mFirst;
    static DynamicModuleControlBase* mLast;
};

struct DynamicModuleControl : DynamicModuleControlBase {
    /* 802df4c0 */ virtual const char* getModuleName() const;
    /* 802df450 */ virtual ~DynamicModuleControl();
    /* 802dfb10 */ virtual int getModuleSize() const;
    /* 802dfb70 */ virtual const char* getModuleTypeString() const;
    /* 802df800 */ virtual void dump();
    /* 802df5d0 */ virtual bool do_load();
    /* 802df710 */ virtual BOOL do_load_async();
    /* 802df7a0 */ virtual bool do_unload();
    /* 802df810 */ virtual BOOL do_link();
    /* 802dfa60 */ virtual bool do_unlink();
    /* 802df3d0 */ DynamicModuleControl(char const*, EGG::ExpHeap *);
    /* 802df4d0 */ static void initialize(EGG::ExpHeap *heap);
    /* 802df530 */ static u32 callback(void*);
    void checkHeapStatus();

    /* 0x10 */ UNKWORD unk_16;
    /* 0x14 */ OSModuleHeader *mModule;
    /* 0x18 */ void *mBss;
    /* 0x1C */ UNKWORD unk_24;
    /* 0x20 */ const char *mName;
    /* 0x24 */ u8 mResourceType;
    /* 0x25 */ u8 unk_33;
    /* 0x28 */ UNKWORD unk_40;
    /* 0x2C */ mDvd_callback_c *mDvdCallbackRequest;
    /* 0x30 */ EGG::ExpHeap *mHeap;
    /* 0x34 */ DbMapFile mpRelMapFile;
    // Some of these might be members of DbMapFile
    UNKWORD unk1;
    UNKWORD unk2;
    UNKWORD unk3;
    UNKWORD unk4;

    static u32 sAllocBytes;
    static mDvd_toMainRam_base_c *sDvdFile;
    static EGG::ExpHeap *sDylinkHeap;
    static u32 sCurrentAllocatableSize;
    static u32 sTotalFreeSize;
    static EGG::Archive *sArchive;

    static const char *sRelsDir;
};

#endif
