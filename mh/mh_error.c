#include "mh_error.h"
#include <stdlib.h>
#include <stdio.h>

mh_error_reporter mh_reporter = NULL;

void mh_error_report(const char *error) {
    if (mh_reporter != NULL) {
        mh_reporter(error);
    } else {
        fprintf(stderr, "%s", error);
        abort();
    }
}

void mh_error_set_reporter(mh_error_reporter reporter) {
    mh_reporter = reporter;
}

void mh_error_report_internal(bool cond) {
    if (!cond) {
        perror("Internal error");
        mh_error_report("An internal error was reported.");
    }
}
