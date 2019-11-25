# pQCTIO
Input/Output library for reading and writing images for the
Stratec pQCT system.

[![Build Status](https://dev.azure.com/babesler/n88/_apis/build/status/Numerics88.pQCTIO?branchName=master)](https://dev.azure.com/babesler/n88/_build/latest?definitionId=7&branchName=master)
[![Anaconda-Server Badge](https://anaconda.org/numerics88/pqctio/badges/installer/conda.svg)](https://anaconda.org/Numerics88/pqctio/)

# Compiling and linking
pQCTIO requires the following:

  * CMake: www.cmake.org
  * Boost: www.boost.org
  * Google test: https://github.com/google/googletest

To build and run the tests with cmake, on linux or OS X, something like the
following sequence of commands is required:

```bash
mkdir build
cd build
ccmake ..   # Turn tests on, specify GTest dir
make
ctest -V
```

On Windows the procedure is a rather different: refer to CMake documentation.

For the tests, a two small datasets are download from online. If you don't want
this and you have access to the data, just put that files in the data directory
and no download will occur.

## Usage

### Reading a pQCT file

Here is a simple example of reading a pQCT file. Note that all pQCT data is
of type `short` and comes in a square matrix.

```C++
#include "pQCTIO/pQCTIO.h"

// Create pQCTFile object.
pQCTIO::pQCTFile reader(filename);

// Read header.
reader.ReadImageInfo();

// Examine some header values.
std::cout << reader << std::endl;

// Create a buffer for the image data.
size_t size = reader.number_of_voxels;
std::vector<short> image_data (size);

// Read the image data.
reader.ReadImageData (image_data.data(), size);

// Access a pixel
int y = 100;
int x = 120;
image_data[reader.PicMatrixX*y + x]);
```

For more details, refer to the header file `pQCTIO.h`.
For a complete working example, have a look at the test code in `tests/pQCTIOTests.cxx`.

### Writing a pQCT file
Currently, this is not supported. If this is required, please contact the maintainers of the package.

## Limitations

* Endianess is handled automatically on all platforms (via boost::endian). However,
  this only applies to integers. Currently floating point values are always
  kept int the IEEE binary format used on x86 processors, regardless of platform.
  This is obviously not portable. This may not be a serious limitation in your case.

* A fully defined header file is not available for the pQCT data format. If you need
  a specific piece of header data, please determine the offset, size, and type in the
  header and contact the developers for implementation. Pull requests welcome :)

* A writer for pQCT files is not yet implemented. The two major limitations are the square
  image matrix and the lack of a fully defined header. If you need the ability to write
  pQCT files, please contact the developers for implementation. Pull requests welcome :)

## Authors and Contributors

pQCTIO was developed by Bryce Besler.

## Licence

pQCTIO is licensed under a MIT-style open source license. See the file LICENSE.
