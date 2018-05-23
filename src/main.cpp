#include "hdf5.hpp"

#include <Eigen/Dense>

#include <memory>
#include <iostream>
int main() {
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
