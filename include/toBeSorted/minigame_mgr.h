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
    static bool checkInBossRush() {
        return isInMinigameState(BOSS_RUSH);
    }

private:
    MinigameManager() {}
    virtual ~MinigameManager() {}

    static MinigameManager *sInstance;
};

#endif
