# Welcome to the wonderful world of XML processing!

Have you ever been given a bunch of large cluttered XML files and told to do something with them?

We have a solution for you!

Our modern application will take care of your troubles and transform
your XML files in a bat of an eye.


# Building the application

Building of the application is handled by `CMake`. The process follows
standard approach, but I will repeat the most important details below.

The code resides in the top-most folder, called `solution`. This is 
also where `CMakeLists.txt` resides.

Designated out-of-source folder is located below and is called `.build`.
There is a small `README.md` file there with basic `cmake` invocation
example.
As customary with `cmake` it allows to build the software using different
toolchains under different operating systems.

Even though core development of this program was done under Linux
(64-bit Ubuntu 18.04) it should be possible to build the program
also on Windows, using either clang, MinGW, or Visual Studio.
Care, though, should be taken as to the version of the compiler
as this program makes use of C++17 and `std::filesystem` features.

Specifically, I have used g++ 7.5.0.
Also, there is a dependency to `boost.property_tree`. Version used by me is 1.68.
Other dependencied are already included in the source code bundle.
They include:

* `spdlog` (logging)
* `fmt` (printing and string operations)
* `neither` (error handling)
* `clipp` (CLI parameters)
* `nlohmann::json` (JSON handling)

Once in the `.build` folder first you need to generate build system files.
On Linux these are native `Makefiles`.
Simply execute

`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -Werror -march=native" ..`

then issue

`make -j`

and if you like you may install the program by issuing

`make install`

Make sure, though, that you have privileges to install into default install paths. It is always possible to provide custom install prefixes, should one want that.

Successful build will create program executable in the `app` subfolder of `.build` directory. The program is called `main`.

# Usage

To see the program's synopsis simply execute `main` without arguments.

This is the example output:

```
SYNOPSIS
        ./main --input <XML data file to read from> --output-dir <Destination folder> [--root-name
               <Name for the root node=<ROOT>>] [--skip-root|--no-skip-root] [--json-indent <JSON
               indentation level=-1>] [--verbose] [--debug]

OPTIONS
        --input, -i Input XML file
        --output-dir, -o
                    Destination folder where output files are to be created

        --root-name Name for the root node to use, if enabled
        --skip-root|--no-skip-root
                    Include or do not include root node in created json files, default=true

        --json-indent
                    Indentation level for produced JSON, -1 to disable

        --verbose, -v
                    Enable verbose output, default=false

        --debug     Enable debug output, default=false
```

There are two mandatory parameters: input XML file (`-i`) and output folder path (`-o`).

Their meaning is self-explanatory, and also they were requested in the problem specification.

There are few extra parameters that I will bring my attention to.

* `verbose` and `debug` control verbosity of the program output. `debug` is more verbose than `verbose.
* `json-indent` controls formatting of the produced JSON output written into output files. With default value of -1 no indentation is
used:

```
{"attributes":[{"name":"data_encoding","required":true}],"children":[{"name":"CD_ATTACHMENT_JOURNAL","required":true}]}
```

Specifying different non-negative indentation value will cause indentation to be applied. Value of 2 will give this:

```
{
  "attributes": [
    {
      "name": "project_id",
      "required": true
    },
    {
      "name": "project_target_id",
      "required": true
    },
    {
      "name": "site_id",
      "required": true
    }
  ],
  "children": []
}

```
* `skip-root`/`no-skip-root` and `root-name`. Very specific details about these two
options are provided in the source code. Basically, they control behaviour with respect to the top node which is produced by boost's
XML parser. As such, root node has no name. In the code it is given
an arbitrary name, which can be changed with `root-name`. This matters
if non-default `no-skip-root` option is chosen. If `skip-root` is selected then name of the root node does not matter. But id the
user selects `no-skip-root` then the root node will output into one
of the resulting files.
One thing to note: if you select custom name for the root node make
sure that it is not the same as any of the nodes in the processed
XML file.

# Various comments

I have not made any extra effort as to optimize memory usage.
With the provided example XML file peak resident memory value
indicated by `time -v` shows that `main` ran for 12 seconds and consumed 6.5 GB of RAM.
