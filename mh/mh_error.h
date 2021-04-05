#ifndef MHSERV_MH_ERROR_H
#define MHSERV_MH_ERROR_H
typedef void (*mh_error_reporter)(const char* error);
void mh_error_report(const char* error);
void mh_error_set_reporter(mh_error_reporter reporter);
#endif //MHSERV_MH_ERROR_H
