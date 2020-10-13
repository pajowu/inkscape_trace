# Inkscape Batch Trace

This repo contains cpp code to automatically trace bitmaps to svgs.

## Installation

To build this tool, grab a copy of the inkscape source code:

``` shell
git clone https://gitlab.com/inkscape/inkscape.git
cd inkscape
```

Then copy the `inktrace-main.cpp` into the `src/` folder of inkscape.

Now we have to add a new build target to the inkscape makefiles.
Copy the `cmakelists.patch` to the inkscape folder (`inkscape/`, the one above `src/`) and run the following there:

``` shell
patch -p1 < cmakelists.patch
```

After that you can build inkscape as described in its `INSTALL.md`:
``` shell
mkdir build
cd build
cmake ..
make
```

Now you have the `inktrace` binary in the `bin` folder.

## Usage

You can run the tool the follwing:

``` shell
./bin/inktrace my_favorite_image1.png my_favorite_image2.png my_favorite_image3.png
```

## Configuration

There is no proper configuration (yet?).
Check the `inktrace-main.cpp` file.
The configuration is between `********* START PARAMETERS`  and `********* END PARAMETERS`.
I experienced that after setting `multiScanStack` to `true`, the paths were in the wrong order.
A workaround was added, see the line with `// ********** HACK: the layers in your output svgs are in the wrong order, switch which for loop is active`

## License

Like inkscape, this code is under GPL-2.0-or-later.
`SPDX-License-Identifier: GPL-2.0-or-later`, see `COPYING` for more.
