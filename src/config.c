#include <swilib.h>
#include "config.h"

char CFG_PATH[] = "?:\\zbin\\etc\\XTask4.bcfg";

CONFIG CFG = {
    {CFG_STR_UTF8, "CSM list file", 3, 127},
    "?:\\zbin\\etc\\CSMlist.txt",
    {CFG_CHECKBOX, "Enable hello message", 0, 2},
    1,
    {CFG_STR_WIN1251, "UnderIdleCSM_DESC", 0, 15},
    "",
    {CFG_CBOX, "Activation style", 0, 4},
    0,
    {{"Short press"}, {"Long press"}, {"* + #"}, {"Enter Button"}},
    {CFG_KEYCODE, "Activation key", 0, 99},
    #ifndef NEWSGOLD
        99,
    #else
        #ifdef ELKA
            RED_BUTTON,
        #else
            INTERNET_BUTTON,
        #endif
    #endif
    {CFG_CBOX, "Extra: on longpress", 0, 5},
    0,
    {{"As in FW"}, {"Do nothing"}, {"Hide all tasks"},
        {"Launch XTask"}, {"Hide & Lock"}},
#ifdef NEWSGOLD
    {CFG_CBOX, "Red button mode", 0, 3},
    0,
    {{"As in FW"}, {"Classic"}, {"Hide all tasks"}},
#endif
    {CFG_CHECKBOX, "Enable if keyboard locked", 0, 2},
    0,
    {CFG_CHECKBOX, "Show NewStyleDaemons", 0, 2},
    0,
    {CFG_CHECKBOX, "Show IDLE", 0, 2},
    1,

    {CFG_LEVEL, "Bookmarks", 1, 0},
    {CFG_STR_WIN1251, "Bookmark 1 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 1 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 2 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 2 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 3 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 3 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 4 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 4 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 5 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 5 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 6 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 6 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 7 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 7 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 8 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 8 value", 3, 127},
    "",
    {CFG_STR_WIN1251, "Bookmark 9 name", 1, 31},
    "",
    {CFG_STR_UTF8, "Bookmark 9 value", 3, 127},
    "",
    {CFG_LEVEL, "", 0, 0},
};


int InitConfig() {
    int8_t default_disk = (int8_t)BCFG_GetDefaultDisk();
    CFG_PATH[0] = default_disk;
    CFG.csm_list_path[0] = default_disk;

    int result = 0;
    if (BCFG_LoadConfig(CFG_PATH, &CFG, sizeof(CONFIG)) == -1) {
        if (BCFG_SaveConfig(CFG_PATH, &CFG, sizeof(CONFIG) != -1)) {
            result = 1;
        }
    } else {
        result = 2;
    }
    return result;
}

void SaveConfig() {
    BCFG_SaveConfig(CFG_PATH, &CFG, sizeof(CONFIG));
}
