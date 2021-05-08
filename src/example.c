#include <mh_http_handler.h>

static char version_string[32];
static size_t version_len = 0;

MH_UNUSED void mh_http_handle(mh_http_request_t *request) {
    // Send the headers
    MH_ECHO("HTTP/1.1 200 OK" MH_ENDL);
    MH_ECHO("Content-Type: text/html; charset=UTF-8" MH_ENDL);
    MH_ECHO("Connection: close" MH_ENDL);
    MH_ECHO(MH_ENDL);

    // Check if the URL is `/quit`
    if (mh_memory_is_equal(request->url, MH_STRING("/quit"))) {
        // If it is - stop the listener
        MH_ECHO("<p>The listener should stop.</p>");
        request->listener->running = false;
    } else {
        // If it isn't - print iT wOrKs and the platform
        MH_ECHO("<h1>IT WORKS!!!</h1>");
        MH_ECHO("<p>Running on <b>" MH_PLATFORM "</b>.</p>");

        // Create the version string if there isn't one already created
        if (version_len == 0) {
            mh_version_t version = mh_get_version();
            sprintf(version_string, "libmh %d.%d.%d", version.major, version.minor, version.patch);
            version_len = strlen(version_string);
        }

        // Print the libmh.so version
        MH_ECHO("<p>Using: <code>");
        mh_stream_write_reference(MH_SOCKET_STREAM, version_string, version_len);
        MH_ECHO("</code></p>");
    }
}
