#!/bin/bash

EIGEN_VER=3.3.4
INSTALL_DIR="/usr/local/include"
EIGEN_RELEASE_URL="https://github.com/eigenteam/eigen-git-mirror/archive/${EIGEN_VER}.tar.gz"
TEMP_DIR="$(mktemp -d)"

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

sudo apt-get update
sudo apt-get install -y libeigen3-dev libhdf5-dev libhdf5-serial-dev libhdf5-openmpi-dev hdf5-tools hdf5-helpers
