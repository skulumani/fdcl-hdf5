/**
    All Eigen functionality to read/write to HDF5

    Updated/corrected from the work of Jim Garrison
    https://github.com/garrison/eigen3-hdf5

    @author Shankar Kulumani
    @version 26 April 2018
*/
#ifndef HDF5_EIGEN_H
#define HDF5_EIGEN_H

#include <Eigen/Dense>
#include "H5Cpp.h"

#include <stdexcept>

/** @class DatatypeSpecialization

    @brief Convert a C++ datatype to the equivalent in HDF5 land
    
    Each dataset must define the correct type in the HDF5. 
    This template class handles that by automatially determining the HDF5 type
    from a Eigen input.

    Setup primiarly for Eigen types but needs to be extended.

    @author Shankar Kulumani
    @version 26 April 2018
*/
template <typename T>
struct DatatypeSpecialization;

// floating-point types

template <>
struct DatatypeSpecialization<float> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_FLOAT;
    }
};

template <>
struct DatatypeSpecialization<double> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_DOUBLE;
    }
};

template <>
struct DatatypeSpecialization<long double> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_LDOUBLE;
    }
};

// integer types

template <>
struct DatatypeSpecialization<short> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_SHORT;
    }
};

template <>
struct DatatypeSpecialization<unsigned short> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_USHORT;
    }
};

template <>
struct DatatypeSpecialization<int> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_INT;
    }
};

template <>
struct DatatypeSpecialization<unsigned int> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_UINT;
    }
};

template <>
struct DatatypeSpecialization<long> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_LONG;
    }
};

template <>
struct DatatypeSpecialization<unsigned long> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_ULONG;
    }
};

template <>
struct DatatypeSpecialization<long long> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_LLONG;
    }
};

template <>
struct DatatypeSpecialization<unsigned long long> {
    static inline const H5::DataType * get (void) {
        return &H5::PredType::NATIVE_ULLONG;
    }
};

// string types, to be used mainly for attributes

template <>
struct DatatypeSpecialization<const char *> {
    static inline const H5::DataType * get (void) {
        static const H5::StrType strtype(0, H5T_VARIABLE);
        return &strtype;
    }
};

template <>
struct DatatypeSpecialization<char *> {
    static inline const H5::DataType * get (void){
        static const H5::StrType strtype(0, H5T_VARIABLE);
        return &strtype;
    }
};

// XXX: for some unknown reason the following two functions segfault if
// H5T_VARIABLE is used.  The passed strings should still be null-terminated,
// so this is a bit worrisome.

template <std::size_t N>
struct DatatypeSpecialization<const char [N]> {
    static inline const H5::DataType * get (void) {
        static const H5::StrType strtype(0, N);
        return &strtype;
    }
};

template <std::size_t N>
struct DatatypeSpecialization<char [N]> {
    static inline const H5::DataType * get (void) {
        static const H5::StrType strtype(0, N);
        return &strtype;
    }
};

/** @fn void load(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Derived> &mat)
        
    This will read from an HDF5 file/group and save the data into an eigen matrix. 
    One should have some idea about the eigen array before reading

    @param h5group H5 group or file to read from
    @param name String name of the dataset
    @param mat Eigen variable to save data

    @author Shankar Kulumani
    @version 26 April 2018
*/
template <typename Derived>
void load (const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Derived> &mat);

/** @fn void save(H5::H5Location &h5group, const std::string &name, const Eigen::EigenBase<Derived> &mat, const H5::DSetCreatPropList &plist=H5::DSetCreatPropList::DEFAULT)
        
    This will write Eigen data to an HDF5 file or group

    @param h5group The H5 group or file to write to
    @param name string name of the dataset
    @param mat Eigen variable to save data
    @param plist The dataset property list
    @author Shankar Kulumani
    @version 26 April 2018
*/
template <typename Derived>
void save (H5::H5Location &h5group, const std::string &name, 
        const Eigen::EigenBase<Derived> &mat,
        const H5::DSetCreatPropList &plist=H5::DSetCreatPropList::DEFAULT);


namespace internal
{
    /** @fn H5::DataSpace create_dataspace(const Eigen::EigenBase<Derived> &mat)
            
        Create the dataspace required for the HDF5 dataset

        @param mat Eigen matrix to write to the file
        @returns H5::DataSpace Defines the space required to store mat

        @author Shankar Kulumani
        @version 26 April 2018
    */
    template <typename Derived>
    H5::DataSpace create_dataspace (const Eigen::EigenBase<Derived> &mat);

    template <typename Derived>
    bool write_rowmat(const Eigen::EigenBase<Derived> &mat,
        const H5::DataType * const datatype,
        H5::DataSet *dataset,
        const H5::DataSpace* dspace);

    template <typename Derived>
    bool write_colmat(const Eigen::EigenBase<Derived> &mat,
        const H5::DataType * const datatype,
        H5::DataSet *dataset,
        const H5::DataSpace* dspace);

    // H5::Attribute and H5::DataSet both have similar API's, and although they
    // share a common base class, the relevant methods are not virtual.  Worst
    // of all, they take their arguments in different orders!

    template <typename Scalar>
    inline void read_data (const H5::DataSet &dataset, Scalar *data, const H5::DataType &datatype) {
        dataset.read(data, datatype);
    }

    template <typename Scalar>
    inline void read_data (const H5::Attribute &dataset, Scalar *data, const H5::DataType &datatype) {
        dataset.read(datatype, data);
    }

    // read a column major attribute; I do not know if there is an hdf routine to read an
    // attribute hyperslab, so I take the lazy way out: just read the conventional hdf
    // row major data and let eigen copy it into mat. 
    template <typename Derived>
    bool read_colmat(const Eigen::DenseBase<Derived> &mat,
        const H5::DataType * const datatype,
        const H5::Attribute &dataset);

    template <typename Derived>
    bool read_colmat(const Eigen::DenseBase<Derived> &mat,
        const H5::DataType * const datatype,
        const H5::DataSet &dataset);

    template <typename Derived, typename DataSet>
        void _load (const DataSet &dataset, const Eigen::DenseBase<Derived> &mat);
}
#endif
