Basic Usage
=========================

This file is a convinient header to include all of the HDF5 Eigen functionality into your program. 
You can utilize it by adding the following to your source code and ensuring that the library is available to your compiler

.. code-block:: cpp

    #include "hdf5.hpp"

Here is a simple program which shows how to write and read an Eigen matrix from an HDF5 data file.

.. code-block:: cpp

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

The ``write_data()`` function is used to write an ``Eigen::MatrixXd`` object to an ``HDF5::File hf`` instance. 
Likewise, the ``read_data()`` function then opens the same file, in a read-only mode, and print the matrix to the screen.


