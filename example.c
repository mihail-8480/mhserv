#include "mh/network/mh_http.h"

void http_handle(mh_context_t* context, mh_stream_t* socket_stream, mh_http_request_t* request) {
    ECHO("HTTP/1.1 200 OK" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);
    ECHO("<h1>IT WORKS!!!</h1>");
}
