#!/bin/bash
. util.functions.sh

export DEVCOLLECTER_ROOT_DIR=/usr/local/DevCollecter
if [ -z "$1" ]
then
   echo "you dont input install dir for dispserver.we will use ${DEVCOLLECTER_ROOT_DIR} as default." 
else 
   DEVCOLLECTER_ROOT_DIR=$1
   echo "you input install dir ${DEVCOLLECTER_ROOT_DIR} for dispserver."
fi


if [ -f "${DEVCOLLECTER_ROOT_DIR}/dev_collecter" ]
then
   echo "install dir is ${DEVCOLLECTER_ROOT_DIR},but exists a dev_collecter in this dir.check it and uninstall first."
   exit 1
fi 

if [ -d "/usr/local/logs" ]
then
    echo "/usr/local/logs exists now"
else
    mkdir /usr/local/logs
fi

echo "===============start check preinstall now===================="
./pre_install_dev_collecter.sh
if [ $? -ne 0 ]
then
   exit 1 
fi
echo "===============check preinstall ok===================="


echo "===============stop all service before install===================="
# TODO: Stop service

echo "===============start install now===================="

if [ -d "${DEVCOLLECTER_ROOT_DIR}" ]
then
   echo "${DEVCOLLECTER_ROOT_DIR} is exist now,rename it first"
   CUR_TIME=`date "+%Y-%m-%d-%H-%M-%S"`
   mv ${DEVCOLLECTER_ROOT_DIR} "${DEVCOLLECTER_ROOT_DIR}_${CUR_TIME}"
else
   echo "${DEVCOLLECTER_ROOT_DIR} is not exist now,first mkdir."
fi

rm -rf ${DEVCOLLECTER_ROOT_DIR}
mkdir -p ${DEVCOLLECTER_ROOT_DIR}
if [ -d ${DEVCOLLECTER_ROOT_DIR} ]
then
   echo "${DEVCOLLECTER_ROOT_DIR} create success." 
else
   echo "create ${DEVCOLLECTER_ROOT_DIR} failed.please check path."             
fi


echo "unzip install.tar.gz now"
tar -zxf install.tar.gz 
echo "unzip install.tar.gz done"

echo "start copy file now"
cp -rf ./install/* ${DEVCOLLECTER_ROOT_DIR}/
echo "copy file done"

chmod -R +x ${DEVCOLLECTER_ROOT_DIR}

#modify supervisord conf 


echo "===============install dispserver done in ${DEVCOLLECTER_ROOT_DIR}===================="

echo "===============restart supervisord now===================="
#/etc/init.d/supervisord restart

