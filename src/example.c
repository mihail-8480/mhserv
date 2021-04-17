#include <network/mh_http_handler.h>

void mh_http_handle(mh_http_request_t *request) {
    // Send headers
    MH_ECHO("HTTP/1.1 200 OK" MH_ENDL);
    MH_ECHO("Content-Type: text/html; charset=UTF-8" MH_ENDL);
    MH_ECHO("Connection: close" MH_ENDL);
    MH_ECHO(MH_ENDL);

    if (mh_memory_is_equal(request->url, MH_REF_CONST("/quit"))) {
        MH_ECHO("<p>The listener should stop.</p>");
        request->listener->running = false;
        return;
    } else {
        MH_ECHO("<h1>IT WORKS!!!</h1>");
        MH_ECHO("<p>Running on <b>" MH_PLATFORM "</b>.</p>");
    }
}
