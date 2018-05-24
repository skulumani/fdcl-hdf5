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

    std::cout << matrix << std::endl;
}

int main() {

    // create a new HDF5 file, overwriting if it already exists
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::ReadWrite);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_file_ptr->write("matrix", mat);
    
    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));

    // now test dataset constructor
    HDF5::DataSet hf_dataset(hf_file_ptr.get(), "matrix");
    return 0;
}
