#include <mh_http_handler.h>

static mh_memory_t *version_str;

MH_UNUSED void mh_lib_init(mh_tcp_listener_t *listener) {
    mh_version_t version = mh_get_version();
    mh_stream_t *str = mh_memory_stream_new(listener->context, 0, false);
    version_str = mh_memory_stream_get_memory(str);
    mh_write(mh_writer_from_stream(str), "libmh {}.{}.{}",
             MH_FMT_UINT(version.major), MH_FMT_UINT(version.minor), MH_FMT_UINT(version.patch));
}

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
        // Print the libmh version
        MH_ECHO("<p>Using: <code>");
        mh_stream_write(MH_SOCKET_STREAM, version_str, version_str->offset);
        MH_ECHO("</code></p>");
    }
}
