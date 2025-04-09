#ifndef D_PAD_H
#define D_PAD_H

class dPad {
public:
    static bool checkButtonAPressed();
    static bool checkButtonBPressed();
    static bool checkButtonZPressed();
    static bool checkButtonPlusPressed();

    static bool checkButtonDpadDownPressed();

    static bool checkButtonAHeld();
    static bool checkButtonZHeld();
};

#endif
