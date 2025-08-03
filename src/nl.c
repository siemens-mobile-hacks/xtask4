#include <swilib.h>
#include <stdlib.h>
#include "nl.h"

NAMELIST *volatile NLTOP;

void AddNL(WSHDR *ws, int is_daemon) {
    NAMELIST *nnl = malloc(sizeof(NAMELIST));
    nnl->name = ws;
    nnl->next = 0;
    nnl->is_daemon = is_daemon;
    LockSched();
    if (!NLTOP) {
        NLTOP = nnl;
    }
    else {
        nnl->next = NLTOP;
        NLTOP = nnl;
    }
    UnlockSched();
}

NAMELIST *GetNLItem(int curitem) {
    NAMELIST *nl;
    nl = NLTOP;
    int i = 0;
    while (nl) {
        if (i == curitem) break;
        i++;
        nl = nl->next;
    }
    return (nl);
}

void ClearNL() {
    LockSched();
    NAMELIST *nl = NLTOP;
    NLTOP = 0;
    UnlockSched();
    while (nl) {
        NAMELIST *p = nl;
        FreeWS(nl->name);
        nl = p->next;
        mfree(p);
    }
}
