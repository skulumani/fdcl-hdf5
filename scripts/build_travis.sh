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

# This will download the latest eigen and install for the sy tem
echo "Going to install the latest Eigen"

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

# move the eigen subdirectory to /usr/local/include
if [[ -d "${INSTALL_DIR}/Eigen" ]]; then
	echo "Eigen directory already  exists"
	rm -rf "${INSTALL_DIR}/Eigen"
fi

# delete the unsupported directory
if [[ -d "${INSTALL_DIR}/unsupported" ]]; then
    echo "Eigen unsupported directory already exists"
    rf -rf "${INSTALL_DIR}/Eigen"
fi

# echo "Now going to copy Eigen headers to /usr/include/local"
# read -p "Press enter to continue"

# # copy to /usr/local/include
# echo "Now copying to ${INSTALL_DIR}/Eigen"
# sudo mv ${EIGEN_VER}/Eigen ${INSTALL_DIR}

# # copy to usr/local/include
# echo "Now copying to ${INSTALL_DIR}/unsupported"
# sudo mv ${EIGEN_VER}/unsupported ${INSTALL_DIR}

# echo "Eigen ${EIGEN_VER} installed to ${INSTALL_DIR}/Eigen and ${INSTALL_DIR}/unsupported"

echo "Going to install Eigen using CMake"
cd ${EIGEN_VER}
mkdir build
cd build
cmake ..
sudo make install

echo "Eigen installed"

sudo apt-get update
sudo apt-get install -y libhdf5-dev libhdf5-serial-dev hdf5-tools hdf5-helpers


