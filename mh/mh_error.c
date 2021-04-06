#include "mh_error.h"
#include <stdlib.h>
#include <stdio.h>

// An alternative way to report errors
mh_error_reporter mh_reporter = NULL;
void mh_error_set_reporter(mh_error_reporter reporter) {
    mh_reporter = reporter;
}

void mh_error_report(const char *error) {
    // If there is an alternative error reporter, use it
    if (mh_reporter != NULL) {
        mh_reporter(error);
    } else {
        // Otherwise, print the error and crash the program
        fprintf(stderr, "%s", error);
        abort();
    }
}

void mh_error_report_internal(bool cond) {
    if (!cond) {
        // Report an errno problem
        perror("Internal error");
        mh_error_report("An internal error was reported.");
    }
}
