#include "printutil.h"

void printTimeStamp(time_t t) {
        time_t now = (t == 0 ? time(NULL) : t);
        struct tm* timeinfo;
        timeinfo = localtime(&now);
        printf("%.2i:%.2i:%.2i ",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
        return;
};

