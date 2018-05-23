#include "hdf5.hpp"

#include <iostream>

namespace HDF5 {
   
    const int File::ReadOnly; /**< Read only access */
    const int File::ReadWrite; /**< ReadWrite access */
    const int File::Truncate; /**< Overwrite a file if it exists or create a new one */
    const int File::Excl; /**< Only open if the file doesn't exist */
    const int File::Create; /**< Create a new file */
    File::~File ( void ) {
        file_ptr->close();
    }
    
    File::File( const std::string& file_name , const int open_flag ) {

        // logic to open the file based on open_flag
        if (open_flag == 0) {
            file_ptr = std::make_shared<H5::H5File>(file_name, H5F_ACC_RDONLY);
        } else if (open_flag == 1) {
            file_ptr = std::make_shared<H5::H5File>(file_name, H5F_ACC_RDWR);
        } else if ( open_flag == 2) {
            file_ptr = std::make_shared<H5::H5File>(file_name, H5F_ACC_TRUNC);
        } else if ( open_flag == 3 ) {
            file_ptr = std::make_shared<H5::H5File>(file_name,H5F_ACC_EXCL);
        } else if ( open_flag == 4 ) {
            file_ptr = std::make_shared<H5::H5File>(file_name,H5F_ACC_CREAT);
        }
        
    }

    Group File::group(const std::string& group_name) const {
        return Group(this, group_name);
    }

    DataSet File::dataset(const std::string& dataset_name) const {
        return DataSet(this, dataset_name);
    }

    template<typename Derived>
    DataSet File::dataset(const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat) const {
        return DataSet(this, dataset_name, mat);
    }
    
    DataSet File::open_dataset(const std::string& dataset_name) const {
        return DataSet(this, dataset_name);
    }
    
    template<typename Derived>
    DataSet File::read_dataset(const std::string& dataset_name, const Eigen::DenseBase<Derived> &mat) const {
        return DataSet(this, dataset_name, mat);
    }
    
    template<typename Derived>
    DataSet File::write_dataset(const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat) const {
        return DataSet(this, dataset_name, mat);
    }

    template<typename Derived>
    int File::write(const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat) {
        // TODO Catch proper exceptions
        try {
            save(*file_ptr,dataset_name, mat); 
            return 0;
        } catch(...) {
            std::cout << "Error saving: " << dataset_name << std::endl;
            return 1;
        }
    }

    template<typename Derived>
    int File::read(const std::string& dataset_name, const Eigen::DenseBase<Derived> &mat) {
        // TODO Catch proper exceptions
        try {
            load(*file_ptr, dataset_name, mat);
            return 0;
        } catch(...) {
            std::cout << "Error loading: " << dataset_name << std::endl;
            return 0;
        }
    }

} // namespace HDF5

// File::write template specilization
template int HDF5::File::write<Eigen::Matrix<double, -1, 3> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > &mat);
template int HDF5::File::write<Eigen::Matrix<int, -1, 3> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > &mat);

template int HDF5::File::write<Eigen::Matrix<double, -1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &mat);
template int HDF5::File::write<Eigen::Matrix<int, -1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &mat);

template int HDF5::File::write<Eigen::Matrix<double, 1, 18> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &mat);
template int HDF5::File::write<Eigen::Matrix<int, 1, 18> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &mat);

template int HDF5::File::write<Eigen::Matrix<double, 1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &mat);
template int HDF5::File::write<Eigen::Matrix<int, 1, -1> >(const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &mat);
// File::read template specialization
template int HDF5::File::read<Eigen::Matrix<double, -1, 3> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, -1, 3> > &mat);
template int HDF5::File::read<Eigen::Matrix<int, -1, 3> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, -1, 3> > &mat);

template int HDF5::File::read<Eigen::Matrix<double, -1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, -1, -1> > &mat);
template int HDF5::File::read<Eigen::Matrix<int, -1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, -1, -1> > &mat);

template int HDF5::File::read<Eigen::Matrix<double, 1, 18> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, 1, 18> > &mat);
template int HDF5::File::read<Eigen::Matrix<int, 1, 18> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, 1, 18> > &mat);

template int HDF5::File::read<Eigen::Matrix<double, 1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, 1, -1> > &mat);
template int HDF5::File::read<Eigen::Matrix<int, 1, -1> >(const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, 1, -1> > &mat);
// File::read_dataset template
template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<double, -1, 3> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<double, -1, 3> >& mat) const;
template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<int, -1, 3> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<int, -1, 3> >& mat) const;

template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<double, -1, -1> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<double, -1, -1> >& mat) const;
template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<int, -1, -1> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<int, -1, -1> >& mat) const;

template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<double, 1, 18> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<double, 1, 18> >& mat) const;
template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<int, 1, 18> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<int, 1, 18> >& mat) const;

template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<double, 1, -1> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<double, 1, -1> >& mat) const;
template HDF5::DataSet HDF5::File::read_dataset<Eigen::Matrix<int, 1, -1> >(const std::string& name, const Eigen::DenseBase<Eigen::Matrix<int, 1, -1> >& mat) const;
// File::dataset template specification
template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<double, -1, 3> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > & mat) const;
template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<int, -1, 3> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > & mat) const;

template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<double, -1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > & mat) const;
template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<int, -1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > & mat) const;

template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<double, 1, 18> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > & mat) const;
template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<int, 1, 18> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > & mat) const;

template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<double, 1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > & mat) const;
template HDF5::DataSet HDF5::File::dataset<Eigen::Matrix<int, 1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > & mat) const;
// File::write_dataset template specialization
template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<double, -1, 3> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > &) const;
template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<int, -1, 3> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > &) const;

template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<double, -1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &) const;
template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<int, -1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &) const;

template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<double, 1, 18> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &) const;
template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<int, 1, 18> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &) const;

template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<double, 1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &) const;
template HDF5::DataSet HDF5::File::write_dataset<Eigen::Matrix<int, 1, -1> >(const std::string& name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &) const;
