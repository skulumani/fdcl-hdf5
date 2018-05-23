#!/bin/bash

# CMAKE
VERSION=3.11
BUILD="0-rc2"
TEMP_DIR="$(mktemp -d)"

sudo apt-get -y purge cmake
sudo apt-get -y install checkinstall

cd ${TEMP_DIR}
wget https://cmake.org/files/v$VERSION/cmake-$VERSION.$BUILD.tar.gz
tar -xzvf cmake-$VERSION.$BUILD.tar.gz
cd cmake-$VERSION.$BUILD/

./bootstrap
make -j 4
sudo make install
