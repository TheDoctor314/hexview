## Hexview

`hexview` is a simple hex viewer for the terminal written in C++17. It uses coloured output to differentiate between different kinds of bytes (NULL, Printable ASCII, ASCII whitespace, other ASCII characters and non-ASCII).

### Building from source
`hexview` uses the `meson` build system. Install the latest version of [meson](https://github.com/mesonbuild/meson) and execute the following commands:
```
meson setup buildir
meson compile
```
The resulting binary will be present in the `builddir`.

To run the tests, just do:
```
meson test
```

### License
This project is licensed under the [Unlicense](https://unlicense.org)