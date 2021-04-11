# mhserv
A webserver that i made.

## Building
To build `mhserv` you need to have `gcc`, `cmake` and `make` installed.

The build command is:
```sh
cmake . && make
```

## Running
To make a simple test if it's built correctly you can run `mhserv` with `libexample.so`:
```sh
./mhserv libexample.so
```
Then go to the [default URL](http://localhost:8080/), if a webpage is displayed - it works!

If you have your own handler library you can load it in the same way.

### Environment Variables
| Name | Description | Default |
|------| ----------- | ------- |
| MH_LIB_FUNCTION | The function that handles the HTTP requests. | `mh_http_handle` |
| MH_PORT | The port that the HTTP server is hosted at. | `8080` |

## Making your own handler
Create a new dynamic library that uses `libmh.so` and declares a function that looks like this:
```c
#include "mh/network/mh_http_handler.h"

void mh_http_handle(mh_context_t* context, mh_stream_t* socket_stream, mh_http_request_t* request) {
    // your code here
}

```
See `example.c` for reference.
