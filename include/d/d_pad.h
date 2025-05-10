#ifndef D_PAD_H
#define D_PAD_H

class dPad {
public:
    static bool checkButtonAPressed();
    static bool checkButtonBPressed();
    static bool checkButtonZPressed();
    static bool checkButtonCPressed();
    static bool checkButtonMinusPressed();
    static bool checkButtonPlusPressed();
    static bool checkButton1Pressed();
    static bool checkButton2Pressed();

    static bool checkButtonDpadUpPressed();
    static bool checkButtonDpadDownPressed();
    static bool checkButtonDpadLeftPressed();
    static bool checkButtonDpadRightPressed();

    static bool checkButtonAHeld();
    static bool checkButtonZHeld();
};

#endif
