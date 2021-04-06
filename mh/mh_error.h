#ifndef MHSERV_MH_ERROR_H
#define MHSERV_MH_ERROR_H

#include <stdbool.h>

// The error reporter function pointer type
typedef void (*mh_error_reporter)(const char* error);

// Change the current error reporter
void mh_error_set_reporter(mh_error_reporter reporter);

// Report an error
void mh_error_report(const char* error);

// Report an errno error if the condition is false
void mh_error_report_internal(bool cond);

#endif //MHSERV_MH_ERROR_H
