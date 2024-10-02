#ifndef NW4R_SND_WSD_FILE_H
#define NW4R_SND_WSD_FILE_H
#include <nw4r/snd/snd_Util.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace WsdFile {

struct WsdInfo {
    f32 pitch;                                 // at 0x0
    u8 pan;                                    // at 0x4
    u8 surroundPan;                            // at 0x5
    u8 fxSendA;                                // at 0x6
    u8 fxSendB;                                // at 0x7
    u8 fxSendC;                                // at 0x8
    u8 mainSend;                               // at 0x9
    Util::DataRef<UNKTYPE> graphEnvTableRef;   // at 0xC
    Util::DataRef<UNKTYPE> randomizerTableRef; // at 0x14
    u32 reserved;                              // at 0x1C
};

struct TrackInfo {};

struct NoteInfo {
    s32 waveIndex;                             // at 0x0
    u8 attack;                                 // at 0x4
    u8 decay;                                  // at 0x5
    u8 sustain;                                // at 0x6
    u8 release;                                // at 0x7
    u16 hold;                                  // at 0x8
    u16 padding;                               // at 0xA
    u8 originalKey;                            // at 0xC
    u8 volume;                                 // at 0xD
    u8 pan;                                    // at 0xE
    u8 surroundPan;                            // at 0xF
    f32 pitch;                                 // at 0x10
    Util::DataRef<UNKTYPE> lfoTableRef;        // at 0x14
    Util::DataRef<UNKTYPE> graphEnvTablevRef;  // at 0x1C
    Util::DataRef<UNKTYPE> randomizerTableRef; // at 0x24
    u32 reserved;                              // at 0x2C
};

typedef Util::Table<Util::DataRef<TrackInfo> > TrackTable;

typedef Util::Table<Util::DataRef<NoteInfo> > NoteTable;

struct Wsd {
    Util::DataRef<WsdInfo> refWsdInfo;       // at 0x0
    Util::DataRef<TrackTable> refTrackTable; // at 0x8
    Util::DataRef<NoteTable> refNoteTable;   // at 0x10
};

struct Header {
    ut::BinaryFileHeader fileHeader; // at 0x0
    u32 dataBlockOffset;             // at 0x10
    u32 dataBlockSize;               // at 0x14
    u32 waveBlockOffset;             // at 0x18
    u32 waveBlockSize;               // at 0x1C
};

struct DataBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    u32 wsdCount;                      // at 0x8
    Util::DataRef<Wsd> refWsd[];       // at 0xC
};

struct WaveBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    u32 waveCount;                     // at 0x8
    u32 offsetTable[];                 // at 0xC
};

// <= NW4R_VERSION(1, 0)
struct WaveBlockOld {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    u32 offsetTable[];                 // at 0x8
};

}; // namespace WsdFile

struct WaveSoundInfo {
    f32 pitch;      // at 0x0
    u8 pan;         // at 0x4
    u8 surroundPan; // at 0x5
    u8 fxSendA;     // at 0x6
    u8 fxSendB;     // at 0x7
    u8 fxSendC;     // at 0x8
    u8 mainSend;    // at 0x9
};

struct WaveSoundNoteInfo {
    s32 waveIndex;  // at 0x0
    u8 attack;      // at 0x4
    u8 decay;       // at 0x6
    u8 sustain;     // at 0x7
    u8 release;     // at 0x8
    u8 originalKey; // at 0x9
    u8 pan;         // at 0xA
    u8 surroundPan; // at 0xB
    u8 volume;      // at 0xC
    f32 pitch;      // at 0x10
};

class WsdFileReader {
public:
    static const u32 SIGNATURE = 'RWSD';
    static const int VERSION = NW4R_VERSION(1, 2);

public:
    explicit WsdFileReader(const void* pWsdBin);

    bool IsValidFileHeader(const void* pWsdBin);

    bool ReadWaveSoundInfo(WaveSoundInfo* pSoundInfo, int id) const;
    bool ReadWaveSoundNoteInfo(WaveSoundNoteInfo* pSoundNoteInfo, int id,
                               int note) const;
    bool ReadWaveParam(int id, WaveData* pWaveData,
                       const void* pWaveAddr) const;

private:
    const WsdFile::Header* mHeader;       // at 0x0
    const WsdFile::DataBlock* mDataBlock; // at 0x4
    const WsdFile::WaveBlock* mWaveBlock; // at 0x8
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
