#include <swilib.h>
#include <sie/sie.h>

extern const char BM1NAME[], BM2NAME[], BM3NAME[], BM4NAME[], BM5NAME[], BM6NAME[], BM7NAME[], BM8NAME[], BM9NAME[];
extern const char BM1FILE[], BM2FILE[], BM3FILE[], BM4FILE[], BM5FILE[], BM6FILE[], BM7FILE[], BM8FILE[], BM9FILE[];

const char *GetBookmarkName(int id) {
    switch (id) {
        case 0: return BM1NAME;
        case 1: return BM2NAME;
        case 2: return BM3NAME;
        case 3: return BM4NAME;
        case 4: return BM5NAME;
        case 5: return BM6NAME;
        case 6: return BM7NAME;
        case 7: return BM8NAME;
        case 8: return BM9NAME;
        default: return NULL;
    }
}

const char *GetBookmarkValue(int id) {
    switch (id) {
        case 0: return BM1FILE;
        case 1: return BM2FILE;
        case 2: return BM3FILE;
        case 3: return BM4FILE;
        case 4: return BM5FILE;
        case 5: return BM6FILE;
        case 6: return BM7FILE;
        case 7: return BM8FILE;
        case 8: return BM9FILE;
        default: return NULL;
    }
}

int ExecuteBookmark(int id) {
    const char *value = GetBookmarkValue(id);
    if (value) {
        if (strlen(value)) {
            Sie_Exec_File(value);
            return 1;
        }
    }
    return 0;
}
