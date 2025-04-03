#include "d/d_message.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/d_textunk.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "libms/libms.h"
#include "libms/msgfile.h"
#include "sized_string.h"
#include "toBeSorted/global_fi_context.h"

SPECIAL_BASE_PROFILE(MESSAGE, dMessage_c, fProfile::MESSAGE, 0x2A8, 0);

static const char *sMsbtFileNames[82] = {
    "000-Test.msbt",
    "001-Action.msbt",
    "002-System.msbt",
    "003-ItemGet.msbt",
    "004-Object.msbt",
    "005-Tutorial.msbt",
    "006-KenseiNormal.msbt",
    "006-1KenseiNormal.msbt",
    "006-2KenseiNormal.msbt",
    "006-3KenseiNormal.msbt",
    "006-4KenseiNormal.msbt",
    "006-5KenseiNormal.msbt",
    "006-6KenseiNormal.msbt",
    "006-7KenseiNormal.msbt",
    "006-8KenseiNormal.msbt",
    "006-9KenseiNormal.msbt",
    "007-MapText.msbt",
    "008-Hint.msbt",
    "word.msbt",
    "100-Town.msbt",
    "101-Shop.msbt",
    "102-Zelda.msbt",
    "103-DaiShinkan.msbt",
    "104-Rival.msbt",
    "105-Terry.msbt",
    "106-DrugStore.msbt",
    "107-Kanban.msbt",
    "108-ShinkanA.msbt",
    "109-TakeGoron.msbt",
    "110-DivingGame.msbt",
    "111-FortuneTeller.msbt",
    "112-Trustee.msbt",
    "113-RemodelStore.msbt",
    "114-Friend.msbt",
    "115-Town2.msbt",
    "116-InsectGame.msbt",
    "117-Pumpkin.msbt",
    "118-Town3.msbt",
    "119-Captain.msbt",
    "120-Nushi.msbt",
    "121-AkumaKun.msbt",
    "122-Town4.msbt",
    "123-Town5.msbt",
    "124-Town6.msbt",
    "125-D3.msbt",
    "150-Siren.msbt",
    "198-Test.msbt",
    "199-Demo.msbt",
    "200-Forest.msbt",
    "201-ForestD1.msbt",
    "202-ForestD2.msbt",
    "203-ForestF2.msbt",
    "204-ForestF3.msbt",
    "250-ForestSiren.msbt",
    "251-Salvage.msbt",
    "299-Demo.msbt",
    "300-Mountain.msbt",
    "301-MountainD1.msbt",
    "302-Anahori.msbt",
    "303-MountainF2.msbt",
    "304-MountainD2.msbt",
    "305-MountainF3.msbt",
    "350-MountainSiren.msbt",
    "351-Salvage.msbt",
    "399-Demo.msbt",
    "400-Desert.msbt",
    "401-DesertD2.msbt",
    "402-DesertF2.msbt",
    "403-DesertD1.msbt",
    "404-DesertF3.msbt",
    "405-DesertD2Clear.msbt",
    "406-TrolleyRace.msbt",
    "450-DesertSiren.msbt",
    "451-Salvage.msbt",
    "460-RairyuMinigame.msbt",
    "499-Demo.msbt",
    "500-CenterField.msbt",
    "501-Inpa.msbt",
    "502-CenterFieldBack.msbt",
    "503-Goron.msbt",
    "510-Salvage.msbt",
    "599-Demo.msbt",
};

static char *sMsbfFileNames[80] = {
    "000-Test.msbf",
    "001-Action.msbf",
    "002-System.msbf",
    "003-ItemGet.msbf",
    "004-Object.msbf",
    "005-Tutorial.msbf",
    "006-KenseiNormal.msbf",
    "006-1KenseiNormal.msbf",
    "006-2KenseiNormal.msbf",
    "006-3KenseiNormal.msbf",
    "006-4KenseiNormal.msbf",
    "006-5KenseiNormal.msbf",
    "006-6KenseiNormal.msbf",
    "006-7KenseiNormal.msbf",
    "006-8KenseiNormal.msbf",
    "006-9KenseiNormal.msbf",
    "008-Hint.msbf",
    "100-Town.msbf",
    "101-Shop.msbf",
    "102-Zelda.msbf",
    "103-DaiShinkan.msbf",
    "104-Rival.msbf",
    "105-Terry.msbf",
    "106-DrugStore.msbf",
    "107-Kanban.msbf",
    "108-ShinkanA.msbf",
    "109-TakeGoron.msbf",
    "110-DivingGame.msbf",
    "111-FortuneTeller.msbf",
    "112-Trustee.msbf",
    "113-RemodelStore.msbf",
    "114-Friend.msbf",
    "115-Town2.msbf",
    "116-InsectGame.msbf",
    "117-Pumpkin.msbf",
    "118-Town3.msbf",
    "119-Captain.msbf",
    "120-Nushi.msbf",
    "121-AkumaKun.msbf",
    "122-Town4.msbf",
    "123-Town5.msbf",
    "124-Town6.msbf",
    "125-D3.msbf",
    "150-Siren.msbf",
    "198-Test.msbf",
    "199-Demo.msbf",
    "200-Forest.msbf",
    "201-ForestD1.msbf",
    "202-ForestD2.msbf",
    "203-ForestF2.msbf",
    "204-ForestF3.msbf",
    "250-ForestSiren.msbf",
    "251-Salvage.msbf",
    "299-Demo.msbf",
    "300-Mountain.msbf",
    "301-MountainD1.msbf",
    "302-Anahori.msbf",
    "303-MountainF2.msbf",
    "304-MountainD2.msbf",
    "305-MountainF3.msbf",
    "350-MountainSiren.msbf",
    "351-Salvage.msbf",
    "399-Demo.msbf",
    "400-Desert.msbf",
    "401-DesertD2.msbf",
    "402-DesertF2.msbf",
    "403-DesertD1.msbf",
    "404-DesertF3.msbf",
    "405-DesertD2Clear.msbf",
    "406-TrolleyRace.msbf",
    "450-DesertSiren.msbf",
    "451-Salvage.msbf",
    "460-RairyuMinigame.msbf",
    "499-Demo.msbf",
    "500-CenterField.msbf",
    "501-Inpa.msbf",
    "502-CenterFieldBack.msbf",
    "503-Goron.msbf",
    "510-Salvage.msbf",
    "599-Demo.msbf",
};

static void *msbAlloc(size_t size) {}

static void msbFree(void *ptr) {}

dMessage_c::dMessage_c() {
    sInstance = this;
}

int dMessage_c::create() {
    mLanguage = getLanguageIdentifier();

    mCurrentTextFileNumber = -1;
    field_0x2FC = 0;
    field_0x344 = 0;
    LMS_SetMemFuncs(msbAlloc, msbFree);
    setZevFromMsbArc();

    for (s32 i = 0; i < 82; i++) {
        SizedString<0x40> fileName = getMsbtFileName(i);
        s32 num = getMsbtNumberByIndex(i);
        void *data = getDataFromMsbArc(num, fileName, true);
        if (data == nullptr) {
            mpMsgs[i] = nullptr;
        } else {
            mpMsgs[i] = LMS_InitMessage(data);
        }
    }

    for (s32 i = 0; i < 80; i++) {
        SizedString<0x40> fileName = getMsbfFileName(i);
        s32 num = getMsbfNumberByIndex(i);
        void *data = getDataFromMsbArc(num, fileName, true);
        if (data == nullptr) {
            mpFlows[i] = nullptr;
        } else {
            mpFlows[i] = LMS_InitFlow(data);
        }
    }

    sTagProcessor = new dTagProcessor_c();
    createGlobalFiContext();
    reset();
    return SUCCEEDED;
}

int dMessage_c::doDelete() {
    UnkTextThing::getInstance()->destroy();
    if (sTagProcessor != nullptr) {
        delete sTagProcessor;
        sTagProcessor = nullptr;
    }

    for (int i = 0; i < 82; i++) {
        if (mpMsgs[i] != nullptr) {
            LMS_CloseMessage(mpMsgs[i]);
        }
    }

    for (int i = 0; i < 80; i++) {
        if (mpFlows[i] != nullptr) {
            LMS_CloseFlow(mpFlows[i]);
        }
    }
    return SUCCEEDED;
}

int dMessage_c::execute() {
    return SUCCEEDED;
}

int dMessage_c::draw() {
    return SUCCEEDED;
}

const char *dMessage_c::getMsbtFileName(s32 idx) {
    static SizedString<128> sPath;

    s32 arcIdx = getArcIndexForFile(sMsbtFileNames[idx]);
    const char *arcName = getArcNameByIndex(arcIdx, true);
    sPath.sprintf("%s/%s", arcName, sMsbtFileNames[idx]);
    return sPath;
}

const char *dMessage_c::getMsbfFileName(s32 idx) {
    static SizedString<128> sPath;

    s32 arcIdx = getArcIndexForFile(sMsbfFileNames[idx]);
    const char *arcName = getArcNameByIndex(arcIdx, true);
    sPath.sprintf("%s/%s", arcName, sMsbfFileNames[idx]);
    return sPath;
}

s32 dMessage_c::getMsbtNumberByIndex(s32 index) {
    return atoi(getMsbtFileName(index));
}

s32 dMessage_c::getMsbfNumberByIndex(s32 index) {
    return atoi(getMsbfFileName(index));
}

extern "C" u8 fn_80054F30();

static SizedString<8> sCurrentLanguage;
const char *dMessage_c::getLanguageIdentifier() {
    u8 lang = fn_80054F30();
    if (lang == 3) {
        sCurrentLanguage = "fr_US";
    } else if (lang == 4) {
        sCurrentLanguage = "es_US";
    } else {
        sCurrentLanguage = "en_US";
    }
    return sCurrentLanguage;
}

s32 dMessage_c::getArcIndexForFile(const char *fileName) {
    char buf[2];
    buf[0] = fileName[0];
    buf[1] = '\0';
    return atoi(buf);
}
