#!/bin/bash

. util.functions.sh


check_sqlit3()
{
	#check_program "python" 
	#if [ $? -ne 0 ]
	#then 
	#    echo "please install python 2.6 first."
	#	return 1
	#fi
	
	#TODO:check python version here
	
	#return 0

	which want_to_find > /dev/null 2>&1
	if [ $? == 0 ]; then
    	echo "exist"
    	return 0
	else
    	echo "dose not exist"
    	return 1
	fi
}


check_sqlit3
if [ $? -ne 0 ]
then
   exit 1
fi
exit 0
