#ifndef HDF5_GROUP_H
#define HDF5_GROUP_H

#include <Eigen/Dense>
#include "H5Cpp.h"

#include <memory>

namespace HDF5 {
    class File;
    class DataSet;
class Group {
    // create a new dataset inside the group
    // create attribute in the group
    public:
        Group( void );
        virtual ~Group(void);

        Group(const File* file, const std::string& group_name);
        Group(const Group* group, const std::string& group_name);
    
        DataSet dataset(const std::string& name) const;

        template<typename Derived>
        DataSet dataset(const std::string& name, const Eigen::EigenBase<Derived>& mat) const;

        template<typename Derived>
        int write(const std::string& dataset_name, const Eigen::EigenBase<Derived>& mat);

        template<typename Derived>
        int read(const std::string& dataset_name, const Eigen::DenseBase<Derived>& mat);

        // TODO Create dataset method
        std::shared_ptr<H5::Group> group_ptr;
};
} // namespace HDF5
#endif
