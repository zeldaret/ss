#ifndef EGG_SCENE_MANAGER_H
#define EGG_SCENE_MANAGER_H

namespace EGG {

class SceneManager {
public:
    // vtable at 0x0
    /* 0x08 */ virtual void calc();
    /* 0x0C */ virtual void draw();
    /* 0x10 */ virtual void calcCurrentScene();
    /* 0x14 */ virtual void calcCurrentFader();
    /* 0x18 */ virtual void drawCurrentScene();
    /* 0x1C */ virtual void drawCurrentFader();
    /* 0x20 */ virtual void createDefaultFader();
};

} // namespace EGG

#endif // EGG_SCENE_MANAGER_H
