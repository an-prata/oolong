# oolong
A very small and hopefully effective library for quickly making TUI (sorta a terminal GUI) applications. Oolong aims to manage more rendering the screen than anything else, the actual logic is left largely to the application itself (see project in `example/`).

## Getting Started
The headers files should provide most if not all of the required documentation, the example project should help get you off the ground too. To use in a project just run the `build.sh` script, it will place the static library `oolong.a` and a tarball package `oolong_headers.tar` in the `build/` directory, just extract the headers (or copy them from the `oolong/` directory) and place them in your program then compile with `oolong.a`.
