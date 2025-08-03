#pragma once

#include <swilib.h>

typedef int (*on_message_t)(CSM_RAM *, GBS_MSG *msg);
typedef void (*on_close_t)(CSM_RAM *);

extern CSM_DESC IDLE_CSM_DESC;
extern on_message_t IDLE_OnMessage;
extern on_close_t IDLE_OnClose;

void IDLE_EnableHook(on_message_t on_message, on_close_t on_close);
void IDLE_DisableHook();
