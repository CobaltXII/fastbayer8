# fastbayer8

Fast 8-color Bayer dithering written in C++11

![Alt text](https://github.com/CobaltXII/fastbayer8/blob/master/fd8_hubble.png?raw=true)

# Compilation

```bash
# Multithreaded version
./build.sh
# Non-multithreaded version
./build_no_omp.sh
```

# Usage

```bash
cobalt$ ./fastbayer8.o
usage: ./fastbayer8.o <file>
```

# Credits

Thanks to http://hubblesite.org for a great test image. Thanks to Sean T. Barrett for the stb libraries.