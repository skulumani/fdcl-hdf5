#!/bin/bash

HDF5_VER=1.10.2
INSTALL_DIR="/usr/local/include"
# HDF5_RELEASE_URL="https://support.hdfgroup.org/ftp/HDF5/current/src/hdf5-${HDF5_VER}.tar.gz"
HDF5_RELEASE_URL="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.2/src/hdf5-${HDF5_VER}.tar.gz"
TEMP_DIR="$(mktemp -d)"

# This will download the latest eigen and install for the sy tem
echo "Going to install the latest HDF5 library"
read -p "Press Enter to continue........"

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

# trap cleanup EXIT

echo "We're going to download HDF5 ${HDF5_VER} and install to ${INSTALL_DIR}"
cd ${TEMP_DIR}
mkdir ${HDF5_VER}
wget ${HDF5_RELEASE_URL} -O ${TEMP_DIR}/${HDF5_VER}.tar.gz
tar -xvzf ${HDF5_VER}.tar.gz -C ./${HDF5_VER} --strip-components=1

echo "Going to install HDF5 using the configure script"
read -p "Press enter to continue"
cd ${HDF5_VER}
bash ./configure --prefix=/usr/local/hdf5 --enable-cxx 
make -j5
# make check
sudo checkinstall make install
sudo make check-install

read -p "Press enter to exit"
