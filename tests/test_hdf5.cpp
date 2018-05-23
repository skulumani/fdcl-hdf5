#include "hdf5.hpp"

#include <gtest/gtest.h>

#include <iostream>

// DATASET Testing
TEST(TestHDF5DataSet, OpenDataSetFromFile) {
    // first create a file with a dataset
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_file_ptr->write("matrix", mat);
    
    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));

    // now test dataset constructor
    HDF5::DataSet hf_dataset(hf_file_ptr.get(), "matrix");
}

TEST(TestHDF5DataSet, OpenDataSetFromGroup) {
    // first create a file with a dataset
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    std::shared_ptr<HDF5::Group> hf_group_ptr = std::make_shared<HDF5::Group>(hf_file_ptr.get(), "group");
    
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_group_ptr->write("matrix", mat);

    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    hf_group_ptr.reset(new HDF5::Group(hf_file_ptr.get(), "group")); 
    // now test dataset constructor
    HDF5::DataSet hf_dataset(hf_group_ptr.get(), "matrix");
    hf_dataset.read(mat_load);
    ASSERT_TRUE(mat.isApprox(mat_load));
}

TEST(TestHDF5DataSet, ReadDataSetFromFile) {
    // first create a file with a dataset
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_file_ptr->write("matrix", mat);
    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    // now test dataset constructor
    HDF5::DataSet hf_dataset(hf_file_ptr.get(), "matrix", mat_load);
    ASSERT_TRUE(mat.isApprox(mat_load));
}

TEST(TestHDF5DataSet, ReadDataSetFromGroup) {
    // first create a file with a dataset
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    std::shared_ptr<HDF5::Group> hf_group_ptr = std::make_shared<HDF5::Group>(hf_file_ptr.get(), "group");
    
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_group_ptr->write("matrix", mat);

    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    hf_group_ptr.reset(new HDF5::Group(hf_file_ptr.get(), "group")); 
    // now test dataset constructor
    HDF5::DataSet hf_dataset(hf_group_ptr.get(), "matrix", mat_load);
    ASSERT_TRUE(mat.isApprox(mat_load));
}

TEST(TestHDF5DataSet, WriteDataSetFromFile) {
    // first create a file with a dataset
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    std::shared_ptr<HDF5::DataSet> hf_dataset_ptr = std::make_shared<HDF5::DataSet>(hf_file_ptr.get(), "matrix", mat);

    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    hf_dataset_ptr.reset(new HDF5::DataSet(hf_file_ptr.get(), "matrix", mat_load));
    // now test dataset constructor
    ASSERT_TRUE(mat.isApprox(mat_load));

}

TEST(TestHDF5DataSet, WriteDataSetFromGroup) {
    // first create a file with a dataset
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    std::shared_ptr<HDF5::Group> hf_group_ptr = std::make_shared<HDF5::Group>(hf_file_ptr.get(), "group");
    
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    std::shared_ptr<HDF5::DataSet> hf_dataset_ptr = std::make_shared<HDF5::DataSet>(hf_group_ptr.get(), "matrix", mat);

    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    hf_group_ptr.reset(new HDF5::Group(hf_file_ptr.get(), "group")); 
    hf_dataset_ptr.reset(new HDF5::DataSet(hf_group_ptr.get(), "matrix", mat_load));
    // now test dataset constructor
    ASSERT_TRUE(mat.isApprox(mat_load));

}
TEST(TestHDF5Group, PointerGroupWrite) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    std::shared_ptr<HDF5::Group> hf_group_ptr = std::make_shared<HDF5::Group>(hf_file_ptr.get(), "group");
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    int write_flag = hf_group_ptr->write("matrix", mat);
    ASSERT_EQ(write_flag, 0);
}


TEST(TestHDF5Group, GroupWrite) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    HDF5::Group hf_group(hf_file_ptr.get(), "group");
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    int write_flag = hf_group.write("matrix", mat);
    ASSERT_EQ(write_flag, 0);
}

TEST(TestHDF5Group, GroupFromGroup) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    std::shared_ptr<HDF5::Group> hf_group_ptr = std::make_shared<HDF5::Group>(hf_file_ptr.get(), "group");
    std::shared_ptr<HDF5::Group> hf_subgroup_ptr = std::make_shared<HDF5::Group>(hf_group_ptr.get(), "subgroup");
}

TEST(TestHDF5Group, ReturnDataSetFromGroup) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    std::shared_ptr<HDF5::Group> hf_group_ptr = std::make_shared<HDF5::Group>(hf_file_ptr.get(), "group");

    // now make a dataset inside the group
    Eigen::MatrixXd mat(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    HDF5::DataSet hf_dataset = hf_group_ptr->dataset("matrix", mat);

}

TEST(TestEigenHDF5, SaveData) {
    H5::H5File hf_file("/tmp/test.hdf5", H5F_ACC_TRUNC);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    save(hf_file, "matrix", mat);
}

TEST(TestHDF5File, CreateFile) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate); 
}

TEST(TestHDF5File, CreateGroupinFile) {
    std::shared_ptr<HDF5::File> hf_file = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    HDF5::Group hf_group = hf_file->group("group_test");
}
TEST(TestHDF5File, FileDataSet) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_file.write("matrix", mat);

    hf_file.read("matrix", mat_load);

    ASSERT_TRUE(mat.isApprox(mat_load));

}

TEST(TestHDF5File, OpenDataSet) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_file_ptr->write("matrix", mat);
    
    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    // now open and read the data into a dataset
    HDF5::DataSet hf_dataset = hf_file_ptr->open_dataset("matrix");
}

TEST(TestHDF5File, OpenAndReadDataSet) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_file_ptr->write("matrix", mat);
    
    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    // now open and read dataset directly
    HDF5::DataSet hf_dataset = hf_file_ptr->read_dataset("matrix", mat_load);
    ASSERT_TRUE(mat.isApprox(mat_load));
}

TEST(TestHDF5File, WriteDataSet) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    
    /* HDF5::DataSet hf_dataset(hf_file_ptr.get(), "matrix", mat); */
    /* HDF5::DataSet hf_dataset = hf_file_ptr->write_dataset("matrix", mat); */
    HDF5::DataSet hf_dataset();
    /* save(*(hf_file_ptr->file_ptr), "matrix", mat); */
}
TEST(TestHDF5Group, GroupDataSet) {
    HDF5::File hf_file("/tmp/test.hdf5", HDF5::File::Truncate);
    HDF5::Group hf_group = hf_file.group("group");
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_group.write("matrix", mat);

    hf_group.read("matrix", mat_load);

    ASSERT_TRUE(mat.isApprox(mat_load));

}

TEST(TestHDF5Group, GroupConstructor) {
    std::shared_ptr<HDF5::File> hf_file_ptr = std::make_shared<HDF5::File>("/tmp/test.hdf5", HDF5::File::Truncate);
    std::shared_ptr<HDF5::Group> hf_group_ptr = std::make_shared<HDF5::Group>(hf_file_ptr.get(), "/group");
    
    // write to group to check and make sure it's working
    Eigen::MatrixXd mat(1, 3), mat_load(1, 3);
    mat = Eigen::MatrixXd::Random(1, 3);
    hf_group_ptr->write("matrix", mat);
    
    // close the file by reset
    hf_file_ptr.reset(new HDF5::File("/tmp/test.hdf5", HDF5::File::ReadOnly));
    /* hf_group_ptr.reset(new HDF5::Group(hf_file_ptr.get(), "group")); */
    H5::Group hf_group(hf_file_ptr->file_ptr->openGroup("/group"));
}

TEST(TestH5, FileGroupCreation) {
    H5::H5File hf("/tmp/test.hdf5", H5F_ACC_TRUNC);
    H5::Group hf_group(hf.createGroup("group"));

    // open the group
    hf = H5::H5File("/tmp/test.hdf5", H5F_ACC_RDONLY);
    hf_group = H5::Group(hf.openGroup("group"));
}

