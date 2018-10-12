| Build Status                             | Coverage                                    | Latest Release                                      | Version                                             | Last Commit                                                     | Activity
| :--------------------------------------: | :-------------:                             | :--------------------------:                        | :----:                                              | :------:                                                        | :------:
| [![Travis][travis_shield]][travis]       | [![Coveralls][coveralls_shield]][coveralls] | [![Github Release][release_shield]][github_release] | [![Github Version][version_shield]][github_version] | [![Github Last Commit][last_commit_shield]][github_last_commit] | [![Github commit activity][activity_shield]][github_activity]

[travis_shield]: https://travis-ci.org/skulumani/fdcl-hdf5.svg?branch=master 
[release_shield]: https://img.shields.io/github/release/skulumani/fdcl-hdf5.svg
[version_shield]: https://badge.fury.io/gh/skulumani%2Ffdcl-hdf5.svg
[last_commit_shield]: https://img.shields.io/github/last-commit/skulumani/fdcl-hdf5.svg
[activity_shield]: https://img.shields.io/github/commit-activity/y/skulumani/fdcl-hdf5.svg
[coveralls_shield]: https://coveralls.io/repos/github/skulumani/fdcl-hdf5/badge.png?branch=master

[travis]: https://travis-ci.org/skulumani/fdcl-hdf5
[github_release]: https://github.com/skulumani/fdcl-hdf5/releases/latest
[github_version]: https://github.com/skulumani/fdcl-hdf5/releases/latest
[github_last_commit]: https://github.com/skulumani/fdcl-hdf5/commits/master
[github_activity]: https://github.com/skulumani/fdcl-hdf5/graphs/commit-activity
[coveralls]: https://coveralls.io/github/skulumani/fdcl-hdf5?branch=master

## FDCL-HDF5

This is a wrapper library for interfacing [Eigen](eigen.tuxfamily.org) datatypes with the [HDF5](https://support.hdfgroup.org/HDF5/) file format.
It allows one to easily save/read Eigen data in an open and cross-platform and cross language manner.
The overall goal is to emulate [`h5py`](http://docs.h5py.org/en/latest/quick.html) in easy of use with numpy/Eigen based data.

It currently supports the following:

1. Opening HDF5 Files in read only, read/write, truncate modes
2. Given an HDF5 file create subsequent groups, or datasets 
3. Given a group create more subgroups or datasets
4. Read/Write any Eigen variable (currently setup primarily for `Eigen::DenseBase` objects)

## Usage

All of the functionality is included in the `HDF5` namespace and is exposed to your program using

~~~
#include "hdf5.hpp"
~~~

There are several different modes for operating on files:

~~~
HDF5::File::ReadOnly; /**< Read only access */
HDF5::File::ReadWrite; /**< ReadWrite access */
HDF5::File::Truncate; /**< Overwrite a file if it exists or create a new one */
HDF5::File::Excl; /**< Only open if the file doesn't exist */
HDF5::File::Create; /**< Create a new file */
~~~

Here is a basic example of reading/writing Eigen matrices.
You can try this yourself using `main`
Look in the `tests` directory for more examples 
~~~
#include "hdf5.hpp"

#include <Eigen/Dense>

#include <iostream>

void write_data() {
    Eigen::MatrixXd matrix(3, 3);
    matrix << 1, 2, 3, 4, 5, 6, 7, 8, 9;

    // open the file 
    HDF5::File hf = HDF5::File("filename.hdf5", HDF5::File::Truncate);

    // write the data
    hf.write("dataset_name", matrix);
    
    std::cout << "Original Matrix: " << std::endl;
    std::cout << matrix << std::endl;
}

void read_data() {
    // open the file for reading
    HDF5::File hf = HDF5::File("filename.hdf5", HDF5::File::ReadOnly);

    Eigen::MatrixXd matrix;

    hf.read("dataset_name", matrix);
    
    std::cout << "Matrix read: " << std::endl;
    std::cout << matrix << std::endl;
}

int main() {
    write_data();
    read_data();
    return 0;
}
~~~

## HDF5 Primer

An HDF5 file is a container for two kinds of objects:

1. **datasets** - array like collections of data (Eigen data in this case)
2. **groups** - folder like containers that hold other datasets or groups

    The important thing to remember is that within an HDF5 file, data is stored in a unix like file structure consisiting of a root group, `/` , and nested groups/datasets.

You can create any arbitrary number of  groups:

~~~
HDF5::File hf = HDF5::File("filename.hdf5", HDF5::File::Truncate);

HDF5::Group group1 = hf.group("group_name");
HDF5::Group group2 = group1.group("group_name");
~~~

And any number of **datasets** within the groups, or within the root group (file)

~~~
HDF5::DataSet group1_dataset = group1.dataset("dataset_name", matrix);
HDF5::DataSet group2_dataset = group2.dataset("dataset_name", matrix);
HDF5::DataSet hf_dataset = hf.dataset("dataset_name", matrix);
~~~

Finally, if you only want to read/write directly without creating additional objects

~~~
group1.write("dataset_name", matrix);
hf.write("dataset_name", matrix);
~~~

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

## Eigen Sizes that are/will be implemented

Also all integers versions

VectorXd - Matrix<double, -1, 1>
VectorXi - Matrix<int, -1, 1>

RowVectorXd - Matrix<double, 1, -1>
RowVectorXi - Matrix<int, 1, -1>

Matrix<double, -1, -1>
Matrix<double, -1, 3>

Matrix<double, 3, -1> 


Matrix<double, 1, 18>
Matirx<double, 18, 1>

Matrix<double, 3, 1>
Matrix<double, 1, 3>

Matrix<double, 3, 3>
All above implemented fro teh file

Matrix<double, 4, 1>
Matrix<double, 1, 4>

Matrix<double, 15, 15>

## Inspiration

This borrows heavily from lots of other great work:

* [eigen3-hdf5](https://github.com/garrison/eigen3-hdf5)
