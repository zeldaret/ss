#ifndef MINIGAME_MGR_H
#define MINIGAME_MGR_H

class MinigameManager {
public:
    enum Minigame {
        NONE,
        BAMBOO_CUTTING,
        FUN_FUN_ISLAND,
        THRILL_DIGGER,
        PUMPKIN_CARRY,
        INSECT_CAPTURE,
        PUMPKIN_ARCHERY,
        ROLLERCOASTER,
        TRIAL_TIME_ATTACK,
        BOSS_RUSH,
        HOUSE_CLEANING,
        SPIRAL_CHARGE_TUTORIAL,
        HARP_PLAYING,
    };
    static bool isInMinigameState(Minigame game);

    static bool isInAnyMinigame();

    static bool endMinigame(Minigame game);

    static void create();
    static void destroy();
    static void execute();

    static MinigameManager *GetInstance() {
        return sInstance;
    }
    static bool checkInInsectCapture() {
        return isInMinigameState(INSECT_CAPTURE);
    }
    static bool checkInPumpkinCarry() {
        return isInMinigameState(PUMPKIN_CARRY);
    }
    static bool checkInBambooCutting() {
        return isInMinigameState(BAMBOO_CUTTING);
    }
    static bool checkInThrillDigger() {
        return isInMinigameState(THRILL_DIGGER);
    }
    static bool checkInFunFunIsland() {
        return isInMinigameState(FUN_FUN_ISLAND);
    }
    static bool checkInHouseCleaning() {
        return isInMinigameState(HOUSE_CLEANING);
    }
    static bool checkInTrialTimeAttack() {
        return isInMinigameState(TRIAL_TIME_ATTACK);
    }
    static bool checkInBossRush() {
        return isInMinigameState(BOSS_RUSH);
    }
    static bool checkInPumpkinArchery() {
        return isInMinigameState(PUMPKIN_ARCHERY);
    }
    static bool checkInHarpPlaying() {
        return isInMinigameState(HARP_PLAYING);
    }
    static bool checkInRollerCoaster() {
        return isInMinigameState(ROLLERCOASTER);
    }
    static bool checkInSprialChargeTutorial() {
        return isInMinigameState(SPIRAL_CHARGE_TUTORIAL);
    }

private:
    MinigameManager() {}
    virtual ~MinigameManager() {}

    static MinigameManager *sInstance;
};

#endif
