#pragma once

#include <cfg_items.h>

#pragma pack(push, 1)
typedef struct {
    const CFG_HDR cfghdr_0;
    char csm_list_path[128];
    const CFG_HDR cfghdr_1;
    int enable_hello_msg;
    const CFG_HDR cfghdr_2;
    char under_idle_constr[16];
    const CFG_HDR cfghdr_3;
    int activation_style;
    const CFG_CBOX_ITEM cfgcbox_3[4];
    const CFG_HDR cfghdr_4;
    int activation_key;
    const CFG_HDR cfghdr_5;
    int enable_long_press;
    const CFG_CBOX_ITEM cfgcbox_5[5];
#ifdef NEWSGOLD
    const CFG_HDR cfghdr_6;
    int red_button_mode;
    const CFG_CBOX_ITEM cfgcbox_6[3];
#endif
    const CFG_HDR cfghdr_7;
    int enable_lock;
    const CFG_HDR cfghdr_8;
    int show_daemons;
    const CFG_HDR cfghdr_9;
    int show_idle;

    const CFG_HDR cfghdr_b_b;
    const CFG_HDR cfghdr_b_0;
    char bm1_name[32];
    const CFG_HDR cfghdr_b_1;
    char bm1_value[128];
    const CFG_HDR cfghdr_b_2;
    char bm2_name[32];
    const CFG_HDR cfghdr_b_3;
    char bm2_value[128];
    const CFG_HDR cfghdr_b_4;
    char bm3_name[32];
    const CFG_HDR cfghdr_b_5;
    char bm3_value[128];
    const CFG_HDR cfghdr_b_6;
    char bm4_name[32];
    const CFG_HDR cfghdr_b_7;
    char bm4_value[128];
    const CFG_HDR cfghdr_b_8;
    char bm5_name[32];
    const CFG_HDR cfghdr_b_9;
    char bm5_value[128];
    const CFG_HDR cfghdr_b_10;
    char bm6_name[32];
    const CFG_HDR cfghdr_b_11;
    char bm6_value[128];
    const CFG_HDR cfghdr_b_12;
    char bm7_name[32];
    const CFG_HDR cfghdr_b_13;
    char bm7_value[128];
    const CFG_HDR cfghdr_b_14;
    char bm8_name[32];
    const CFG_HDR cfghdr_b_15;
    char bm8_value[128];
    const CFG_HDR cfghdr_b_16;
    char bm9_name[32];
    const CFG_HDR cfghdr_b_17;
    char bm9_value[128];
    const CFG_HDR cfghdr_b_e;
} CONFIG;
#pragma pack(pop)

extern CONFIG CFG;
extern char CFG_PATH[];

int InitConfig();
void SaveConfig();

