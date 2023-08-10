#include <types.h>

// stub
struct FileManager {
    u8 pad[0xa840];
    u8 isFileUnk1[3];
    static FileManager *sInstance;
    u16* getSceneflags();
    u16* getTempflags();
    u16* getZoneflags();
    u16* getSceneflags2();
    u16* getTempflags2();
    u16* getZoneflags2();
    void setTempflags(u16*, u16, u16);
    void setZoneflags(u16*, u16, u16);
    void setSceneflags(u16*, u32, u16);
    static FileManager* getInstance() {
        return sInstance;
    }
};
