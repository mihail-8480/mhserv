# mhserv
A relatively simple multi-platform C HTTP server. 

## Building

### Unix-like
Currently tested on: GNU/Linux, Android (Termux Arch Linux), FreeBSD, Haiku and Mac.

To build `mhserv` you need to have `gcc`, `cmake` and `make` installed.

```sh
git clone https://github.com/mihail-8480/mhserv
cd mhserv
cmake . && cmake --build .
```

### Windows

You need to install `msys2`, and inside of it to install the following packages:

* `mingw-w64-x86-64-gcc`
* `mingw-w64-x86-64-gdb`
* `mingw-w64-x86-64-make`
* `mingw-w64-x86_64-dlfcn`

Then you can use CLion or the Unix-like instructions to compile it.

## Running
On Windows use `libexample.dll`, on Mac use `libexample.dylib` instead.

To make a simple test if it's built correctly you can run `mhserv` with `libexample.so`:

```sh
./mhserv ./libexample.so
```

Then go to the [default URL](http://localhost:8080/), if a webpage is displayed - it works!

If you have your own handler library you can load it in the same way.

### Environment Variables

| Name | Description | Default |
|------| ----------- | ------- |
| MH_LIB_FUNCTION | The function that handles the HTTP requests. | `mh_http_handle` |
| MH_PORT | The port that the HTTP server is hosted at. | `8080` |
| MH_IP | The IP that the HTTP server is hosted at. | `127.0.0.1` |

## Making your own handler

Create a new dynamic library that uses `libmh.so` and declares a function that looks like this:

```c
#include "mh/network/mh_http_handler.h"

void mh_http_handle(const mh_tcp_listener_t* listener, mh_context_t *context, mh_stream_t *socket_stream, mh_http_request_t *request) {
    // your code here
}

```

See `example.c` for reference.
