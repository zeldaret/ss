#ifndef D_SND_FILE_MGR_H
#define D_SND_FILE_MGR_H

#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundHeap.h"

class dSndFileManager : public nw4r::snd::SoundArchivePlayer_FileManager, public nw4r::snd::detail::DisposeCallback {
private:
    struct dSndFileManagerGlob {
        u32 numFiles;
        const void **pData;
        const void **pWaveData;
    };

    static dSndFileManagerGlob sGlob;

public:
    dSndFileManager() {
        mpGlob = &sGlob;
    }
    static dSndFileManager *create(nw4r::snd::SoundArchive *pArchive, nw4r::snd::SoundHeap *pHeap);

    virtual void const *GetFileAddress(u32) override;
    virtual void const *GetFileWaveDataAddress(u32) override;

    virtual void InvalidateData(void const *pStart, void const *pEnd) override;
    virtual void InvalidateWaveData(void const *pStart, void const *pEnd) override;

    virtual ~dSndFileManager() {}

    static bool loadFileForSound(const nw4r::snd::SoundArchive &pArchive, u32 soundId, nw4r::snd::SoundHeap *pHeap);
    static bool isLoadedFileForSound(nw4r::snd::SoundArchivePlayer *pPlayer, u32 soundId);
    static bool isLoadedFileAndWaveForBank(nw4r::snd::SoundArchivePlayer *pPlayer, u32 soundId);
    static bool isLoadedFileAndWaveForSound(nw4r::snd::SoundArchivePlayer *pPlayer, u32 soundId);

private:
    static void clearGlob();

    /* 0x10 */ dSndFileManagerGlob *mpGlob;
};

#endif
