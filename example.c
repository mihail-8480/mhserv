#include "mh/network/mh_http.h"

void MH_HTTP_HANDLE(MH_HTTP_DEFAULT_ARGS) {
    MH_ECHO("HTTP/1.1 200 OK" MH_ENDL);
    MH_ECHO("Content-Type: text/html; charset=UTF-8" MH_ENDL);
    MH_ECHO("Connection: close" MH_ENDL);
    MH_ECHO(MH_ENDL);
    MH_ECHO("<h1>IT WORKS!!!</h1>");
}
