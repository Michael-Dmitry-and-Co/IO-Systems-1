#! /bin/bash
SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd ${SCRIPTPATH}/..

echo "Building driver"
cd driver
make
cd ..

echo "Building user"
cd user
make
cd ..

echo "Installing module"
insmod driver/build/fan_driver.ko

echo "Build and install finished"