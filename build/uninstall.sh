#!/bin/bash

DEV_COLLECTER_ROOT_DIR=$PWD

if [ -f ${PWD}/dev_collecter ]
then
    echo "start uninstall dispserver in ${DEV_COLLECTER_ROOT_DIR} now!"
else
    echo "dispserver not installed in ${DEV_COLLECTER_ROOT_DIR}.see you!"
    exit 0 
fi

echo "===============stop all service before uninstall===================="
# TODO: Stop service
#unreg dispserver from supervisord
#python mod_supervisord.py ${SUPERVISOR_CONF} ${DEV_COLLECTER_ROOT_DIR} unreg 127.0.0.1

echo "===============restart supervisord now===================="


echo "===============remove dispserver now===================="
rm -rf ${DEV_COLLECTER_ROOT_DIR}

echo "===============uninstall dispserver done===================="
