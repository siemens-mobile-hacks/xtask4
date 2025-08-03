#include <swilib.h>
#include <stdlib.h>
#include <string.h>
#include <sie/sie.h>
#include "menu_bookmarks.h"

extern char CFG_PATH[];

static HEADER_DESC HEADER_D = {{0, 0, 0, 0}, NULL, (int)"Bookmarks...", LGP_NULL};

static const int SOFTKEYS[] = {SET_LEFT_SOFTKEY, SET_RIGHT_SOFTKEY, SET_MIDDLE_SOFTKEY};

static const SOFTKEY_DESC SOFTKEYS_D[] = {
    {0x0018, 0x0000, (int)"Config"},
    {0x0001, 0x0000, (int)"Close"},
    {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB SOFTKEYS_TAB = {
    SOFTKEYS_D, 0
};

static void GHook(GUI *gui, int cmd) {
    if (cmd == 0xA) { // focus
        DisableIDLETMR();
    }
}

static int OnKey(GUI *gui, GUI_MSG *msg) {
    MAIN_CSM *csm = MenuGetUserPointer(gui);

    int cursor = GetCurMenuItem(gui);
    if (msg->keys == 0x18) {
        Sie_Exec_File(CFG_PATH);
        GeneralFunc_flag1(csm->gui_id, 1);
        return 1;
    }
    if (msg->keys == 0x3D) {
        if (ExecuteBookmark(cursor)) {
            GeneralFunc_flag1(csm->gui_id, 1);
            return 1;
        }
        return -1;
    }
    return 0;
}

static void ItemProc(void *gui, int item_n, void *user_pointer) {
    WSHDR *ws;
    size_t len;
    const char *s;
    s = GetBookmarkName(item_n);
    void *item = AllocMenuItem(gui);
    if (s) {
        if ((len = strlen(s))) {
            ws = AllocMenuWS(gui, (int)len);
            wsprintf(ws, "%t", s);
        }
        else {
            s = GetBookmarkValue(item_n);
            if (s) {
                if (strlen(s)) {
                    char *p = strrchr(s, '\\');
                    if (p) {
                        int pos;
                        len = strlen(p + 1);
                        ws = AllocWS((int)len);
                        str_2ws(ws, p + 1, len);
                        if ((pos = wstrrchr(ws, ws->wsbody[0], '.')) != 0xFFFF) {
                            CutWSTR(ws, pos - 1);
                        }
                    }
                    else goto L_EMPTY;
                }
                else goto L_EMPTY;
            }
            else {
                L_EMPTY:
                    ws = AllocMenuWS(gui, 10);
                    wsprintf(ws, "Empty");
            }
        }
    }
    else {
        ws = AllocMenuWS(gui, 10);
        wsprintf(ws, "error");
    }
    SetMenuItemText(gui, item, ws, item_n);
}

static const MENU_DESC MENU_D =
{
    8,
    OnKey,
    GHook,
    NULL,
    SOFTKEYS,
    &SOFTKEYS_TAB,
    MENU_FLAGS_ENABLE_TEXT_SCROLLING,
    ItemProc,
    NULL,
    NULL,
    9,
};

int MenuBookmarks_Create(MAIN_CSM *csm) {
    memcpy(&(HEADER_D.rc), GetHeaderRECT(), sizeof(RECT));

    return CreateMenu(0, 0, &MENU_D, &HEADER_D, 0, 9, csm, 0);
}
