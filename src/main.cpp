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
