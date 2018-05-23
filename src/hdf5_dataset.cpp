#include "hdf5.hpp"

#include <iostream>

// class definitions for working with HDF5
namespace HDF5 {
     
    DataSet::DataSet( void ) {
        dataset_ptr = std::make_shared<H5::DataSet>();
    }

    DataSet::~DataSet( void ) {
        dataset_ptr->close();
    }
    
    DataSet::DataSet(const File* file, const std::string& dataset_name) {
        dataset_ptr = std::make_shared<H5::DataSet>();
        try {
            H5::Exception::dontPrint();
            dataset_ptr = std::make_shared<H5::DataSet>(file->file_ptr->openDataSet(dataset_name));
        } catch ( const H5::FileIException& err_does_not_exist) {
            dataset_ptr.reset();
            std::cout << "DataSet: " << dataset_name << " does not exist" << std::endl;
        }
    }

    DataSet::DataSet(const Group* group, const std::string& dataset_name) {
        dataset_ptr = std::make_shared<H5::DataSet>();
        try {
            H5::Exception::dontPrint();
            dataset_ptr = std::make_shared<H5::DataSet>(group->group_ptr->openDataSet(dataset_name));
        } catch ( const H5::GroupIException& err_does_not_exist) {
            dataset_ptr.reset();
            std::cout << "DataSet: " << dataset_name << " does not exist" << std::endl;
        }
    }
    
    template<typename Derived>
    int DataSet::read(const Eigen::DenseBase<Derived>& mat) {
        try {
            internal::_load(*dataset_ptr, mat);  
            return 0;
        } catch(...) {
            std::cout << "Error reading dataset" << std::endl;
            return 1;
        }
    }
    
    template<typename Derived>
    DataSet::DataSet(const File* file, const std::string& dataset_name, const Eigen::EigenBase<Derived> &mat) {
        dataset_ptr = std::make_shared<H5::DataSet>();
        try {
            H5::Exception::dontPrint();
            dataset_ptr = std::make_shared<H5::DataSet>(file->file_ptr->openDataSet(dataset_name));
            internal::_load(*dataset_ptr, mat.derived());
        } catch( const H5::FileIException& err_does_not_exist) {
            // create a new dataset and save to pointer in this
            typedef typename Derived::Scalar Scalar;
            const H5::DataType * const datatype = DatatypeSpecialization<Scalar>::get();
            const H5::DataSpace dataspace = internal::create_dataspace(mat);
            const H5::DSetCreatPropList plist = H5::DSetCreatPropList::DEFAULT;

            dataset_ptr = std::make_shared<H5::DataSet>(file->file_ptr->createDataSet(dataset_name, *datatype, dataspace, plist));

            bool written = false;  // flag will be true when the data has been written
            if (mat.derived().Flags & Eigen::RowMajor)
            {
                written = internal::write_rowmat(mat, datatype, dataset_ptr.get(), &dataspace);
            }
            else
            {
                written = internal::write_colmat(mat, datatype, dataset_ptr.get(), &dataspace);
            }
            
            if (!written)
            {
                // data has not yet been written, so there is nothing else to try but copy the input
                // matrix to a row major matrix and write it. 
                const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> row_major_mat(mat);
                dataset_ptr->write(row_major_mat.data(), *datatype);
            }
                }
    }

    template<typename Derived>
    DataSet::DataSet(const Group* group, const std::string& dataset_name, const Eigen::EigenBase<Derived> &mat) {
        dataset_ptr = std::make_shared<H5::DataSet>();
        try {
            H5::Exception::dontPrint();
            dataset_ptr = std::make_shared<H5::DataSet>(group->group_ptr->openDataSet(dataset_name));
            internal::_load(*dataset_ptr, mat.derived());
        } catch( const H5::GroupIException& err_does_not_exist) {
            // create a new dataset and save to pointer in this
            typedef typename Derived::Scalar Scalar;
            const H5::DataType * const datatype = DatatypeSpecialization<Scalar>::get();
            const H5::DataSpace dataspace = internal::create_dataspace(mat);
            const H5::DSetCreatPropList plist = H5::DSetCreatPropList::DEFAULT;

            dataset_ptr = std::make_shared<H5::DataSet>(group->group_ptr->createDataSet(dataset_name, *datatype, dataspace, plist));

            bool written = false;  // flag will be true when the data has been written
            if (mat.derived().Flags & Eigen::RowMajor)
            {
                written = internal::write_rowmat(mat, datatype, dataset_ptr.get(), &dataspace);
            }
            else
            {
                written = internal::write_colmat(mat, datatype, dataset_ptr.get(), &dataspace);
            }
            
            if (!written)
            {
                // data has not yet been written, so there is nothing else to try but copy the input
                // matrix to a row major matrix and write it. 
                const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> row_major_mat(mat);
                dataset_ptr->write(row_major_mat.data(), *datatype);
            }
                }
    }



} // namespace HDF5

// DataSet::read template specialization
template int HDF5::DataSet::read<Eigen::Matrix<double, -1, 3> >(const Eigen::DenseBase<Eigen::Matrix<double, -1, 3> > &mat);
template int HDF5::DataSet::read<Eigen::Matrix<int, -1, 3> >(const Eigen::DenseBase<Eigen::Matrix<int, -1, 3> > &mat);

template int HDF5::DataSet::read<Eigen::Matrix<double, -1, -1> >(const Eigen::DenseBase<Eigen::Matrix<double, -1, -1> > &mat);
template int HDF5::DataSet::read<Eigen::Matrix<int, -1, -1> >(const Eigen::DenseBase<Eigen::Matrix<int, -1, -1> > &mat);

template int HDF5::DataSet::read<Eigen::Matrix<double, 1, 18> >(const Eigen::DenseBase<Eigen::Matrix<double, 1, 18> > &mat);
template int HDF5::DataSet::read<Eigen::Matrix<int, 1, 18> >(const Eigen::DenseBase<Eigen::Matrix<int, 1, 18> > &mat);

template int HDF5::DataSet::read<Eigen::Matrix<double, 1, -1> >(const Eigen::DenseBase<Eigen::Matrix<double, 1, -1> > &mat);
template int HDF5::DataSet::read<Eigen::Matrix<int, 1, -1> >(const Eigen::DenseBase<Eigen::Matrix<int, 1, -1> > &mat);
// DataSet::DataSet for Files
template HDF5::DataSet::DataSet<Eigen::Matrix<double, -1, 3> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, -1, 3> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > &mat);

template HDF5::DataSet::DataSet<Eigen::Matrix<double, -1, -1> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, -1, -1> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &mat);

template HDF5::DataSet::DataSet<Eigen::Matrix<double, 1, 18> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, 1, 18> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &mat);

template HDF5::DataSet::DataSet<Eigen::Matrix<double, 1, -1> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, 1, -1> >(const HDF5::File *file, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &mat);
// DataSet::DataSet for Groups
template HDF5::DataSet::DataSet<Eigen::Matrix<double, -1, 3> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, -1, 3> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > &mat);

template HDF5::DataSet::DataSet<Eigen::Matrix<double, -1, -1> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, -1, -1> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &mat);

template HDF5::DataSet::DataSet<Eigen::Matrix<double, 1, 18> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, 1, 18> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &mat);

template HDF5::DataSet::DataSet<Eigen::Matrix<double, 1, -1> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &mat);
template HDF5::DataSet::DataSet<Eigen::Matrix<int, 1, -1> >(const HDF5::Group *group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &mat);
