#include "hdf5.hpp"

namespace internal {
    template <typename Derived>
    H5::DataSpace create_dataspace (const Eigen::EigenBase<Derived> &mat) {

        const std::size_t dimensions_size = 2;
        const hsize_t dimensions[dimensions_size] = {
            static_cast<hsize_t>(mat.rows()),
            static_cast<hsize_t>(mat.cols())
        };
        return H5::DataSpace(dimensions_size, dimensions);
    }


	template <typename Derived>
	bool write_rowmat(const Eigen::EigenBase<Derived> &mat, 
					  const H5::DataType * const datatype,
				      H5::DataSet *dataset,
					  const H5::DataSpace* dspace) {
        if (mat.derived().innerStride() != 1)
        {
            // inner stride != 1 is an edge case this function does not (yet) handle. (I think it
            // could by using the inner stride as the first element of mstride below. But I do
            // not have a test case to try it out, so just return false for now.) 
            return false;
        }

        assert(mat.rows() >= 0);
        assert(mat.cols() >= 0);
        assert(mat.derived().outerStride() >= 0);
        hsize_t rows = hsize_t(mat.rows());
        hsize_t cols = hsize_t(mat.cols());
        hsize_t stride = hsize_t(mat.derived().outerStride());

        // slab params for the file data
        hsize_t fstride[2] = { 1, cols };

        // slab params for the memory data
        hsize_t mstride[2] = { 1, stride };

        // slab params for both file and memory data
        hsize_t count[2] = { 1, 1 };
        hsize_t block[2] = { rows, cols };
        hsize_t start[2] = { 0, 0 };

        // memory dataspace
        hsize_t mdim[2] = { rows, stride };
        H5::DataSpace mspace(2, mdim);

        dspace->selectHyperslab(H5S_SELECT_SET, count, start, fstride, block);
        mspace.selectHyperslab(H5S_SELECT_SET, count, start, mstride, block);
        dataset->write(mat.derived().data(), *datatype, mspace, *dspace);

        return true;
    }

    template <typename Derived>
        bool write_colmat(const Eigen::EigenBase<Derived> &mat,
                const H5::DataType * const datatype,
                H5::DataSet *dataset,
                const H5::DataSpace* dspace) {
            if (mat.derived().innerStride() != 1)
            {
                // inner stride != 1 is an edge case this function does not (yet) handle. (I think it
                // could by using the inner stride as the first element of mstride below. But I do
                // not have a test case to try it out, so just return false for now.) 
                return false;
            }

            assert(mat.rows() >= 0);
            assert(mat.cols() >= 0);
            assert(mat.derived().outerStride() >= 0);
            hsize_t rows = hsize_t(mat.rows());
            hsize_t cols = hsize_t(mat.cols());
            hsize_t stride = hsize_t(mat.derived().outerStride());

            // slab params for the file data
            hsize_t fstride[2] = { 1, cols };
            hsize_t fcount[2] = { 1, 1 };
            hsize_t fblock[2] = { 1, cols };

            // slab params for the memory data
            hsize_t mstride[2] = { stride, 1 };
            hsize_t mcount[2] = { 1, 1 };
            hsize_t mblock[2] = { cols, 1 };

            // memory dataspace
            hsize_t mdim[2] = { cols, stride };
            H5::DataSpace mspace(2, mdim);

            // transpose the column major data in memory to the row major data in the file by
            // writing one row slab at a time. 
            for (hsize_t i = 0; i < rows; i++)
            {
                hsize_t fstart[2] = { i, 0 };
                hsize_t mstart[2] = { 0, i };
                dspace->selectHyperslab(H5S_SELECT_SET, fcount, fstart, fstride, fblock);
                mspace.selectHyperslab(H5S_SELECT_SET, mcount, mstart, mstride, mblock);
                dataset->write(mat.derived().data(), *datatype, mspace, *dspace);
            }
            return true;
        }

    
    template <typename Derived>
    bool read_colmat(const Eigen::DenseBase<Derived> &mat,
            const H5::DataType * const datatype,
            const H5::Attribute &dataset) {
        typename Derived::Index rows = mat.rows();
        typename Derived::Index cols = mat.cols();
        typename Eigen::Matrix<typename Derived::Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> temp(rows, cols);
        internal::read_data(dataset, temp.data(), *datatype);
        const_cast<Eigen::DenseBase<Derived> &>(mat) = temp;
        return true;
    }

    template <typename Derived>
    bool read_colmat(const Eigen::DenseBase<Derived> &mat,
            const H5::DataType * const datatype,
            const H5::DataSet &dataset) {
        if (mat.derived().innerStride() != 1)
        {
            // inner stride != 1 is an edge case this function does not (yet) handle. (I think it
            // could by using the inner stride as the first element of mstride below. But I do
            // not have a test case to try it out, so just return false for now.) 
            return false;
        }

        assert(mat.rows() >= 0);
        assert(mat.cols() >= 0);
        assert(mat.derived().outerStride() >= 0);
        hsize_t rows = hsize_t(mat.rows());
        hsize_t cols = hsize_t(mat.cols());
        hsize_t stride = hsize_t(mat.derived().outerStride());

        if (stride != rows)
        {
            // this function does not (yet) read into a mat that has a different stride than the
            // dataset. 
            return false;
        }

        // slab params for the file data
        hsize_t fstride[2] = { 1, cols };
        hsize_t fcount[2] = { 1, 1 };
        hsize_t fblock[2] = { 1, cols };

        // file dataspace
        hsize_t fdim[2] = { rows, cols };
        H5::DataSpace fspace(2, fdim);

        // slab params for the memory data
        hsize_t mstride[2] = { stride, 1 };
        hsize_t mcount[2] = { 1, 1 };
        hsize_t mblock[2] = { cols, 1 };

        // memory dataspace
        hsize_t mdim[2] = { cols, stride };
        H5::DataSpace mspace(2, mdim);

        // transpose the column major data in memory to the row major data in the file by
        // writing one row slab at a time. 
        for (hsize_t i = 0; i < rows; i++)
        {
            hsize_t fstart[2] = { i, 0 };
            hsize_t mstart[2] = { 0, i };
            fspace.selectHyperslab(H5S_SELECT_SET, fcount, fstart, fstride, fblock);
            mspace.selectHyperslab(H5S_SELECT_SET, mcount, mstart, mstride, mblock);
            dataset.read(const_cast<Eigen::DenseBase<Derived> &>(mat).derived().data(), *datatype, mspace, fspace);
        }
        return true;
    }

    template <typename Derived, typename DataSet>
    void _load (const DataSet &dataset, const Eigen::DenseBase<Derived> &mat) {
            typedef typename Derived::Scalar Scalar;
            const H5::DataSpace dataspace = dataset.getSpace();
            const std::size_t ndims = dataspace.getSimpleExtentNdims();
            assert(ndims > 0);
            const std::size_t dimensions_size = 2;
            hsize_t dimensions[dimensions_size];
            dimensions[1] = 1; // in case it's 1D
            if (ndims > dimensions_size) {
                throw std::runtime_error("HDF5 array has too many dimensions.");
            }
            dataspace.getSimpleExtentDims(dimensions);
            const hsize_t rows = dimensions[0], cols = dimensions[1];
            const H5::DataType * const datatype = DatatypeSpecialization<Scalar>::get();
            Eigen::DenseBase<Derived> &mat_ = const_cast<Eigen::DenseBase<Derived> &>(mat);
            mat_.derived().resize(rows, cols);
            bool written = false;
            bool isRowMajor = mat.Flags & Eigen::RowMajor;
            if (isRowMajor || dimensions[0] == 1 || dimensions[1] == 1)
            {
                // mat is already row major
                typename Derived::Index istride = mat_.derived().outerStride();
                assert(istride >= 0);
                hsize_t stride = istride >= 0 ? istride : 0;
                if (stride == cols || (stride == rows && cols == 1))
                {
                    // mat has natural stride, so read directly into its data block
                    read_data(dataset, mat_.derived().data(), *datatype);
                    written = true;
                }
            }
            else 
            {
                // colmajor flag is 0 so the assert needs to check that mat is not rowmajor. 
                assert(!(mat.Flags & Eigen::RowMajor));

                written = read_colmat(mat_, datatype, dataset);
            }

            if (!written)
            {
                // dataset has not been loaded directly into mat_, so as a last resort read it into a
                // temp and copy it to mat_. (Should only need to do this when the mat_ to be loaded
                // into has an unnatural stride.) 
                Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> temp(rows, cols);
                internal::read_data(dataset, temp.data(), *datatype);
                mat_ = temp;
                written = true;
            }

    }
}

template <typename Derived>
void load (const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Derived> &mat) {
    const H5::DataSet dataset = h5group.openDataSet(name);
    internal::_load(dataset, mat);
}
template <typename Derived>
void save (H5::H5Location &h5group, const std::string &name,
        const Eigen::EigenBase<Derived> &mat, 
        const H5::DSetCreatPropList &plist) {
    typedef typename Derived::Scalar Scalar;
    const H5::DataType * const datatype = DatatypeSpecialization<Scalar>::get();
    const H5::DataSpace dataspace = internal::create_dataspace(mat);
    H5::DataSet dataset = h5group.createDataSet(name, *datatype, dataspace, plist);

    bool written = false;  // flag will be true when the data has been written
    if (mat.derived().Flags & Eigen::RowMajor)
    {
        written = internal::write_rowmat(mat, datatype, &dataset, &dataspace);
    }
    else
    {
        written = internal::write_colmat(mat, datatype, &dataset, &dataspace);
    }
    
    if (!written)
    {
        // data has not yet been written, so there is nothing else to try but copy the input
        // matrix to a row major matrix and write it. 
        const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> row_major_mat(mat);
        dataset.write(row_major_mat.data(), *datatype);
    }
}

// Big Save template specification
template void save<Eigen::Matrix<double, -1, 3> >(H5::H5Location& h5group,const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> > &mat, const H5::DSetCreatPropList& plist);
template void save<Eigen::Matrix<int, -1, 3> >(H5::H5Location& h5group,const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> > &mat, const H5::DSetCreatPropList& plist);

template void save<Eigen::Matrix<double, -1, -1> >(H5::H5Location& h5group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &mat, const H5::DSetCreatPropList &plist);
template void save<Eigen::Matrix<int, -1, -1> >(H5::H5Location& h5group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &mat, const H5::DSetCreatPropList &plist);

template void save<Eigen::Matrix<double, 1, 18> >(H5::H5Location& h5group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &mat, const H5::DSetCreatPropList &plist);
template void save<Eigen::Matrix<int, 1, 18> >(H5::H5Location& h5group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &mat, const H5::DSetCreatPropList &plist);

template void save<Eigen::Matrix<double, 1, -1> >(H5::H5Location& h5group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &mat, const H5::DSetCreatPropList &plist);
template void save<Eigen::Matrix<int, 1, -1> >(H5::H5Location& h5group, const std::string &name, const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &mat, const H5::DSetCreatPropList &plist);

// Explicit template specialization
template H5::DataSpace internal::create_dataspace<Eigen::Matrix<double, -1 , 3> >(const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> >&);
template H5::DataSpace internal::create_dataspace<Eigen::Matrix<int, -1 , 3> >(const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> >&);

template bool internal::write_rowmat<Eigen::Matrix<double, -1, 3> > (const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> >& mat,
        const H5::DataType * const datatype,
        H5::DataSet* dataset,
        const H5::DataSpace* dspace);
template bool internal::write_rowmat<Eigen::Matrix<int, -1, 3> > (const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> >& mat,
        const H5::DataType * const datatype,
        H5::DataSet* dataset,
        const H5::DataSpace* dspace);

template bool internal::write_colmat<Eigen::Matrix<double, -1, 3> > (const Eigen::EigenBase<Eigen::Matrix<double, -1, 3> >& mat,
        const H5::DataType * const datatype,
        H5::DataSet* dataset,
        const H5::DataSpace* dspace);
template bool internal::write_colmat<Eigen::Matrix<int, -1, 3> > (const Eigen::EigenBase<Eigen::Matrix<int, -1, 3> >& mat,
        const H5::DataType * const datatype,
        H5::DataSet* dataset,
        const H5::DataSpace* dspace);

template bool internal::read_colmat<Eigen::Matrix<double, -1, 3> > (const Eigen::DenseBase<Eigen::Matrix<double, -1, 3> > &mat,
        const H5::DataType * const datatype,
        const H5::Attribute &dataset);
template bool internal::read_colmat<Eigen::Matrix<int, -1, 3> > (const Eigen::DenseBase<Eigen::Matrix<int, -1, 3> > &mat,
        const H5::DataType * const datatype,
        const H5::Attribute &dataset);

template bool internal::read_colmat<Eigen::Matrix<double, -1, 3> > (const Eigen::DenseBase<Eigen::Matrix<double, -1, 3> > &mat,
        const H5::DataType * const datatype,
        const H5::DataSet &dataset);
template bool internal::read_colmat<Eigen::Matrix<int, -1, 3> > (const Eigen::DenseBase<Eigen::Matrix<int, -1, 3> > &mat,
        const H5::DataType * const datatype,
        const H5::DataSet &dataset);

// internal::write_rowmat

template bool internal::write_rowmat<Eigen::Matrix<double, -1, -1> >(const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);
template bool internal::write_rowmat<Eigen::Matrix<int, -1, -1> >(const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);

template bool internal::write_rowmat<Eigen::Matrix<double, 1, 18> >(const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);
template bool internal::write_rowmat<Eigen::Matrix<int, 1, 18> >(const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);

template bool internal::write_rowmat<Eigen::Matrix<double, 1, -1> >(const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);
template bool internal::write_rowmat<Eigen::Matrix<int, 1, -1> >(const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);

// internal::write_colmat
template bool internal::write_colmat<Eigen::Matrix<double, -1, -1> >(const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);
template bool internal::write_colmat<Eigen::Matrix<int, -1, -1> >(const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);

template bool internal::write_colmat<Eigen::Matrix<double, 1, 18> >(const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);
template bool internal::write_colmat<Eigen::Matrix<int, 1, 18> >(const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);

template bool internal::write_colmat<Eigen::Matrix<double, 1, -1> >(const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);
template bool internal::write_colmat<Eigen::Matrix<int, 1, -1> >(const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &, H5::DataType const*, H5::DataSet*, H5::DataSpace const*);

// internal::create_dataspace
template H5::DataSpace internal::create_dataspace<Eigen::Matrix<double, 1, -1> >(const Eigen::EigenBase<Eigen::Matrix<double, 1, -1> > &);
template H5::DataSpace internal::create_dataspace<Eigen::Matrix<int, 1, -1> >(const Eigen::EigenBase<Eigen::Matrix<int, 1, -1> > &);

template H5::DataSpace internal::create_dataspace<Eigen::Matrix<double, -1, -1> >(const Eigen::EigenBase<Eigen::Matrix<double, -1, -1> > &);
template H5::DataSpace internal::create_dataspace<Eigen::Matrix<int, -1, -1> >(const Eigen::EigenBase<Eigen::Matrix<int, -1, -1> > &);

template H5::DataSpace internal::create_dataspace<Eigen::Matrix<double, 1, 18> >(const Eigen::EigenBase<Eigen::Matrix<double, 1, 18> > &);
template H5::DataSpace internal::create_dataspace<Eigen::Matrix<int, 1, 18> >(const Eigen::EigenBase<Eigen::Matrix<int, 1, 18> > &);


// Big load template specificaitons
template void internal::_load<Eigen::Matrix<double, -1, 3>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<double, -1, 3> > const& mat);
template void internal::_load<Eigen::Matrix<int, -1, 3>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<int, -1, 3> > const& mat);

template void internal::_load<Eigen::Matrix<double, -1, -1>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<double, -1, -1> > const& mat);
template void internal::_load<Eigen::Matrix<int, -1, -1>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<int, -1, -1> > const& mat);

template void internal::_load<Eigen::Matrix<double, -1, 18>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<double, -1, 18> > const& mat);
template void internal::_load<Eigen::Matrix<int, -1, 18>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<int, -1, 18> > const& mat);

template void internal::_load<Eigen::Matrix<double, 1, 18>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<double, 1, 18> > const& mat);
template void internal::_load<Eigen::Matrix<int, 1, 18>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<int, 1, 18> > const& mat);

template void internal::_load<Eigen::Matrix<double, 1, -1>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<double, 1, -1> > const& mat);
template void internal::_load<Eigen::Matrix<int, 1, -1>, H5::DataSet>(H5::DataSet const& dataset, Eigen::DenseBase<Eigen::Matrix<int, 1, -1> > const& mat);

template void load<Eigen::Matrix<double, -1, 3> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, -1, 3> > & mat);
template void load<Eigen::Matrix<int, -1, 3> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, -1, 3> > & mat);

template void load<Eigen::Matrix<double, -1, -1> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, -1, -1> > & mat);
template void load<Eigen::Matrix<int, -1, -1> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, -1, -1> > & mat);

template void load<Eigen::Matrix<double, 1, 18> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, 1, 18> > & mat);
template void load<Eigen::Matrix<int, 1, 18> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, 1, 18> > & mat);

template void load<Eigen::Matrix<double, 1, -1> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<double, 1, -1> > & mat);
template void load<Eigen::Matrix<int, 1, -1> >(const H5::H5Location &h5group, const std::string &name, const Eigen::DenseBase<Eigen::Matrix<int, 1, -1> > & mat);

