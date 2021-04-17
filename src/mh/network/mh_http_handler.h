#ifndef MHSERV_MH_HTTP_HANDLER_H
#define MHSERV_MH_HTTP_HANDLER_H

#include "mh_http.h"

// End of line (CRLF)
#define MH_ENDL "\r\n"

// The socket stream variable
#define MH_SOCKET_STREAM request->stream

// The request variable
#define MH_REQUEST request

// Write a string literal into MH_SOCKET_STREAM
#define MH_ECHO(str) mh_stream_write_reference(MH_SOCKET_STREAM, str, sizeof(str)-1)

// Write a string into MH_SOCKET_STREAM
#define MH_ECHO_STR(str) mh_stream_write_reference(MH_SOCKET_STREAM, str, strlen(str))

// Get a header from MH_REQUEST (str = string literal)
#define MH_HEADER(str) mh_map_get(MH_REQUEST->headers, MH_REF_CONST(str))

// Get a header from MH_REQUEST (str = string)
#define MH_HEADER_STR(str) mh_map_get(MH_REQUEST->headers, MH_REF_STRING(str))

#define MH_POST_READ() mh_http_request_read_content(MH_REQUEST)

// The HTTP handler
void mh_http_handle(mh_http_request_t *request);

#if defined(WIN32)
#define MH_PLATFORM "Windows"
#elif defined(LINUX)
#define MH_PLATFORM "Linux"
#elif defined (HAIKU)
#define MH_PLATFORM "Haiku"
#elif defined (UNIX_LIKE)
#define MH_PLATFORM "Generic Unix"
#elif defined (UNIX)
#define MH_PLATFORM "Other Unix"
#else
#define MH_PLATFORM "Unknown"
#endif

#endif //MHSERV_MH_HTTP_HANDLER_H
