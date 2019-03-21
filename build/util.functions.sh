#!/bin/bash

check_program()
{
    echo "begin test program $1 now."
    test_program_result=`which $1 2>/dev/null`
	
	if [ -z "${test_program_result}" ]
	then
	    echo "test program $1 failed."
		return 1
	else
	    echo "test program $1 success."
		return 0
	fi
}

check_ip_num()
{
    num=`ifconfig  | grep 'inet addr:'| grep -v '127.0.0.1' | cut -d: -f2 |awk '{print $1}'|wc -l`
    return ${num}
}

check_ip_valid()
{
    input_ip=$1
    num=`ifconfig  | grep 'inet addr:'| grep -v '127.0.0.1' | grep "${input_ip}"|wc -l`
    #ifconfig  | grep 'inet addr:'| grep -v '127.0.0.1' | grep "${input_ip}"|wc -l
    #echo "ip:${input_ip} num:${num}"
    if [ ${num} -gt  0 ];
    then 
        return 0
    fi
	
    return 1
}
