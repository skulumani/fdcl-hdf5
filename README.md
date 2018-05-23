## FDCL-HDF5

This is a wrapper library for interfacing [Eigen]() datatypes with the [HDF5](https://support.hdfgroup.org/HDF5/) file format.
It allows one to easily save/read Eigen data in an open and cross-platform and cross language manner.

## Usage

All of the functionality is included in the `HDF5` namespace and is exposed to your program using

~~~
#include "hdf5.hpp"
~~~

You can easily create a file:

~~~
HDF5::File hf = HDF5::File("/tmp/test.hdf5", HDF5::File::Truncate);
~~~

There are several different modes for operating on files:

~~~
HDF5::File::ReadOnly; /**< Read only access */
HDF5::File::ReadWrite; /**< ReadWrite access */
HDF5::File::Truncate; /**< Overwrite a file if it exists or create a new one */
HDF5::File::Excl; /**< Only open if the file doesn't exist */
HDF5::File::Create; /**< Create a new file */
~~~

Once a file is created you can write some data (dataset) to it:

~~~
Eigen::MatriXd mat(1, 3);
mat = Eigen::MatrixXd::Random(1, 3);
hf.write("mat", mat);
~~~

Which write the variable `mat` to the file with a name of `"mat"`.

## Installation

This library depends on both `Eigen` and `HDF5` which can be installed using the included scripts.

1. [`build_eigen.sh`]( ./scripts/buid_eigen.sh ) - Install `Eigen` to `/usr/local`
2. [`build_hdf5.sh`]( ./scripts/build_hdf5.sh ) - Build and install `HDF5`
3. [`build_cmake.sh`](./scripts/build_cmake.sh) = Build `cmake` from source

From the source directory:

~~~
mkdir build && cd build
cmake ..
make
sudo checkinstall (or make install)
~~~~

Which uses `checkinstall` (`sudo apt-get install checkinstall`) to allow one to easily uninstall

~~~
sudo dpkg -r fdcl-hdf5
~~~
