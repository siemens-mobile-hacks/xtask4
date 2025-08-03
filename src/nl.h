#pragma once

#include <swilib.h>

typedef struct {
    void *next;
    WSHDR *name;
    void *p;
    int is_daemon;
} NAMELIST;

extern NAMELIST *volatile NLTOP;

void AddNL(WSHDR *ws, int is_daemon);
NAMELIST *GetNLItem(int id);
void ClearNL();
