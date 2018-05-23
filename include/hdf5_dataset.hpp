#ifndef HDF5_DATASET_H
#define HDF5_DATASET_H

#include <Eigen/Dense>
#include "H5Cpp.h"

#include <memory>

namespace HDF5 {
    // Ahead of time declaration
    class Group;
    class File;
    
    class DataSet {
        public:
            DataSet( void );
            virtual ~DataSet( void );

            // Simply open a dataset with another function to read
            DataSet(const File* file, const std::string& dataset_name);
            DataSet(const Group* group, const std::string& dataset_name);

            // read an already opened dataset to a new variable
            template<typename Derived>
            int read(const Eigen::DenseBase<Derived>& mat);
            
            template<typename Derived>
            int write(const Eigen::EigenBase<Derived>& mat);

            // open and read to variable or create a dataset if not exisiting
            template<typename Derived>
            DataSet(const File* file, const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat);
            template<typename Derived>
            DataSet(const Group* group, const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat);

            std::shared_ptr<H5::DataSet> dataset_ptr;
    };



}
#endif
