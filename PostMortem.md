# Issues reported before the Appeals phase

These issues were related to the deployment of the application.

* `boost` was not being detected. I forgot about it and it worked on my setup because on ubuntu `boost` is installed under default `/usr` prefix, whereas reviewer had it installed in some non standard location.
* `stdc++fs` was unconditionally linked against. This library is required for `std::filesystem` with `g++` being used. It turned out that macos `clang` used by the reviewer does not have this library and support for `std::filesystem` does not require linking against any additional library.
* unused capture in one of the lambdas. Again, `g++` was not reporting this warning, but reviewer's macos clang warned about it and with `-Werror` caused compilation to fail.

Fixes to the above are included in the `appeals` tag.
