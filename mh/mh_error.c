#include "mh_error.h"
#include "mh_console.h"
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
        mh_internal_console = mh_internal_console.open();
        mh_internal_console.error.write(error);
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

void mh_error_report_safe(const char *error, mh_destructor_t *destructor) {
    // Free the memory and report the error
    mh_destructor_free(destructor);
    mh_error_report(error);
}
