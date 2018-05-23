#include "hdf5.hpp"

#include <iostream>

namespace HDF5 {

    Group::Group( void ) {
        group_ptr = std::make_shared<H5::Group>();
    }

    Group::~Group( void ) {
        group_ptr->close();
    }
    
    Group::Group(const File* file, const std::string& group_name) {
        try {
            H5::Exception::dontPrint();
            group_ptr = std::make_shared<H5::Group>(file->file_ptr->openGroup(group_name)); 
        } catch(const H5::FileIException& err_does_not_exist) {
            group_ptr = std::make_shared<H5::Group>(file->file_ptr->createGroup(group_name));
        }	 
    }
    
    Group::Group(const Group* group, const std::string& group_name) {
        try {
            H5::Exception::dontPrint();
            group_ptr = std::make_shared<H5::Group>(group->group_ptr->openGroup(group_name));
        } catch(const H5::GroupIException err_does_not_exist) {
            group_ptr = std::make_shared<H5::Group>(group->group_ptr->createGroup(group_name));
        }
    }
   
    DataSet Group::dataset(const std::string& dataset_name) const {
        return DataSet(this, dataset_name);
    }

    template<typename Derived>
    DataSet Group::dataset(const std::string& name, const Eigen::EigenBase<Derived>& mat) const {
        return DataSet(this, name, mat);
    }

    template<typename Derived>
    int Group::write(const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat) {
        // TODO Catch proper exceptions
        try {
            save(*group_ptr,dataset_name, mat); 
            return 0;
        } catch(...) {
            std::cout << "Error saving: " << dataset_name << std::endl;
            return 1;
        }
    }

    template<typename Derived>
    int Group::read(const std::string& dataset_name, const Eigen::DenseBase<Derived> &mat) {
        // TODO Catch proper exceptions
        try {
            load(*group_ptr, dataset_name, mat);
            return 0;
        } catch(...) {
            std::cout << "Error loading: " << dataset_name << std::endl;
            return 0;
        }
    }
} // namespace HDF5

// Group::dataset template specification
template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<double, -1, 3> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > & mat) const;
template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<int, -1, 3> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > & mat) const;

template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<double, -1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > & mat) const;
template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<int, -1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > & mat) const;

template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<double, 1, 18> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > & mat) const;
template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<int, 1, 18> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > & mat) const;

template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<double, 1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > & mat) const;
template HDF5::DataSet HDF5::Group::dataset<Eigen::Matrix<int, 1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > & mat) const;
// Group::write template specilization
template int HDF5::Group::write<Eigen::Matrix<double, -1, 3> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > &mat);
template int HDF5::Group::write<Eigen::Matrix<int, -1, 3> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > &mat);

template int HDF5::Group::write<Eigen::Matrix<double, -1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &mat);
template int HDF5::Group::write<Eigen::Matrix<int, -1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &mat);

template int HDF5::Group::write<Eigen::Matrix<double, 1, 18> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &mat);
template int HDF5::Group::write<Eigen::Matrix<int, 1, 18> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &mat);

template int HDF5::Group::write<Eigen::Matrix<double, 1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &mat);
template int HDF5::Group::write<Eigen::Matrix<int, 1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &mat);
// Group::read template specialization
template int HDF5::Group::read<Eigen::Matrix<double, -1, 3> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, -1, 3> > &mat);
template int HDF5::Group::read<Eigen::Matrix<int, -1, 3> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, -1, 3> > &mat);

template int HDF5::Group::read<Eigen::Matrix<double, -1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, -1, -1> > &mat);
template int HDF5::Group::read<Eigen::Matrix<int, -1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, -1, -1> > &mat);

template int HDF5::Group::read<Eigen::Matrix<double, 1, 18> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, 1, 18> > &mat);
template int HDF5::Group::read<Eigen::Matrix<int, 1, 18> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, 1, 18> > &mat);

template int HDF5::Group::read<Eigen::Matrix<double, 1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, 1, -1> > &mat);
template int HDF5::Group::read<Eigen::Matrix<int, 1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, 1, -1> > &mat);
