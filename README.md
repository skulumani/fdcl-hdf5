| Build Status                             | Latest Release                                      | Version                                             | Last Commit                                                     | Activity                                                      
| :--------------------------------------: | :--------------------------:                        | :----:                                              | :------:                                                        | :------:                                                      
| [![Travis][travis_shield]][travis]       | [![Github Release][release_shield]][github_release] | [![Github Version][version_shield]][github_version] | [![Github Last Commit][last_commit_shield]][github_last_commit] | [![Github commit activity][activity_shield]][github_activity] 

[travis_shield]: https://travis-ci.org/skulumani/fdcl-hdf5.svg?branch=master 
[release_shield]: https://img.shields.io/github/release/skulumani/fdcl-hdf5.svg
[version_shield]: https://badge.fury.io/gh/skulumani%2Ffdcl-hdf5.svg
[last_commit_shield]: https://img.shields.io/github/last-commit/skulumani/fdcl-hdf5.svg
[activity_shield]: https://img.shields.io/github/commit-activity/y/skulumani/fdcl-hdf5.svg

[travis]: https://travis-ci.org/skulumani/fdcl-hdf5
[github_release]: https://github.com/skulumani/fdcl-hdf5/releases/latest
[github_version]: https://github.com/skulumani/fdcl-hdf5/releases/latest
[github_last_commit]: https://github.com/skulumani/fdcl-hdf5/commits/master
[github_activity]: https://github.com/skulumani/fdcl-hdf5/graphs/commit-activity

## FDCL-HDF5

This is a wrapper library for interfacing [Eigen]() datatypes with the [HDF5](https://support.hdfgroup.org/HDF5/) file format.
It allows one to easily save/read Eigen data in an open and cross-platform and cross language manner.

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

To use the shared library in your own project, execute everything above then add the following to your project `CMakeLists.txt`

~~~
find_package(FDCL_HDF5 REQUIRED)
add_executable(<your_target> <source_files>)
target_link_libraries(<your_target> fdcl_hdf5)
~~~

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
