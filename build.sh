#!/bin/bash

rm -rf ./install 
mkdir ./install
cp -rf ./dev_collecter ./install
cp -rf ./config.ini   ./install
cp -rf ./ReleaseNote.txt ./install
# which sqlite3 > /dev/null 2>&1
# if [ $? == 0 ]; then
#     echo "sqlite3 is installed"
# else
#     echo "dose not exist"
# fi
cp -rf ./vendor/sqlite3.tar.gz ./install


PRODUCT_NAME="dev_collecter"
MAJOR_VERSION="1"
SUB_VERSION="0"
THIRD_VERSION="0"
echo "product.name=${PRODUCT_NAME}" > ./install/version.ini
echo "product.major.version=${MAJOR_VERSION}"  >> ./install/version.ini
echo "product.sub.version=${SUB_VERSION}"  >> ./install/version.ini
echo "product.sub.version=${THIRD_VERSION}"  >> ./install/version.ini
#SVN_BUILD_VERSION=`svn info https://10.1.10.4:8443/svn/MediaServer/Dispatcher/trunk|grep 'Last Changed Rev'| cut -d: -f2 |sed 's/^ //;s/ $//'`
#echo "product.build.version=${SVN_BUILD_VERSION}"  >> ./install/version.ini
BUILD_TIME=`date '+%Y%m%d%H%M%S'`
echo "product.build.date=${BUILD_TIME}"  >> ./install/version.ini

cp -rf ./build/* ./install
rm -rf ./install/install.sh

tar zcvf install.tar.gz install 
 
rm -rf ./DevCollecter 
mkdir ./DevCollecter
cp -rf ./install.tar.gz ./DevCollecter
cp -rf ./build/install.sh ./DevCollecter
cp -rf ./build/pre_install_dev_collecter.sh ./DevCollecter
cp -rf ./build/util.functions.sh ./DevCollecter


#packet_name="${PRODUCT_NAME}-V${MAJOR_VERSION}.${SUB_VERSION}.${THIRD_VERSION}.${SVN_BUILD_VERSION}.${BUILD_TIME}.linux.tar.gz"
packet_name="${PRODUCT_NAME}-V${MAJOR_VERSION}.${SUB_VERSION}.${THIRD_VERSION}.${BUILD_TIME}.linux.tar.gz"

echo ${packet_name}
tar zcvf ${packet_name} DevCollecter 
