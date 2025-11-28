#ifndef D_T_SWORD_BATTLE_GAME_H
#define D_T_SWORD_BATTLE_GAME_H

#include "d/flag/storyflag_manager.h"
#include "d/t/d_tg.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/timekeeper.h"

class dTgSwordBattleGame_c : public dTg_c {
public:
    dTgSwordBattleGame_c() : mStateMgr(*this, sStateID::null), mEventRelated(*this, 0) {}
    virtual ~dTgSwordBattleGame_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    /* vt 0x74-0x80*/ STATE_VIRTUAL_FUNC_DECLARE(dTgSwordBattleGame_c, Wait);
    /* vt 0x80-0x88*/ STATE_VIRTUAL_FUNC_DECLARE(dTgSwordBattleGame_c, Manage);
    void init();

    bool checkInEvent(const char *eventName);

    static bool checkInBossRushFlag();
    void unsetInBossRushFlag();
    void setInBossRushFlag();

    static bool checkInBossRushMinigame();

    void unsetEarlyFightFlags() const;
    void unsetImprisonedFightFlags() const;
    void unsetDemiseFightFlag() const;

    s32 getCurrentTime() const;
    void saveCurrentTime(s32 time) const;

    s32 getBossIdx() const;
    s32 getBossHighscore() const;

    /** Index 0 */
    s32 getGhirahim1Highscore() const;
    /** Index 1 */
    s32 getScalderaHighscore() const;
    /** Index 2 */
    s32 getMolderachHighscore() const;
    /** Index 3 */
    s32 getImprisoned1Highscore() const;
    /** Index 4 */
    s32 getKoloktosHighscore() const;
    /** Index 5 */
    s32 getTentalusHighscore() const;
    /** Index 6 */
    s32 getGhirahim2Highscore() const;
    /** Index 7 */
    s32 getImprisoned2Highscore() const;
    /** Index 8 */
    s32 getImprisoned3Highscore() const;
    /** Index 9 */
    s32 getHordeHighscore() const;
    /** Index 10 */
    s32 getGhirahim3Highscore() const;
    /** Index 11 */
    s32 getDemiseHighscore() const;

    s32 getCurrentBossNumber() const;
    void setCurrentBossNumber(s32 num);

    void notifyBossDeath(bool ghirahim_related = false);
    bool checkFightStarted() const;

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dTgSwordBattleGame_c);
    /* 0x138 */ ActorEventRelated mEventRelated;
    /* 0x188 */ Timekeeper mTimer;
    /* 0x1A8 */ bool field_0x1A8;
    /* 0x1A9 */ bool field_0x1A9;
    /* 0x1AA */ bool field_0x1AA;
    /* 0x1AB */ bool field_0x1AB; ///< set if current boss is complete
    /* 0x1AC */ bool field_0x1AC;
    /* 0x1AD */ u8 field_0x1AD;
    /* 0x1AE */ u8 field_0x1AE;
    /* 0x1B0 */ u32 mBossNumber;

    // Note: Gets padded to 0x1B8 due to Timekeepers 8-byte alignment
};

#endif
