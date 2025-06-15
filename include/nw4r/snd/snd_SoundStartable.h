#ifndef NW4R_SND_SOUND_STARTABLE_H
#define NW4R_SND_SOUND_STARTABLE_H

/*******************************************************************************
 * headers
 */

#include "common.h" // u32
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_ExternalSoundPlayer.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { class SoundHandle; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x289cb
	class SoundStartable
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x267f0
		enum StartResult
		{
			START_SUCCESS,

			START_ERR_LOW_PRIORITY,
			START_ERR_INVALID_LABEL_STRING,
			START_ERR_INVALID_SOUNDID,
			START_ERR_NOT_DATA_LOADED,
			START_ERR_NOT_ENOUGH_PLAYER_HEAP,
			START_ERR_CANNOT_OPEN_FILE,
			START_ERR_NOT_AVAILABLE,
			START_ERR_CANNOT_ALLOCATE_TRACK,
			START_ERR_NOT_ENOUGH_INSTANCE,
			START_ERR_INVALID_PARAMETER,
			START_ERR_INVALID_SEQ_START_LOCATION_LABEL,

			START_ERR_USER = 128,

			START_ERR_UNKNOWN = 255,
		};

	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x27291
		struct StartInfo
		{
		StartInfo() : enableFlag(0) {}
		// enums
		public:
			enum EnableFlag
			{
				ENABLE_START_OFFSET		= 1 << 0,
				ENABLE_PLAYER_ID		= 1 << 1,
				ENABLE_PLAYER_PRIORITY	= 1 << 2,
				ENABLE_ACTOR_PLAYER_ID	= 1 << 3,
				ENABLE_SEQ_SOUND_INFO	= 1 << 4,
			};

			// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x27187
			enum StartOffsetType
			{
				START_OFFSET_TYPE_MILLISEC,
				START_OFFSET_TYPE_TICK,
				START_OFFSET_TYPE_SAMPLE,
			};

			// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x27208
			struct SeqSoundInfo
			{
				SeqSoundInfo() : seqDataAddress(NULL), startLocationLabel(NULL) {}
				void	*seqDataAddress;			// size 0x04, offset 0x00
				char	const *startLocationLabel;	// size 0x04, offset 0x04
			}; // size 0x08

		// members
		public:
			u32				enableFlag;			// size 0x04, offset 0x00
			StartOffsetType	startOffsetType;	// size 0x04, offset 0x04
			int				startOffset;		// size 0x04, offset 0x08
			u32				playerId;			// size 0x04, offset 0x0c
			int				playerPriority;		// size 0x04, offset 0x10
			int				actorPlayerId;		// size 0x04, offset 0x14
			SeqSoundInfo	seqSoundInfo;		// size 0x08, offset 0x18
		}; // size 0x20

	// methods
	public:
		// cdtors
		virtual ~SoundStartable() {}

		// virtual function ordering
		// vtable SoundStartable
		virtual StartResult detail_SetupSound(SoundHandle *handle, u32 soundId,
		                                      bool holdFlag,
		                                      StartInfo const *startInfo) = 0;
		virtual u32 detail_ConvertLabelStringToSoundId(char const *label) = 0;

		// methods
		StartResult detail_StartSound(
			SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo
		);
		StartResult detail_StartSound(
			SoundHandle *pHandle, const char *label, const StartInfo *pStartInfo
		);

		StartResult detail_HoldSound(
			SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo
		);
		StartResult detail_HoldSound(
			SoundHandle *pHandle, const char *label, const StartInfo *pStartInfo
		);

		StartResult detail_PrepareSound(
			SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo
		);
		StartResult detail_PrepareSound(
			SoundHandle *pHandle, const char *label,
			const StartInfo *pStartInfo
		);

		bool StartSound(SoundHandle *pHandle, u32 id) {
			return detail_StartSound(pHandle, id, NULL) == START_SUCCESS;
		}
		bool StartSound(SoundHandle *pHandle, const char *label) {
			return detail_StartSound(pHandle, label, NULL) == START_SUCCESS;
		}
		bool StartSound(SoundHandle *pHandle, unsigned int id) {
			return detail_StartSound(pHandle, id, NULL) == START_SUCCESS;
		}
		bool StartSound(SoundHandle *pHandle, int id) {
			return detail_StartSound(pHandle, id, NULL) == START_SUCCESS;
		}

		bool HoldSound(SoundHandle *pHandle, u32 id) {
			return detail_HoldSound(pHandle, id, NULL) == START_SUCCESS;
		}
		bool HoldSound(SoundHandle *pHandle, const char *label) {
			return detail_HoldSound(pHandle, label, NULL) == START_SUCCESS;
		}
		bool HoldSound(SoundHandle *pHandle, unsigned int id) {
			return detail_HoldSound(pHandle, id, NULL) == START_SUCCESS;
		}
		bool HoldSound(SoundHandle *pHandle, int id) {
			return detail_HoldSound(pHandle, id, NULL) == START_SUCCESS;
		}

		bool PrepareSound(SoundHandle *pHandle, u32 id) {
			return detail_PrepareSound(pHandle, id, NULL) == START_SUCCESS;
		}
		bool PrepareSound(SoundHandle *pHandle, unsigned int id) {
			return detail_PrepareSound(pHandle, id, NULL) == START_SUCCESS;
		}
		bool PrepareSound(SoundHandle *pHandle, int id) {
			return detail_PrepareSound(pHandle, id, NULL) == START_SUCCESS;
		}

		// The detail_ functions above are probably not meant to be called directly from game code,
		// so these inlines probably exist, but I haven't found them in Ketteiban or BBA. An inline
		// fixes a regswap in d/snd so that's more evidence. Names are obviously made up.
		// TODO: Do these exist in other variants for different ID types too???

		StartResult StartSoundReturnStatus(SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo) {
			return detail_StartSound(pHandle, id, pStartInfo);
		}

		StartResult HoldSoundReturnStatus(SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo) {
			return detail_HoldSound(pHandle, id, pStartInfo);
		}

		StartResult PrepareSoundReturnStatus(SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo) {
			return detail_PrepareSound(pHandle, id, pStartInfo);
		}

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_STARTABLE_H
