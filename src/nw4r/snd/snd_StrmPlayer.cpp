#include "nw4r/snd/snd_StrmPlayer.h"

#include "nw4r/snd/snd_TaskManager.h"
#include "nw4r/snd/snd_VoiceManager.h"
#include "nw4r/snd/snd_WaveFile.h"

#include <string.h>

namespace nw4r {
namespace snd {
namespace detail {

u8 StrmPlayer::sLoadBuffer[LOAD_BUFFER_SIZE] ALIGN_DECL(32);
OSMutex StrmPlayer::sLoadBufferMutex;

bool StrmPlayer::sStaticInitFlag = false;

StrmPlayer::StrmPlayer() : mSetupFlag(false), mActiveFlag(false), mFileStream(NULL), mVoice(NULL) {
    if (!sStaticInitFlag) {
        OSInitMutex(&sLoadBufferMutex);
        sStaticInitFlag = true;
    }

    mStrmDataLoadTaskPool.Create(mStrmDataLoadTaskArea, DATA_BLOCK_COUNT_MAX * sizeof(StrmDataLoadTask));
}

StrmPlayer::~StrmPlayer() {
    Shutdown();
}

bool StrmPlayer::Setup(StrmBufferPool *pBufferPool) {
    SoundThread::AutoLock lock;

    if (mSetupFlag) {
        Shutdown();
    }

    InitParam();
    mBufferPool = pBufferPool;
    mSetupFlag = true;

    return true;
}

void StrmPlayer::Shutdown() {
    Stop();

    SoundThread::AutoLock lock;


    if (!mSetupFlag) {
        return;
    }

    mBufferPool = NULL;
    mStrmDataLoadTaskPool.Destroy(mStrmDataLoadTaskArea, DATA_BLOCK_COUNT_MAX * sizeof(StrmDataLoadTask));

    mSetupFlag = false;
}

bool StrmPlayer::Prepare(ut::FileStream *pFileStream, int voices, StartOffsetType offsetType, int offset) {
    SoundThread::AutoLock lock;

    mFileStream = pFileStream;
    mVoiceOutCount = voices;
    mStartOffsetType = offsetType;
    mStartOffset = offset;

    mTaskErrorFlag = false;
    mTaskCancelFlag = false;
    mLoadingDelayFlag = false;
    mActiveFlag = true;
    SoundThread::GetInstance().RegisterPlayerCallback(this);

    StrmHeaderLoadTask *pTask = &mStrmHeaderLoadTask;
    pTask->strmPlayer = this;
    pTask->fileStream = mFileStream;
    pTask->startOffsetType = mStartOffsetType;
    pTask->startOffset = mStartOffset;
    TaskManager::GetInstance().AppendTask(pTask);

    return true;
}

bool StrmPlayer::Start() {
    SoundThread::AutoLock lock;

    if (!mPreparedFlag) {
        return false;
    }

    if (!mStartedFlag) {
        s32 blockIndex = 0;
        u32 blockOffset = 0;
        s32 loopCount = 0;

        if (!CalcStartOffset(&blockIndex, &blockOffset, &loopCount)) {
            return false;
        }

        mLoopCounter += loopCount;

        WaveInfo waveData;
        waveData.sampleFormat = mStrmInfo.format;
        waveData.loopFlag = true;
        waveData.numChannels = mChannelCount;
        waveData.sampleRate = mStrmInfo.sampleRate;
        waveData.loopStart = 0;

        SampleFormat format = WaveFileReader::GetAxVoiceFormatFromWaveFileFormat(mStrmInfo.format);

        waveData.loopEnd = AxVoice::GetSampleByByte(mDataBlockSize * mPlayingBufferBlockCount, format);

        for (int i = 0; i < mChannelCount; i++) {
            ChannelParam &rParam = waveData.channelParam[i];

            rParam.dataAddr = mChannels[i].bufferAddress;
            rParam.adpcmParam = mChannels[i].adpcmParam;
            rParam.adpcmLoopParam = mChannels[i].adpcmLoopParam;

            // Isn't the scale only the *bottom nibble*?
            rParam.adpcmParam.pred_scale = *static_cast<u8 *>(mChannels[i].bufferAddress);

            rParam.adpcmParam.yn1 = mAdpcmLoopYn1[i];
            rParam.adpcmParam.yn2 = mAdpcmLoopYn2[i];
        }

        ut::AutoInterruptLock lock;

        if (mVoice != NULL) {
            mVoice->Setup(waveData, blockOffset);
            mVoice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);

            if (blockIndex == mStrmInfo.numBlocks - 2) {
                UpdateDataLoopAddress(1);
            } else if (blockIndex == mStrmInfo.numBlocks - 1) {
                UpdateDataLoopAddress(0);
            }

            mVoice->Start();
            UpdatePauseStatus();
        }

        mStartedFlag = true;
    }

    return true;
}

void StrmPlayer::Stop() {
    {
        SoundThread::AutoLock lock;

        if (mVoice != NULL) {
            mVoice->Stop();
        }

        FreeChannels();

        if (mActiveFlag) {
            SoundThread::GetInstance().UnregisterPlayerCallback(this);
        }
    }

    TaskManager::GetInstance().CancelTask(&mStrmHeaderLoadTask);

    {
        ut::AutoInterruptLock lock;

        while (!mStrmDataLoadTaskList.IsEmpty()) {
            TaskManager::GetInstance().CancelTask(&mStrmDataLoadTaskList.GetBack());
        }
    }

    if (mFileStream != NULL) {
        mFileStream->Close();
        mFileStream = NULL;
    }

    mStartedFlag = false;
    mPreparedFlag = false;
    mActiveFlag = false;
}

void StrmPlayer::Pause(bool flag) {
    SoundThread::AutoLock lock;

    mPauseFlag = flag;

    if (flag) {
        mLoadWaitFlag = true;
    }

    UpdatePauseStatus();
}

void StrmPlayer::InitParam() {
    BasicPlayer::InitParam();

    mStartedFlag = false;
    mPreparedFlag = false;
    mLoadFinishFlag = false;

    mPauseFlag = false;
    mPauseStatus = false;

    mLoadWaitFlag = false;
    mNoRealtimeLoadFlag = false;

    mPlayFinishFlag = false;

    mSkipUpdateAdpcmLoop = false;
    mValidAdpcmLoop = false;

    mChannelCount = 0;
    mLoopCounter = 0;
    mVoiceOutCount = 1;

    for (int i = 0; i < CHANNEL_MAX; i++) {
        mChannels[i].bufferAddress = NULL;
        mChannels[i].bufferSize = 0;
    }
}

bool StrmPlayer::LoadHeader(ut::FileStream *pFileStream, StartOffsetType offsetType, int offset) {
    ut::detail::AutoLock<OSMutex> lock(sLoadBufferMutex);

    StrmFileLoader loader(*pFileStream);
    if (!loader.LoadFileHeader(sLoadBuffer, ut::RoundUp(LOAD_BUFFER_SIZE, 64))) {
        return false;
    }

    // StrmHeader header;
    loader.ReadStrmInfo(&mStrmInfo);

    for (int i = 0; i < mStrmInfo.numChannels; i++) {
        loader.ReadAdpcmInfo(&mChannels[i].adpcmParam, &mChannels[i].adpcmLoopParam, i);
    }

    // TODO types
    if (mStrmInfo.format == WaveFile::FORMAT_ADPCM) {
        if (offset == 0) {
            /*for (int i = 0; i < mStrmInfo.numChannels; i++) {
                header.loopYn1[i] = header.adpcmInfo[i].param.yn1;
                header.loopYn2[i] = header.adpcmInfo[i].param.yn2;
            }*/
        } else {
            int startSample;
            if (offsetType == START_OFFSET_TYPE_SAMPLE) {
                startSample = offset;
            } else if (offsetType == START_OFFSET_TYPE_MILLISEC) {
                startSample = offset * mStrmInfo.sampleRate / 1000;
            }

            s32 block = startSample / static_cast<s32>(mStrmInfo.blockSamples);

            // loader.ReadAdpcBlockData(header.loopYn1, header.loopYn2, block, mStrmInfo.numChannels);
        }
    }

    if (!SetupPlayer()) {
        return false;
    }

    mPrepareCounter = 0;
    for (int i = 0; i < mBufferBlockCountBase; i++) {
        UpdateLoadingBlockIndex();
        mPrepareCounter++;

        if (mLoadFinishFlag) {
            break;
        }
    }

    if (mStrmInfo.numBlocks <= 2 && !mStrmInfo.loopFlag) {
        SetLoopEndToZeroBuffer(mStrmInfo.numBlocks - 1);
    }

    return true;
}

bool StrmPlayer::LoadStreamData(
    ut::FileStream *pFileStream, int offset, u32 size, u32 blockSize, int blockIndex, bool needUpdateAdpcmLoop
) {
    ut::DvdFileStream *pDvdStream = ut::DynamicCast<ut::DvdFileStream *>(pFileStream);

    if (pDvdStream != NULL) {
        pDvdStream->SetPriority(DVD_PRIO_HIGH);
    }

    ut::detail::AutoLock<OSMutex> lock(sLoadBufferMutex);
    DCInvalidateRange(sLoadBuffer, size);

    pFileStream->Seek(offset, ut::FileStream::SEEKORG_BEG);
    s32 bytesRead = pFileStream->Read(sLoadBuffer, size);

    if (bytesRead != size) {
        return false;
    }

    u16 adpcmPredScale[CHANNEL_MAX];
    for (int i = 0; i < mChannelCount; i++) {
        if (needUpdateAdpcmLoop) {
            adpcmPredScale[i] = sLoadBuffer[i * ut::RoundUp(blockSize, 32) + mStrmInfo.blockHeaderOffset];
        }

        const void *pSrc =
            ut::AddOffsetToPtr(sLoadBuffer, i * ut::RoundUp(blockSize, 32) + mStrmInfo.blockHeaderOffset);

        void *pDst = ut::AddOffsetToPtr(mChannels[i].bufferAddress, mDataBlockSize * blockIndex);

        u32 len = ut::RoundUp(blockSize, 32);
        memcpy(pDst, pSrc, len);
        DCFlushRange(pDst, len);
    }

    if (needUpdateAdpcmLoop) {
        SetAdpcmLoopContext(mChannelCount, adpcmPredScale);
    }

    if (!mPreparedFlag && --mPrepareCounter == 0) {
        mPreparedFlag = true;
    }

    return true;
}

bool StrmPlayer::SetupPlayer() {
    u32 poolBlockSize = mBufferPool->GetBlockSize();

    s32 blockIndex = 0;
    u32 blockOffset = 0;
    s32 loopCount = 0;
    if (!CalcStartOffset(&blockIndex, &blockOffset, &loopCount)) {
        return false;
    }
    /*
    if (mStrmInfo.format == WaveFile::FORMAT_ADPCM) {
        for (int i = 0; i < mStrmInfo.numChannels; i++) {
            mChannels[i].adpcmParam = mStrmInfo.adpcmInfo[i];
            mAdpcmLoopYn1[i] = pStrmHeader->loopYn1[i];
            mAdpcmLoopYn2[i] = pStrmHeader->loopYn2[i];
        }
    }
    */

    mLoopStartBlockIndex = mStrmInfo.loopStart / mStrmInfo.blockSamples;
    mLastBlockIndex = mStrmInfo.numBlocks - 1;

    mDataBlockSize = mStrmInfo.blockSize;
    if (mDataBlockSize > DATA_BLOCK_SIZE_MAX) {
        return false;
    }

    mBufferBlockCount = poolBlockSize / mDataBlockSize;
    if (mBufferBlockCount < DATA_BLOCK_COUNT_MIN) {
        return false;
    } else if (mBufferBlockCount > DATA_BLOCK_COUNT_MAX) {
        mBufferBlockCount = DATA_BLOCK_COUNT_MAX;
    }

    mBufferBlockCountBase = mBufferBlockCount - 1;
    mChangeNumBlocks = mBufferBlockCountBase;

    mPlayingDataBlockIndex = blockIndex;
    mLoadingDataBlockIndex = blockIndex;

    mLoadingBufferBlockIndex = 0;
    mPlayingBufferBlockIndex = 0;

    if (mNoRealtimeLoadFlag) {
        mLoadingBufferBlockCount = mStrmInfo.numBlocks;
    } else {
        mLoadingBufferBlockCount = CalcLoadingBufferBlockCount();
    }

    mPlayingBufferBlockCount = mLoadingBufferBlockCount;

    ut::AutoInterruptLock lock;
    mChannelCount = ut::Min<int>(mStrmInfo.numChannels, CHANNEL_MAX);
    if (!AllocChannels(mChannelCount, mVoiceOutCount)) {
        return false;
    }

    return true;
}

bool StrmPlayer::AllocChannels(int channels, int voices) {
    ut::AutoInterruptLock lock;

    for (int i = 0; i < channels; i++) {
        void *pBuffer = mBufferPool->Alloc();

        if (pBuffer == NULL) {
            for (int j = 0; j < i; j++) {
                mBufferPool->Free(mChannels[j].bufferAddress);
            }

            return false;
        }

        mChannels[i].bufferAddress = pBuffer;
        mChannels[i].bufferSize = mBufferPool->GetBlockSize();
    }

    Voice *pVoice =
        VoiceManager::GetInstance().AllocVoice(channels, voices, Voice::PRIORITY_MAX, VoiceCallbackFunc, this);

    if (pVoice == NULL) {
        for (int i = 0; i < channels; i++) {
            mBufferPool->Free(mChannels[i].bufferAddress);
        }

        return false;
    }

    mVoice = pVoice;
    return true;
}

void StrmPlayer::FreeChannels() {
    ut::AutoInterruptLock lock;

    for (int i = 0; i < mChannelCount; i++) {
        if (mChannels[i].bufferAddress != NULL) {
            mBufferPool->Free(mChannels[i].bufferAddress);

            mChannels[i].bufferAddress = NULL;
            mChannels[i].bufferSize = 0;
        }
    }

    mChannelCount = 0;

    if (mVoice != NULL) {
        mVoice->Free();
        mVoice = NULL;
    }
}

void StrmPlayer::Update() {
    if (!mActiveFlag) {
        return;
    }

    if (mTaskErrorFlag && !mTaskCancelFlag) {
        Stop();
        return;
    }

    if (mStartedFlag && mVoice == NULL) {
        Stop();
        return;
    }

    if (mLoadWaitFlag && mStrmDataLoadTaskList.IsEmpty()) {
        mLoadWaitFlag = false;
        UpdatePauseStatus();
    }

    if (mLoadingDelayFlag) {
        mLoadingDelayFlag = false;
    }

    if (mVoice != NULL) {
        f32 volume = 1.0f;
        volume *= GetVolume();

        f32 pitchRatio = 1.0f;
        pitchRatio *= GetPitch();

        f32 pan = 0.0f;
        pan += GetPan();

        f32 surroundPan = 0.0f;
        surroundPan += GetSurroundPan();

        f32 lpfFreq = 1.0f;
        lpfFreq += GetLpfFreq();

        int remoteFilter = 0;
        remoteFilter += GetRemoteFilter();

        f32 mainSend = 0.0f;
        mainSend += GetMainSend();

        f32 fxSend[AUX_BUS_NUM];
        for (int i = 0; i < AUX_BUS_NUM; i++) {
            fxSend[i] = 0.0f;
            fxSend[i] += GetFxSend(static_cast<AuxBus>(i));
        }

        f32 remoteOutVolume[WPAD_MAX_CONTROLLERS];
        f32 remoteSend[WPAD_MAX_CONTROLLERS];
        f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            remoteOutVolume[i] = GetRemoteOutVolume(i);

            remoteSend[i] = 0.0f;
            remoteSend[i] += GetRemoteSend(i);

            remoteFxSend[i] = 0.0f;
            remoteFxSend[i] += GetRemoteFxSend(i);
        }

        ut::AutoInterruptLock lock;

        if (mVoice != NULL) {
            mVoice->SetVolume(volume);
            mVoice->SetPitch(pitchRatio);
            mVoice->SetPan(pan);
            mVoice->SetSurroundPan(surroundPan);
            mVoice->SetLpfFreq(lpfFreq);
            mVoice->SetRemoteFilter(remoteFilter);
            mVoice->SetOutputLine(GetOutputLine());
            mVoice->SetMainOutVolume(GetMainOutVolume());
            mVoice->SetMainSend(mainSend);

            for (int i = 0; i < AUX_BUS_NUM; i++) {
                mVoice->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
            }

            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                mVoice->SetRemoteOutVolume(i, remoteOutVolume[i]);
            }
        }
    }
}

void StrmPlayer::UpdateBuffer() {
    if (mStartedFlag && mVoice != NULL) {
        if (!mPlayFinishFlag && !mNoRealtimeLoadFlag && !mLoadWaitFlag) {
            u32 sample = mVoice->GetCurrentPlayingSample();
            int block = sample / mStrmInfo.blockSamples;

            while (mPlayingBufferBlockIndex != block) {
                if (!mLoadWaitFlag && mStrmDataLoadTaskList.GetSize() >= mBufferBlockCountBase - 2) {
                    mLoadingDelayFlag = true;
                    mLoadWaitFlag = true;
                    UpdatePauseStatus();
                    break;
                }

                UpdatePlayingBlockIndex();
                UpdateLoadingBlockIndex();
            }
        }
    }
}

void StrmPlayer::UpdateLoopAddress(u32 startSample, u32 endSample) {
    ut::AutoInterruptLock lock;

    for (int i = 0; i < mChannelCount; i++) {
        mVoice->SetLoopStart(i, mChannels[i].bufferAddress, startSample);
        mVoice->SetLoopEnd(i, mChannels[i].bufferAddress, endSample);
    }

    mVoice->SetLoopFlag(true);
}

void StrmPlayer::UpdatePlayingBlockIndex() {
    mPlayingDataBlockIndex++;

    if (mPlayingDataBlockIndex > mLastBlockIndex && mStrmInfo.loopFlag) {
        mPlayingDataBlockIndex = mLoopStartBlockIndex;

        if (mLoopCounter < INT_MAX) {
            mLoopCounter++;
        }

        UpdateLoopAddress(0, mPlayingBufferBlockCount * mStrmInfo.blockSamples);
    }

    mPlayingBufferBlockIndex++;

    if (mPlayingBufferBlockIndex >= mPlayingBufferBlockCount) {
        mPlayingBufferBlockIndex = 0;
        mPlayingBufferBlockCount = mLoadingBufferBlockCount;

        UpdateLoopAddress(0, mPlayingBufferBlockCount * mStrmInfo.blockSamples);
    }

    if (mPlayingBufferBlockIndex == mPlayingBufferBlockCount - 1 && mVoice->GetFormat() == SAMPLE_FORMAT_DSP_ADPCM) {
        if (!mSkipUpdateAdpcmLoop && mValidAdpcmLoop) {
            ut::AutoInterruptLock lock;

            for (int i = 0; i < mChannelCount; i++) {
                AdpcmLoopParam loopParam;
                loopParam.loop_pred_scale = mAdpcmLoopPredScale[i];
                loopParam.loop_yn1 = 0;
                loopParam.loop_yn2 = 0;

                mVoice->SetAdpcmLoop(i, &loopParam);
            }

            mVoice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);
        }

        mValidAdpcmLoop = false;
        mSkipUpdateAdpcmLoop = false;
    }

    if (mPlayingDataBlockIndex == mLastBlockIndex - 1) {
        UpdateDataLoopAddress(mPlayingBufferBlockIndex + 1);
    }
}

void StrmPlayer::UpdateDataLoopAddress(s32 endBlock) {
    if (mStrmInfo.loopFlag) {
        s32 startBlock = endBlock + 1;
        if (startBlock >= mPlayingBufferBlockCount) {
            startBlock -= mPlayingBufferBlockCount;
        }

        ut::AutoInterruptLock lock;

        UpdateLoopAddress(
            startBlock * mStrmInfo.blockSamples, mStrmInfo.lastBlockSamples + (endBlock * mStrmInfo.blockSamples)
        );

        if (mStrmInfo.format == WaveFile::FORMAT_ADPCM) {
            if (mVoice->GetFormat() == SAMPLE_FORMAT_DSP_ADPCM) {
                mVoice->SetVoiceType(AxVoice::VOICE_TYPE_NORMAL);

                for (int i = 0; i < mChannelCount; i++) {
                    mVoice->SetAdpcmLoop(i, &mChannels[i].adpcmLoopParam);
                }
            }

            if (endBlock == mPlayingBufferBlockCount - 1) {
                mSkipUpdateAdpcmLoop = true;
            }
        }

        return;
    }

    SetLoopEndToZeroBuffer(endBlock);
}

void StrmPlayer::SetLoopEndToZeroBuffer(int endBlock) {
    {
        ut::AutoInterruptLock lock;

        for (int i = 0; i < mChannelCount; i++) {
            mVoice->StopAtPoint(
                i, mChannels[i].bufferAddress, mStrmInfo.lastBlockSamples + (endBlock * mStrmInfo.blockSamples)
            );
        }
    }

    mPlayFinishFlag = true;
}

void StrmPlayer::UpdateLoadingBlockIndex() {
    if (mLoadFinishFlag) {
        return;
    }

    u32 blockSize = mLoadingDataBlockIndex < static_cast<s32>(mStrmInfo.numBlocks - 1) ? mStrmInfo.blockSize :
                                                                                         mStrmInfo.lastBlockPaddedSize;

    u32 loadSize = mStrmInfo.blockHeaderOffset + mChannelCount * ut::RoundUp(blockSize, 32);

    s32 loadOffset = mStrmInfo.dataOffset + mLoadingDataBlockIndex * (mStrmInfo.blockHeaderOffset +
                                                                      mStrmInfo.blockSize * mStrmInfo.numChannels);

    bool needUpdateAdpcmLoop = mLoadingBufferBlockIndex == 0 && mStrmInfo.format == WaveFile::FORMAT_ADPCM;

    StrmDataLoadTask *pTask = mStrmDataLoadTaskPool.Alloc();
    pTask->strmPlayer = this;
    pTask->fileStream = mFileStream;
    pTask->size = loadSize;
    pTask->offset = loadOffset;
    pTask->blockSize = blockSize;
    pTask->bufferBlockIndex = mLoadingBufferBlockIndex;
    pTask->needUpdateAdpcmLoop = needUpdateAdpcmLoop;

    ut::AutoInterruptLock lock;
    mStrmDataLoadTaskList.PushBack(pTask);

    TaskManager::GetInstance().AppendTask(
        pTask, mStartedFlag ? TaskManager::PRIORITY_HIGH : TaskManager::PRIORITY_MIDDLE
    );

    mLoadingDataBlockIndex++;

    if (mLoadingDataBlockIndex > mLastBlockIndex) {
        if (mStrmInfo.loopFlag) {
            mLoadingDataBlockIndex = mLoopStartBlockIndex;
        } else {
            mLoadFinishFlag = true;
            return;
        }
    }

    mLoadingBufferBlockIndex++;

    if (mLoadingBufferBlockIndex >= mLoadingBufferBlockCount) {
        mLoadingBufferBlockIndex = 0;
        mLoadingBufferBlockCount = CalcLoadingBufferBlockCount();
    }
}

void StrmPlayer::UpdatePauseStatus() {
    ut::AutoInterruptLock lock;

    bool paused = false;

    if (mPauseFlag) {
        paused = true;
    }

    if (mLoadWaitFlag) {
        paused = true;
    }

    if (paused != mPauseStatus) {
        if (mVoice != NULL) {
            mVoice->Pause(paused);
        }

        mPauseStatus = paused;
    }
}

int StrmPlayer::CalcLoadingBufferBlockCount() const {
    int restBlocks = (mLastBlockIndex - mLoadingDataBlockIndex) + 1;
    int loopBlocks = (mLastBlockIndex - mLoopStartBlockIndex) + 1;

    if ((mBufferBlockCountBase + 1 - restBlocks) % loopBlocks == 0) {
        return mBufferBlockCountBase + 1;
    }

    return mBufferBlockCountBase;
}

bool StrmPlayer::CalcStartOffset(s32 *pBlockIndex, u32 *pBlockOffset, s32 *pLoopCount) {
    if (mStrmInfo.blockSamples == 0) {
        return false;
    }

    int startSample;
    if (mStartOffsetType == START_OFFSET_TYPE_SAMPLE) {
        startSample = mStartOffset;
    } else if (mStartOffsetType == START_OFFSET_TYPE_MILLISEC) {
        startSample = (mStartOffset * static_cast<s64>(mStrmInfo.sampleRate)) / 1000;
    }

    *pLoopCount = 0;

    if (startSample >= mStrmInfo.loopEnd) {
        if (mStrmInfo.loopFlag) {
            s32 loopStart = mStrmInfo.loopStart;
            s32 loopEnd = mStrmInfo.loopEnd;
            s32 loopLength = loopEnd - loopStart;

            s32 startOffset2 = startSample - loopEnd;
            *pLoopCount = startOffset2 / loopLength + 1;

            s32 startLoop = startOffset2 / loopLength;
            startSample = loopStart + (startOffset2 - (startLoop * loopLength));
        } else {
            return false;
        }
    }

    *pBlockIndex = startSample / static_cast<s32>(mStrmInfo.blockSamples);

    if (mStrmInfo.format == WaveFile::FORMAT_ADPCM) {
        s32 startBlock = startSample / mStrmInfo.blockSamples;
        *pBlockOffset = startSample - (startBlock * mStrmInfo.blockSamples);
    }

    return true;
}

void StrmPlayer::VoiceCallbackFunc(Voice *pDropVoice, Voice::VoiceCallbackStatus status, void *pCallbackArg) {
    StrmPlayer *pStrmPlayer = static_cast<StrmPlayer *>(pCallbackArg);
    ut::AutoInterruptLock lock;

    switch (status) {
        case Voice::CALLBACK_STATUS_FINISH_WAVE:
        case Voice::CALLBACK_STATUS_CANCEL:      {
            pDropVoice->Free();
            pStrmPlayer->mVoice = NULL;
            break;
        }

        case Voice::CALLBACK_STATUS_DROP_VOICE:
        case Voice::CALLBACK_STATUS_DROP_DSP:   {
            pStrmPlayer->mVoice = NULL;
            break;
        }

        default: {
            return;
        }
    }
}

void StrmPlayer::SetAdpcmLoopContext(int channels, u16 *pPredScale) {
    if (mStrmInfo.format != WaveFile::FORMAT_ADPCM) {
        return;
    }

    for (int i = 0; i < channels && i < CHANNEL_MAX; i++) {
        mAdpcmLoopPredScale[i] = pPredScale[i];
    }

    mValidAdpcmLoop = true;
}

StrmPlayer::StrmHeaderLoadTask::StrmHeaderLoadTask() : strmPlayer(NULL), fileStream(NULL), startOffset(0) {}

void StrmPlayer::StrmHeaderLoadTask::Execute() {
    if (!strmPlayer->LoadHeader(fileStream, startOffsetType, startOffset)) {
        strmPlayer->SetTaskErrorFlag();
    }
}

void StrmPlayer::StrmHeaderLoadTask::Cancel() {}

void StrmPlayer::StrmHeaderLoadTask::OnCancel() {
    strmPlayer->SetTaskCancelFlag();

    if (fileStream != NULL && fileStream->CanCancel()) {
        if (fileStream->CanAsync()) {
            fileStream->CancelAsync(NULL, NULL);
        } else {
            fileStream->Cancel();
        }
    }
}

StrmPlayer::StrmDataLoadTask::StrmDataLoadTask()
    : strmPlayer(NULL), fileStream(NULL), size(0), offset(0), blockSize(0), bufferBlockIndex(-1),
      needUpdateAdpcmLoop(false) {}

void StrmPlayer::StrmDataLoadTask::Execute() {
    if (!strmPlayer->LoadStreamData(fileStream, offset, size, blockSize, bufferBlockIndex, needUpdateAdpcmLoop)) {
        strmPlayer->SetTaskErrorFlag();
    }

    ut::AutoInterruptLock lock;
    strmPlayer->mStrmDataLoadTaskList.Erase(this);
    strmPlayer->mStrmDataLoadTaskPool.Free(this);
}

void StrmPlayer::StrmDataLoadTask::Cancel() {
    ut::AutoInterruptLock lock;
    strmPlayer->mStrmDataLoadTaskList.Erase(this);
    strmPlayer->mStrmDataLoadTaskPool.Free(this);
}

void StrmPlayer::StrmDataLoadTask::OnCancel() {
    strmPlayer->SetTaskCancelFlag();

    if (fileStream != NULL && fileStream->CanCancel()) {
        if (fileStream->CanAsync()) {
            fileStream->CancelAsync(NULL, NULL);
        } else {
            fileStream->Cancel();
        }
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r
