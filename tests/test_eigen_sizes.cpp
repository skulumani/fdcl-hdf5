#include "hdf5.hpp"

#include <gtest/gtest.h>

#include <iostream>

// File Testing
TEST(TestFDCLUAVFile, VectorXd) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::VectorXd mat, mat_read;
    mat = Eigen::VectorXd::Random(100, 1);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, VectorXi) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::VectorXi mat, mat_read;
    mat = Eigen::VectorXi::Random(100, 1);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, RowVectorXd) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::RowVectorXd mat, mat_read;
    mat = Eigen::RowVectorXd::Random(1, 100);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, RowVectorXi) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::VectorXi mat, mat_read;
    /* mat = Eigen::RowVectorXi::Random(1, 100); */
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, MatrixDynamicRowsandColsDouble) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, -1, -1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(124, 13);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, MatrixDynamicRowsandColsInt) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<int, -1, -1> mat, mat_read;
    mat = Eigen::MatrixXi::Random(12, 32874);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, MatrixNby3Double) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, -1, 3> mat, mat_read;
    mat = Eigen::MatrixXd::Random(501, 3);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, MatrixNby3Int) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<int, -1, 3> mat, mat_read;
    mat = Eigen::MatrixXi::Random(2, 3);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix3xNDouble) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 3, -1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(3, 123);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix3xNInt) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<int, 3, -1> mat, mat_read;
    mat = Eigen::MatrixXi::Random(3, 12);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix1x18Double) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 1, 18> mat, mat_read;
    mat << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18;
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix1x18Int) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<int, 1, 18> mat, mat_read;
    mat << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18;
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix18x1Double) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 18, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(18, 1);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix18x1Int) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<int, 18, 1> mat, mat_read;
    mat = Eigen::MatrixXi::Random(18, 1);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix3x1Double) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 3, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(3, 1);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix3x1Int) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<int, 3, 1> mat, mat_read;
    mat = Eigen::MatrixXi::Random(3, 1);
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Matrix1x3Double) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 1, 3> mat, mat_read;
    mat << 1, 2, 3;
    hf_file.write("matrix", mat);
    hf_file.read("matrix", mat_read);
    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Eigen3by3) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 3, 3> mat, mat_read;
    mat = Eigen::MatrixXd::Random(3, 3);
    hf_file.write("matrix", mat);
    
    hf_file.read("matrix", mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVFile, Eigen4by1) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 4, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(4, 1);
    hf_file.write("matrix", mat);
    
    hf_file.read("matrix", mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}

// Group testing
TEST(TestFDCLUAVGroup, Eigen3by1) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    HDF5::Group hf_group = hf_file.group("data");
    Eigen::Matrix<double, 3, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(3, 1);
    hf_group.write("matrix", mat);
    
    hf_group.read("matrix", mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVGroup, Eigen3by3) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    HDF5::Group hf_group = hf_file.group("data");
    Eigen::Matrix<double, 3, 3> mat, mat_read;
    mat = Eigen::MatrixXd::Random(3, 3);
    hf_group.write("matrix", mat);
    
    hf_group.read("matrix", mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVGroup, Eigen4by1) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    HDF5::Group hf_group = hf_file.group("data");
    Eigen::Matrix<double, 4, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(4, 1);
    hf_group.write("matrix", mat);
    
    hf_group.read("matrix", mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}

// Dataset Testing
TEST(TestFDCLUAVDataSet, Eigen3by1) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 3, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(3, 1);

    HDF5::DataSet hf_dataset = hf_file.dataset("matrix", mat);
    
    hf_dataset.read( mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVDataSet, Eigen3by3) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 4, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(4, 1);

    HDF5::DataSet hf_dataset = hf_file.dataset("matrix", mat);
    
    hf_dataset.read( mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}

TEST(TestFDCLUAVDataSet, Eigen4by1) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::Matrix<double, 4, 1> mat, mat_read;
    mat = Eigen::MatrixXd::Random(4, 1);

    HDF5::DataSet hf_dataset = hf_file.dataset("matrix", mat);
    
    hf_dataset.read( mat_read);

    ASSERT_TRUE(mat_read.isApprox(mat));
}
