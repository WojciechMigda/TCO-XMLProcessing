# Issues reported before the Appeals phase

These issues were related to the deployment of the application.

* `boost` was not being detected. I forgot about it and it worked on my setup because on ubuntu `boost` is installed under default `/usr` prefix, whereas reviewer had it installed in some non standard location.
* `stdc++fs` was unconditionally linked against. This library is required for `std::filesystem` with `g++` being used. It turned out that macos `clang` used by the reviewer does not have this library and support for `std::filesystem` does not require linking against any additional library.
* unused capture in one of the lambdas. Again, `g++` was not reporting this warning, but reviewer's macos clang warned about it and with `-Werror` caused compilation to fail.

Fixes to the above are included in the `appeals` tag.

# Issues reported as part of the Review phase

Three issues were reported. Quote:

1. If the output directory does not exist, the CLI should create it.
2. There's no way to provide a list of excluded tags as discussed in the forum.
3. Your CLI does not handle invalid input files gracefully. It should catch the error and show a meaningful message. Right now, it throws something like the error below and crashes:
`[2020-11-23 20:26:16.724] [error] Bad XML: <unspecified file>(1): expected <`

For the first two, fixes are included in the `final` tag.

As for the third one, this is the implemented graceful exit. Error notification is presented with the timestamp, `Bad XML` is clearly part of the message, and the exact failure reason reported by the used 3rd party XML parser is quoted verbatim. Arguably this one is unjust.
