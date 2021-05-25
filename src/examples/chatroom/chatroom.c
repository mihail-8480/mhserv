#include <mh_http_handler.h>

MH_UNUSED void mh_lib_init(MH_UNUSED mh_tcp_listener_t *listener) {

}

MH_UNUSED void mh_http_handle(mh_http_request_t *request) {
    mh_stream_t *stream
            = mh_file_stream_new(request->context,
                                 fopen("chatroom/index.html", "rb"),
                                 true);
    size_t size = mh_stream_get_size(stream);
    MH_ECHO("HTTP/1.1 200 OK" MH_ENDL);
    MH_ECHO("Content-Type: text/html; charset=UTF-8" MH_ENDL);
    MH_ECHO("Connection: close" MH_ENDL);
    MH_ECHO(MH_ENDL);
    mh_stream_copy_to(request->stream, stream, size);
}
