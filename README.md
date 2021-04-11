# mhserv
A webserver that i made.

## Building
You need `gcc`, `cmake` and `make` installed.

After that you can execute this commands:
```sh
cmake .
make
```

## Running
To make a simple test if it's built correctly you can test by running `mhserv` with `libexample.so`:
```
./mhserv libexample.so
```
Then go to the [default URL](http://localhost:8080/), if you see a web page - it works.

If you have your own handler library you can load it in the same way.

## Making your own handler

TODO: Add text here.
