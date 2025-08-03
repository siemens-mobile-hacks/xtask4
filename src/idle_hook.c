#include <swilib.h>
#include <string.h>
#include "idle_hook.h"

CSM_DESC IDLE_CSM_DESC;
CSM_DESC *OLD_IDLE_CSM_DESC;
on_message_t IDLE_OnMessage;
on_close_t IDLE_OnClose;

void IDLE_EnableHook(on_message_t on_message, on_close_t on_close) {
    CSM_RAM *idle = FindCSMbyID(CSM_root()->idle_id);
    memcpy(&IDLE_CSM_DESC, idle->constr, sizeof(IDLE_CSM_DESC));
    IDLE_OnClose = IDLE_CSM_DESC.onClose;
    IDLE_OnMessage = IDLE_CSM_DESC.onMessage;
    IDLE_CSM_DESC.onMessage = on_message;
    IDLE_CSM_DESC.onClose = on_close;
    OLD_IDLE_CSM_DESC = idle->constr;
    idle->constr = &IDLE_CSM_DESC;
}

void IDLE_DisableHook() {
    CSM_RAM *idle = FindCSMbyID(CSM_root()->idle_id);
    if (idle) {
        OLD_IDLE_CSM_DESC->onMessage = IDLE_OnMessage;
        OLD_IDLE_CSM_DESC->onClose = IDLE_OnClose;
        idle->constr = OLD_IDLE_CSM_DESC;
    }
}
