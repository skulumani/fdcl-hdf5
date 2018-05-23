#ifndef HDF5_FILE_H
#define HDF5_FILE_H

#include <Eigen/Dense>
#include "H5Cpp.h"

#include <memory>

namespace HDF5 {
    
    class File;
    class Group;
    class DataSet;
    
class File {
    public: 
        static const int ReadOnly = 0; /**< Read only access */
        static const int ReadWrite = 1; /**< ReadWrite access */
        static const int Truncate = 2; /**< Overwrite a file if it exists or create a new one */
        static const int Excl = 3; /**< Only open if the file doesn't exist */
        static const int Create = 4; /**< Create a new file */


        File( void );

        // close the file
        virtual ~File( void );
        
        /** @fn Open HDF5 file for reading/writing
                
            Can specify some options for the file

            @param file_name File name to open/create
            @param open_flag Choice of mode to open the file in

            @author Shankar Kulumani
            @version 23 April 2018
        */
        File(const std::string& file_name, const int open_flag = File::ReadOnly);
        
        const std::string getName( void ) const;
        
        Group group(const std::string& group_name) const;
        
        DataSet dataset(const std::string& dataset_name) const;
        template<typename Derived>
        DataSet dataset(const std::string& name, const Eigen::EigenBase<Derived>& mat) const;

        // Open exisiting dataset
        DataSet open_dataset(const std::string& dataset_name) const;

        // Open and read to variable an exisiting dataset
        template<typename Derived>
        DataSet read_dataset(const std::string& dataset_name, const Eigen::DenseBase<Derived> &mat) const;
        
        // create and write to dataset
        template<typename Derived>
        DataSet write_dataset(const std::string& dataset_name, const Eigen::EigenBase<Derived> &mat) const;

        // TODO Add attribute saving
        // TODO Add saving scalar double, int, strings
        // create a dataset and return HDF5DataSet
        template<typename Derived>
        int write(const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat);
        
        template<typename Derived>
        int read(const std::string& dataset_name, const Eigen::DenseBase<Derived> &mat);

        // create attribute
        
        std::shared_ptr<H5::H5File> file_ptr; /**< HDF5 file to save data */
};

} // namespace HDF5
#endif
