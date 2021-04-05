#ifndef MHSERV_MH_ERROR_H
#define MHSERV_MH_ERROR_H

#include <stdbool.h>

typedef void (*mh_error_reporter)(const char* error);
void mh_error_report(const char* error);
void mh_error_set_reporter(mh_error_reporter reporter);
void mh_error_report_internal(bool cond);

#endif //MHSERV_MH_ERROR_H
