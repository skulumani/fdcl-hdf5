#!/bin/bash

EIGEN_VER=3.3.4
INSTALL_DIR="/usr/local/include"
EIGEN_RELEASE_URL="https://github.com/eigenteam/eigen-git-mirror/archive/${EIGEN_VER}.tar.gz"
TEMP_DIR="$(mktemp -d)"
WORK_DIR="$(pwd)"

HDF5_VER=1.10.2
INSTALL_DIR="/usr/local/include"
# HDF5_RELEASE_URL="https://support.hdfgroup.org/ftp/HDF5/current/src/hdf5-${HDF5_VER}.tar.gz"
HDF5_RELEASE_URL="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.2/src/hdf5-${HDF5_VER}.tar.gz"

# download  latest release of eigen
if [[ ! "$TEMP_DIR" || ! -d "$TEMP_DIR" ]]; then
	echo "Could not create temp dir"
	exit 1
fi

# delete the temp directory on cleanup
function cleanup {
    rm -rf "$TEMP_DIR"
    echo "Deleted temp working directory $TEMP_DIR"
}

trap cleanup EXIT

echo "We're going to download Eigen ${EIGEN_VER} and install to ${INSTALL_DIR}"
cd ${TEMP_DIR}
mkdir ${EIGEN_VER}
wget ${EIGEN_RELEASE_URL} -O ${TEMP_DIR}/${EIGEN_VER}.tar.gz
tar -xvzf ${EIGEN_VER}.tar.gz -C ./${EIGEN_VER} --strip-components=1

echo "Going to install Eigen using CMake"
cd ${EIGEN_VER}
mkdir build
cd build
cmake ..
sudo make install

echo "Eigen installed"

echo "We're going to download HDF5 ${HDF5_VER} and install to ${INSTALL_DIR}"
cd ${TEMP_DIR}
mkdir ${HDF5_VER}
wget ${HDF5_RELEASE_URL} -O ${TEMP_DIR}/${HDF5_VER}.tar.gz
tar -xvzf ${HDF5_VER}.tar.gz -C ./${HDF5_VER} --strip-components=1

echo "Going to install HDF5 using the configure script"
cd ${HDF5_VER}
bash ./configure --prefix=/usr/local --enable-cxx 
make -j5
# make check
sudo make install

echo "Finished installing HDF5"
