Installation
=====================

This library serves as an interface wrapper for using the HDF5 library with Eigen objects.
As a result, all Eigen Matrix types are able to be written to HDF5 objects and subsequently read back. 
In addition, all HDF5 structures, such as files, groups, and datasets are compatible with Eigen data types. 

Dependencies
==============

This library depends on the following:

1. `Eigen`_ - A C++ template library for linear algebra.
2. `HDF5`_ - A file format to store and organize large amounts of data.
3. `CMake`_ - A cross-platform build tool.

.. _Eigen: http://eigen.tuxfamily.org/index.php?title=Main_Page 
.. _HDF5: https://www.hdfgroup.org/solutions/hdf5/
.. _CMake: https://cmake.org/

Build Scripts
===================

Several ``bash`` scripts are available which simplify the dependency installtion process. 
Usage of these scripts is not required if the dependencies are already installed or managed seperately.

Build and Installation
========================
