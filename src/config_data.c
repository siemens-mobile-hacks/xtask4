#include <swilib.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

__root CFG_HDR cfghdr_0 = {CFG_STR_UTF8, "CSM list file", 3, 127};
__root char CFG_CSM_LIST_PATH[128] = "0:\\zbin\\etc\\CSMlist.txt";

__root CFG_HDR cfghdr_1 = {CFG_CHECKBOX, "Enable hello message", 0, 2};
__root int CFG_ENABLE_HELLO_MSG = 1;

__root CFG_HDR cfghdr_2 = {CFG_STR_WIN1251, "UnderIdleCSM_DESC", 0, 8};
__root char CFG_UNDER_IDLE_CONSTR[9] = "";


__root CFG_HDR cfghdr_3 = {CFG_CBOX, "Activation style", 0, 4};
__root int CFG_ACTIVATION_STYLE = 0;
__root CFG_CBOX_ITEM cfgcbox_3[4] = {{"Short press"}, {"Long press"}, {"* + #"}, {"Enter Button"}};

#ifndef NEWSGOLD
    __root  CFG_HDR cfghdr_4 = {CFG_KEYCODE, "Activation key", 0, 99};
    __root  int CFG_ACTIVATION_KEY = 99;
#else
    #ifdef ELKA
        __root CFG_HDR cfghdr_4 = {CFG_KEYCODE, "Activation key", 0, 99};
        __root int CFG_ACTIVATION_KEY = RED_BUTTON;
    #else
        __root  CFG_HDR cfghdr_4 = {CFG_KEYCODE, "Activation key", 0, 99};
        __root  int CFG_ACTIVATION_KEY = 0x11;
    #endif
#endif

__root CFG_HDR cfghdr_5 = {CFG_CBOX, "Extra: on longpress", 0, 5};
__root int CFG_ENABLE_LONG_PRESS = 0;
__root CFG_CBOX_ITEM cfgcbox_5[5] = {
    {"As in FW"}, {"Do nothing"}, {"Hide all tasks"},
    {"Launch XTask"}, {"Hide & Lock"}
};

#ifdef NEWSGOLD
    __root CFG_HDR cfghdr_6 = {CFG_CBOX, "Red button mode", 0, 3};
    __root int CFG_RED_BUTTON_MODE = 0;
    __root CFG_CBOX_ITEM cfgcbox_6[3] = {{"As in FW"}, {"Classic"}, {"Hide all tasks"}};
#endif

__root CFG_HDR cfghdr_7 = {CFG_CHECKBOX, "Enable if keyboard locked", 0, 2};
__root int CFG_ENABLE_LOCK = 0;

__root CFG_HDR cfghdr_8 = {CFG_CHECKBOX, "Show NewStyleDaemons", 0, 2};
__root int CFG_SHOW_DAEMONS = 0;

__root CFG_HDR cfghdr_9 = {CFG_CHECKBOX, "Show IDLE", 0, 2};
__root int CFG_SHOW_IDLE = 1;

__root CFG_HDR cfghdr_10_1 = {CFG_STR_WIN1251, "Bookmark 1 name", 1, 31};
__root char BM1NAME[32] = "";
__root CFG_HDR cfghdr_10_2 = {CFG_STR_UTF8, "Bookmark 1 value", 3, 127};
__root char BM1FILE[128] = "";

__root CFG_HDR cfghdr10_3 = {CFG_STR_WIN1251, "Bookmark 2 name", 1, 31};
__root char BM2NAME[32] = "";
__root CFG_HDR cfghdr10_4 = {CFG_STR_UTF8, "Bookmark 2 value", 3, 127};
__root char BM2FILE[128] = "";

__root CFG_HDR cfghdr10_5 = {CFG_STR_WIN1251, "Bookmark 3 name", 1, 31};
__root char BM3NAME[32] = "";
__root CFG_HDR cfghdr10_6 = {CFG_STR_UTF8, "Bookmark 3 value", 3, 127};
__root char BM3FILE[128] = "";

__root CFG_HDR cfghdr10_7 = {CFG_STR_WIN1251, "Bookmark 4 name", 1, 31};
__root char BM4NAME[32] = "";
__root CFG_HDR cfghdr10_8 = {CFG_STR_UTF8, "Bookmark 4 value", 3, 127};
__root char BM4FILE[128] = "";

__root CFG_HDR cfghdr10_9 = {CFG_STR_WIN1251, "Bookmark 5 name", 1, 31};
__root char BM5NAME[32] = "";
__root CFG_HDR cfghdr10_10 = {CFG_STR_UTF8, "Bookmark 5 value", 3, 127};
__root char BM5FILE[128] = "";

__root CFG_HDR cfghdr10_11 = {CFG_STR_WIN1251, "Bookmark 6 name", 1, 31};
__root char BM6NAME[32] = "";
__root CFG_HDR cfghdr10_12 = {CFG_STR_UTF8, "Bookmark 6 value", 3, 127};
__root char BM6FILE[128] = "";

__root CFG_HDR cfghdr10_13 = {CFG_STR_WIN1251, "Bookmark 7 name", 1, 31};
__root char BM7NAME[32] = "";
__root CFG_HDR cfghdr10_14 = {CFG_STR_UTF8, "Bookmark 7 value", 3, 127};
__root char BM7FILE[128] = "";

__root CFG_HDR cfghdr10_15 = {CFG_STR_WIN1251, "Bookmark 8 name", 1, 31};
__root char BM8NAME[32] = "";
__root CFG_HDR cfghdr10_16 = {CFG_STR_UTF8, "Bookmark 8 value", 3, 127};
__root char BM8FILE[128] = "";

__root CFG_HDR cfghdr10_17 = {CFG_STR_WIN1251, "Bookmark 9 name", 1, 31};
__root char BM9NAME[32] = "";
__root CFG_HDR cfghdr10_18 = {CFG_STR_UTF8, "Bookmark 9 value", 3, 127};
__root char BM9FILE[128] = "";

__CFG_END(0)
